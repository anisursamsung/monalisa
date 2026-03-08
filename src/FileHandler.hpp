#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

class FileHandler {
public:
    static bool isImageFile(const std::string& path);
    static std::string getNextImage(const std::string& current);
    static std::string getPrevImage(const std::string& current);
    static std::vector<std::string> getImagesInDirectory(const std::string& path);
    
private:
    static const std::vector<std::string> s_imageExtensions;
};
