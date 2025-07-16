# FileDictCracker

A password cracking tool that supports PDF and ZIP files using dictionary attacks.

## Getting Started

## Project Setup

This project uses vcpkg for package management. Make sure you have vcpkg installed and properly configured before proceeding.

## Prerequisites

- vcpkg package manager installed on your system
- CMake
- Visual Studio 2022 (recommended) or compatible C++ compiler

## Setup Steps

### 1. Install vcpkg
Install vcpkg on your PC and add its location to your system's PATH environment variable.

### 2. Clone and Navigate
Clone the repository and navigate to the project folder:
```bash
git clone https://github.com/Candyris/FileDictCracker.git
cd FileDictCracker
```

### 3. Install Dependencies
Navigate to the project folder and run:
```bash
vcpkg install
```

### 4. Configure CMake
Run the following command, replacing `<path_where_vcpkg_installed>` with your actual vcpkg installation path:

```bash
cmake --fresh -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/<path_where_vcpkg_installed>/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows
```

### 5. Build the Project
Choose one of the following options:

**Option A: Build with CMake**
```bash
cmake --build build
```

**Option B: Build with Visual Studio**
1. Open the generated solution file in Visual Studio 2022
2. Set the current project as the startup project
3. Build and run (Debug/Release mode)

### 6. Run the Application
After successful build, you can run the executable:

```bash
FileDictCracker.exe <password_dictionary> <document>
```

**Examples:**
```bash
FileDictCracker.exe passwords.txt document.pdf
FileDictCracker.exe wordlist.txt archive.zip
```

**Supported file types:** PDF (.pdf), ZIP (.zip)

## Troubleshooting

- Ensure vcpkg is properly installed and added to PATH
- Verify that the vcpkg toolchain file path is correct
- Make sure you're using the correct target triplet for your system
