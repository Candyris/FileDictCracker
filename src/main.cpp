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
    if (argc != 3)
    {
        std::cerr << "Invaild Argument: PDFCracker.exe (password dictionary) (document.pdf)" << std::endl;
        return -1;
    }

    //  defining the variable relative to the path
     std::string dictonaryPath = argv[1];
     std::string documentPath = argv[2];
    
     /* std::string dictonaryPath = "External/pass_ch8.txt.txt";
      std::string documentPath = "External/Hello.zip";
     */
    FileExtensionOption extension = ParseExtension(documentPath);
    if (extension == FileExtensionOption::NOTSUPPORTED)
    {
        std::cout << "Extension is not supported!" << std::endl;
        return -1;
    }

    std::unique_ptr<std::vector<std::string>> dictionaryPasswords = loadDictonary(dictonaryPath.c_str());
    

    // later decide whether to put the attempt value;
    int attempt = 0;
    switch (extension)
    {
        case FileExtensionOption::PDF:
        {
            std::optional<std::string> foundPassword = Pdf::CrackPassword(documentPath,dictionaryPasswords);
            if (!foundPassword)
            {
                std::cout << "Password not found." << std::endl;
            }
            else
            {
                std::cout << "Password found. "<< foundPassword.value() << std::endl;

            }
        }
        break;
        
        case FileExtensionOption::ZIP:
        {
            std::optional<std::string> foundPassword = Zip::CrackPassword(documentPath,*dictionaryPasswords);
            if (!foundPassword)
            {
                std::cout << "Password not found." << std::endl;
            }
            else
            {
                std::cout << "Password found: "<< foundPassword.value() << std::endl;

            }
        }
        break;

        default:
            std::cout << "Extension is not supported!" << std::endl;
    }
    return 0;
}
