cmake_minimum_required(VERSION 3.15)
project(PDFCracker)

set(CMAKE_CXX_STANDARD 17)

# Make sure to configure your project using the vcpkg toolchain:
# cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg_root]/scripts/buildsystems/vcpkg.cmake ..
include_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include)
link_directories(${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib)

# Find libraries using vcpkg integration
find_package(libzip CONFIG REQUIRED)

# Add source files
add_executable(PDFCracker
    src/main.cpp
    src/Pdf/Pdf.cpp
    src/Zip/Zip.cpp
    
    src/Pdf/Pdf.h
    src/Zip/Zip.h
)

# Link libraries
target_link_libraries(PDFCracker PRIVATE
    poppler-cpp
    libzip::zip
)
