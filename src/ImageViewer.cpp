#include "ImageViewer.hpp"
#include "FileHandler.hpp"
#include <hyprtoolkit/palette/Palette.hpp>
#include <hyprtoolkit/types/FontTypes.hpp>
#include <xkbcommon/xkbcommon-keysyms.h>
#include <chrono>
#include <iostream>

using namespace Hyprtoolkit;
using namespace Hyprutils::Memory;
using namespace Hyprutils::Math;

ImageViewer::ImageViewer(CSharedPointer<IBackend> backend, CSharedPointer<IWindow> window)
    : m_backend(backend)
    , m_window(window) {
    
    createUI();
    setupKeyboardHandlers();
    
    // Handle window close (killactive)
    m_listeners.push_back(
        m_window->m_events.closeRequest.listen([this]() {
            m_window->close();
            m_backend->addIdle([backend = m_backend]() { backend->destroy(); });
        })
    );
}

ImageViewer::~ImageViewer() {
    if (m_statusTimer) {
        m_statusTimer->cancel();
        m_statusTimer = nullptr;
    }
    m_listeners.clear();
}

void ImageViewer::createUI() {
    auto palette = CPalette::palette();
    if (!palette) {
        std::cerr << "Warning: Could not get palette" << std::endl;
    }
    
    m_mainLayout = CColumnLayoutBuilder::begin()
        ->gap(0)
        ->size(CDynamicSize(
            CDynamicSize::HT_SIZE_PERCENT,
            CDynamicSize::HT_SIZE_PERCENT,
            Vector2D(1.0f, 1.0f)))
        ->commence();
    
    // Image container
    m_background = CRectangleBuilder::begin()
        ->color([palette] { 
            return palette ? palette->m_colors.background : CHyprColor(0.1f, 0.1f, 0.1f, 1.0f); 
        })
        ->size(CDynamicSize(
            CDynamicSize::HT_SIZE_PERCENT,
            CDynamicSize::HT_SIZE_PERCENT,
            Vector2D(1.0f, 1.0f)))
        ->commence();
    
    m_imageWidget = std::make_shared<ImageWidget>(m_backend);
    m_background->addChild(m_imageWidget->getWidget());
    
    // Status bar at bottom
    m_statusBar = CTextBuilder::begin()
        ->text("")
        ->color([palette] { 
            auto c = palette ? palette->m_colors.text : CHyprColor(1.0f, 1.0f, 1.0f, 1.0f);
            c.a = 0.0f;
            return c;
        })
        ->fontSize(CFontSize(CFontSize::HT_FONT_SMALL))
        ->align(HT_FONT_ALIGN_CENTER)
        ->size(CDynamicSize(
            CDynamicSize::HT_SIZE_PERCENT,
            CDynamicSize::HT_SIZE_ABSOLUTE,
            Vector2D(1.0f, 24.0f)))
        ->commence();
    
    m_imageWidget->onImageLoaded = [this]() {
        showStatus(fs::path(m_currentPath).filename().string());
    };
    
    m_mainLayout->addChild(m_background);
    m_mainLayout->addChild(m_statusBar);
}

void ImageViewer::setupKeyboardHandlers() {
    m_listeners.push_back(
        m_window->m_events.keyboardKey.listen([this](const Input::SKeyboardKeyEvent& e) {
            if (!e.down) return;
            
            switch (e.xkbKeysym) {
                case XKB_KEY_q:
                case XKB_KEY_Q:
                    m_window->close();
                    m_backend->addIdle([backend = m_backend]() { backend->destroy(); });
                    break;
                case XKB_KEY_plus:
                case XKB_KEY_equal:
                    m_imageWidget->zoomIn();
                    showStatus("Zoom: " + std::to_string((int)(m_imageWidget->getZoom() * 100)) + "%");
                    break;
                case XKB_KEY_minus:
                    m_imageWidget->zoomOut();
                    showStatus("Zoom: " + std::to_string((int)(m_imageWidget->getZoom() * 100)) + "%");
                    break;
                case XKB_KEY_0:
                    m_imageWidget->zoomReset();
                    m_imageWidget->setFitMode(true);
                    showStatus("Actual size");
                    break;
                case XKB_KEY_space:
                    m_imageWidget->toggleFitMode();
                    showStatus(m_imageWidget->getFitMode() ? "Fit to window" : "Zoom: " + 
                              std::to_string((int)(m_imageWidget->getZoom() * 100)) + "%");
                    break;
                case XKB_KEY_Right:
                case XKB_KEY_n:
                case XKB_KEY_N:
                    nextImage();
                    break;
                case XKB_KEY_Left:
                case XKB_KEY_p:
                case XKB_KEY_P:
                    prevImage();
                    break;
                case XKB_KEY_i:
                case XKB_KEY_I:
                    showInfo();
                    break;
            }
        })
    );
}

CSharedPointer<IElement> ImageViewer::getWidget() const {
    return m_mainLayout;
}

void ImageViewer::loadImage(const std::string& path) {
    if (!FileHandler::isImageFile(path)) {
        showStatus("Not an image file: " + fs::path(path).filename().string());
        return;
    }
    
    m_currentPath = path;
    m_imageWidget->loadImage(path);
}


void ImageViewer::nextImage() {
    if (m_currentPath.empty()) return;
    std::string next = FileHandler::getNextImage(m_currentPath);
    if (next != m_currentPath) {
        loadImage(next);
    }
}

void ImageViewer::prevImage() {
    if (m_currentPath.empty()) return;
    std::string prev = FileHandler::getPrevImage(m_currentPath);
    if (prev != m_currentPath) {
        loadImage(prev);
    }
}

void ImageViewer::showInfo() {
    if (m_currentPath.empty()) return;
    
    fs::path p(m_currentPath);
    std::string info = p.filename().string() + " | " + 
                       std::to_string(fs::file_size(p) / 1024) + " KB";
    showStatus(info, 3000);
}

void ImageViewer::showStatus(const std::string& message, int timeoutMs) {
    auto palette = CPalette::palette();
    
    if (m_statusTimer) {
        m_statusTimer->cancel();
        m_statusTimer = nullptr;
    }
    
    if (auto builder = m_statusBar->rebuild()) {
        builder->text(std::string(message))
               ->color([palette] { 
                   return palette ? palette->m_colors.text : CHyprColor(1.0f, 1.0f, 1.0f, 1.0f); 
               })
               ->commence();
    }
    
    m_statusTimer = m_backend->addTimer(
        std::chrono::milliseconds(timeoutMs),
        [this](CAtomicSharedPointer<CTimer> self, void*) {
            auto palette = CPalette::palette();
            if (auto builder = m_statusBar->rebuild()) {
                builder->color([palette] { 
                    auto c = palette ? palette->m_colors.text : CHyprColor(1.0f, 1.0f, 1.0f, 1.0f);
                    c.a = 0.0f;
                    return c;
                })->commence();
            }
            m_statusTimer = nullptr;
        },
        nullptr, false
    );
}
