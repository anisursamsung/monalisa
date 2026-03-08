#include "FileHandler.hpp"
#include <iostream>

const std::vector<std::string> FileHandler::s_imageExtensions = {
    ".png", ".jpg", ".jpeg", ".gif", ".bmp", ".webp", ".svg"
};

bool FileHandler::isImageFile(const std::string& path) {
    if (!fs::exists(path) || !fs::is_regular_file(path))
        return false;
    
    std::string ext = fs::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    return std::find(s_imageExtensions.begin(), s_imageExtensions.end(), ext) != s_imageExtensions.end();
}

std::vector<std::string> FileHandler::getImagesInDirectory(const std::string& path) {
    std::vector<std::string> images;
    fs::path dirPath = fs::path(path).parent_path();
    
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
        return images;
    
    try {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            std::string filePath = entry.path().string();
            if (isImageFile(filePath)) {
                images.push_back(filePath);
            }
        }
        std::sort(images.begin(), images.end());
    } catch (const std::exception& e) {
        std::cerr << "Error reading directory: " << e.what() << std::endl;
    }
    
    return images;
}

std::string FileHandler::getNextImage(const std::string& current) {
    auto images = getImagesInDirectory(current);
    if (images.size() <= 1) return current;
    
    auto it = std::find(images.begin(), images.end(), current);
    if (it == images.end() || it + 1 == images.end())
        return images.front();
    
    return *(it + 1);
}

std::string FileHandler::getPrevImage(const std::string& current) {
    auto images = getImagesInDirectory(current);
    if (images.size() <= 1) return current;
    
    auto it = std::find(images.begin(), images.end(), current);
    if (it == images.begin() || it == images.end())
        return images.back();
    
    return *(it - 1);
}
