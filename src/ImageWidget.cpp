#include "ImageWidget.hpp"
#include <hyprtoolkit/palette/Palette.hpp>
#include <hyprtoolkit/types/ImageTypes.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace Hyprtoolkit;
using namespace Hyprutils::Memory;
using namespace Hyprutils::Math;

ImageWidget::ImageWidget(CSharedPointer<IBackend> backend)
    : m_backend(backend) {
    
    // Create a container that fills the available space
    m_container = CRectangleBuilder::begin()
        ->color([] { return CHyprColor(0,0,0,0); })
        ->size(CDynamicSize(
            CDynamicSize::HT_SIZE_PERCENT,
            CDynamicSize::HT_SIZE_PERCENT,
            Vector2D(1.0f, 1.0f)))
        ->commence();
    
    // Create image that fills the container
    m_image = CImageBuilder::begin()
        ->size(CDynamicSize(
            CDynamicSize::HT_SIZE_PERCENT,
            CDynamicSize::HT_SIZE_PERCENT,
            Vector2D(1.0f, 1.0f)))
        ->fitMode(IMAGE_FIT_MODE_CONTAIN)
        ->sync(false)
        ->commence();
    
    m_container->addChild(m_image);
}

ImageWidget::~ImageWidget() {
    // Cleanup if needed
}

CSharedPointer<IElement> ImageWidget::getWidget() const {
    return m_container;
}

void ImageWidget::loadImage(const std::string& path) {
    m_currentPath = path;
     
    if (auto builder = m_image->rebuild()) {
        std::string pathCopy = path;
        builder->path(std::move(pathCopy))
               ->fitMode(m_fitMode ? IMAGE_FIT_MODE_CONTAIN : IMAGE_FIT_MODE_STRETCH)
                ->sync(false)
               ->commence();
    }
    
    if (onImageLoaded) {
        onImageLoaded();
    }
}


void ImageWidget::zoomIn() {
    if (m_fitMode) {
        setFitMode(false);
        return;
    }
    m_zoom = std::min(m_zoom * 1.2f, 5.0f);
}

void ImageWidget::zoomOut() {
    if (m_fitMode) {
        setFitMode(false);
        return;
    }
    m_zoom = std::max(m_zoom / 1.2f, 0.2f);
}

void ImageWidget::zoomReset() {
    m_zoom = 1.0f;
}

void ImageWidget::toggleFitMode() {
    setFitMode(!m_fitMode);
}

void ImageWidget::setFitMode(bool fit) {
    m_fitMode = fit;
    if (m_fitMode) {
        m_zoom = 1.0f;
    }
    
    if (auto builder = m_image->rebuild()) {
        builder->fitMode(m_fitMode ? IMAGE_FIT_MODE_CONTAIN : IMAGE_FIT_MODE_STRETCH)
               ->commence();
    }
}

void ImageWidget::updateImage() {
    // Not needed with current implementation
}
