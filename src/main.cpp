#include <iostream>
#include <string>
#include <memory>
#include "Extension.h"
#include "Pdf/Pdf.h"
#include "Zip/Zip.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <optional>
#include <filesystem>
std::unique_ptr<std::vector<std::string>> loadDictonary(const char* path)
{
    std::ifstream steam(path);
    std::stringstream ss;
    ss << steam.rdbuf();
    std::string line;

    auto lst = std::make_unique<std::vector<std::string>>();

    while (std::getline(ss, line))
    {
        lst->emplace_back(line);
    }
    return std::move(lst);
} 


int main(int argc, char* argv[])
{
    // Check argument count
    if (argc != 3)
    {
        std::cerr << "Error: Invalid number of arguments.\n";
        std::cerr << "Usage: PDFCracker.exe <password_dictionary> <document>\n";
        std::cerr << "Example: PDFCracker.exe passwords.txt document.pdf\n";
        std::cerr << "Supported file types: .pdf, .zip\n";
        return -1;
    }

    // Define variables relative to the path
    std::string dictionaryPath = argv[1];
    std::string documentPath = argv[2];

    // Check if dictionary file exists
    if (!std::filesystem::exists(dictionaryPath))
    {
        std::cerr << "Error: Dictionary file '" << dictionaryPath << "' not found.\n";
        return -1;
    }

    // Check if document file exists
    if (!std::filesystem::exists(documentPath))
    {
        std::cerr << "Error: Document file '" << documentPath << "' not found.\n";
        return -1;
    }

    // Parse and validate file extension
    FileExtensionOption extension = ParseExtension(documentPath);
    if (extension == FileExtensionOption::NOTSUPPORTED)
    {
        std::string fileExtension = std::filesystem::path(documentPath).extension().string();
        std::cerr << "Error: File extension '" << fileExtension << "' is not supported.\n";
        std::cerr << "Supported extensions: .pdf, .zip\n";
        return -1;
    }

    // Load dictionary
    std::unique_ptr<std::vector<std::string>> dictionaryPasswords = loadDictonary(dictionaryPath.c_str());
    if (!dictionaryPasswords || dictionaryPasswords->empty())
    {
        std::cerr << "Error: Could not load dictionary or dictionary is empty.\n";
        return -1;
    }

    std::cout << "Starting password cracking for: " << documentPath << std::endl;
    std::cout << "Using dictionary: " << dictionaryPath << " (" << dictionaryPasswords->size() << " passwords)" << std::endl;

    // Process based on file extension
    switch (extension)
    {
        case FileExtensionOption::PDF:
        {
            std::optional<std::string> foundPassword = Pdf::CrackPassword(documentPath, dictionaryPasswords);
            if (!foundPassword)
            {
                std::cout << "Password not found in dictionary." << std::endl;
            }
            else
            {
                std::cout << "Password found: " << foundPassword.value() << std::endl;
            }
        }
        break;
        
        case FileExtensionOption::ZIP:
        {
            std::optional<std::string> foundPassword = Zip::CrackPassword(documentPath, *dictionaryPasswords);
            if (!foundPassword)
            {
                std::cout << "Password not found in dictionary." << std::endl;
            }
            else
            {
                std::cout << "Password found: " << foundPassword.value() << std::endl;
            }
        }
        break;
        
        default:
            std::cerr << "Error: Unsupported file extension encountered." << std::endl;
            return -1;
    }

    return 0;
}
