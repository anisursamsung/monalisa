#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/window/Window.hpp>
#include <iostream>
#include <memory>

#include "ImageViewer.hpp"
#include "FileHandler.hpp"

using namespace Hyprtoolkit;
using namespace Hyprutils::Memory;
using namespace Hyprutils::Math;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: monalisa <image-file>" << std::endl;
        return 1;
    }
    
    std::string imagePath = argv[1];
    
    if (!FileHandler::isImageFile(imagePath)) {
        std::cerr << "Error: Not a valid image file: " << imagePath << std::endl;
        return 1;
    }
    
    auto backend = IBackend::create();
    if (!backend) {
        std::cerr << "Failed to create backend" << std::endl;
        return 1;
    }
    
    auto window = CWindowBuilder::begin()
        ->type(HT_WINDOW_TOPLEVEL)
        ->appTitle("Monalisa")
        ->appClass("monalisa")
        ->preferredSize(Vector2D(0, 0)) 
        ->commence();
    
    auto viewer = std::make_shared<ImageViewer>(backend, window);
    viewer->loadImage(imagePath);
    
    window->m_rootElement = viewer->getWidget();
    window->open();
    
    backend->enterLoop();
    
    return 0;
}
