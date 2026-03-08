#pragma once

#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/element/Image.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
#include <hyprutils/math/Vector2D.hpp>
#include <string>
#include <functional>
#include <vector>

class ImageWidget {
public:
    ImageWidget(Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IBackend> backend);
    ~ImageWidget();
    
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IElement> getWidget() const;
    
    void loadImage(const std::string& path);
    void zoomIn();
    void zoomOut();
    void zoomReset();
    void toggleFitMode();
    void setFitMode(bool fit);
    
    float getZoom() const { return m_zoom; }
    bool getFitMode() const { return m_fitMode; }
    const std::string& getCurrentPath() const { return m_currentPath; }
    
    std::function<void()> onImageLoaded;

private:
    void updateImage();
    
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IBackend> m_backend;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CRectangleElement> m_container;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CImageElement> m_image;
    
    std::string m_currentPath;
    float m_zoom = 1.0f;
    bool m_fitMode = true;
};
