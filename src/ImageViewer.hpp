#pragma once

#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/window/Window.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/core/Timer.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <hyprutils/signal/Signal.hpp>
#include <memory>
#include <string>
#include <vector>

#include "ImageWidget.hpp"

class ImageViewer {
public:
    ImageViewer(Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IBackend> backend,
                Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IWindow> window);
    ~ImageViewer();
    
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IElement> getWidget() const;
    
    void loadImage(const std::string& path);
    void nextImage();
    void prevImage();
    void showInfo();
    
private:
    void createUI();
    void showStatus(const std::string& message, int timeoutMs = 2000);
    void setupKeyboardHandlers();
    
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IBackend> m_backend;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IWindow> m_window;
    
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CColumnLayoutElement> m_mainLayout;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CRectangleElement> m_background;
    std::shared_ptr<ImageWidget> m_imageWidget;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CTextElement> m_statusBar;
    
    Hyprutils::Memory::CAtomicSharedPointer<Hyprtoolkit::CTimer> m_statusTimer;
    std::vector<Hyprutils::Memory::CSharedPointer<Hyprutils::Signal::CSignalListener>> m_listeners;
    
    std::string m_currentPath;
};
