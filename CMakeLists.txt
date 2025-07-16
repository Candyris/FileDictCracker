cmake_minimum_required(VERSION 3.15)
project(PDFCracker)

set(CMAKE_CXX_STANDARD 17)

# Use vcpkg
# find_package(ZLIB REQUIRED)
find_package(libzip CONFIG REQUIRED)

# Include Poppler manually from vcpkg
include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include)
link_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib)

add_executable(PDFCracker src/main.cpp src/Pdf/Pdf.h src/Pdf/Pdf.cpp src/Zip/Zip.cpp src/Zip/Zip.h)

# Link manually

target_link_libraries(PDFCracker
poppler-cpp 
libzip::zip)
