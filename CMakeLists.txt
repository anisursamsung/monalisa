cmake_minimum_required(VERSION 3.19)
project(monalisa)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(HYPRTOOLKIT REQUIRED IMPORTED_TARGET hyprtoolkit)
pkg_check_modules(HYPRUTILS REQUIRED IMPORTED_TARGET hyprutils)
pkg_check_modules(AQUAMARINE REQUIRED IMPORTED_TARGET aquamarine)
pkg_check_modules(HYPRGRAPHICS REQUIRED IMPORTED_TARGET hyprgraphics)
pkg_check_modules(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)
pkg_check_modules(LIBDRM REQUIRED IMPORTED_TARGET libdrm)
pkg_check_modules(XKBCOMMON REQUIRED IMPORTED_TARGET xkbcommon)

set(SOURCES
    src/main.cpp
    src/ImageViewer.cpp
    src/ImageWidget.cpp
    src/FileHandler.cpp
  
)

add_executable(monalisa ${SOURCES})

target_include_directories(monalisa PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(monalisa
    PkgConfig::HYPRTOOLKIT
    PkgConfig::HYPRUTILS
    PkgConfig::AQUAMARINE
    PkgConfig::HYPRGRAPHICS
    PkgConfig::PIXMAN
    PkgConfig::LIBDRM
    PkgConfig::XKBCOMMON
    stdc++fs
)
