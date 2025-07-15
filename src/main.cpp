#include <iostream>
#include <string>
#include <memory>
#include "Extension.h"
#include "Pdf/Pdf.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>
#include <mutex>
#include <thread>

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
    /*if (argc != 3)
    {
        std::cerr << "Invaild Argument: PDFCracker.exe (password dictionary) (document.pdf)" << std::endl;
        return -1;
    }*/
    
    // defining the variable relative to the path
    // std::string dictonaryPath = "External/pass_ch8.txt.txt";
    // std::string documentPath = "External/Hello.pdf";
    std::string dictonaryPath = "External/pass_ch8.txt.txt";
    std::string documentPath = "External/Hello.pdf";
    FileExtensionOption extension = ParseExtension(documentPath);
    if (extension == FileExtensionOption::NOTSUPPORTED)
    {
        std::cout << "Extension is not supported!" << std::endl;
        return -1;
    }

    std::unique_ptr<std::vector<std::string>> dictionaryPasswords = loadDictonary(dictonaryPath.c_str());
    const int numThreads = std::thread::hardware_concurrency();
    const int total = dictionaryPasswords->size();
    const int chunkSize = (total + numThreads - 1) / numThreads;


    int attempt = 0;
    switch(extension)
    {
        case FileExtensionOption::PDF:
            {
                std::mutex coutMutex;
                std::atomic<bool> foundPassword(false);
                //std::atomic<std::string> password
                for (int i = 0; i < numThreads; i++)
                {
                    std::vector<std::thread> threads;
                    int startIndex = i * chunkSize;
                    int end = std::min(startIndex + chunkSize, total);
                    threads.emplace_back([startIndex, end, &documentPath, &foundPassword, &dictionaryPasswords, &coutMutex]() {
                        for (size_t i = startIndex; i <= end; i++)
                        {
                            if (foundPassword.load())
                            {
                                return;
                            }
                            const std::string& password = dictionaryPasswords->at(i);
                            if (Pdf::PasswordCheck(documentPath.c_str(), password.c_str()))
                            {
                                std::lock_guard<std::mutex> lock(coutMutex);
                                std::cout << "Thread ID [" << std::this_thread::get_id() << "] => Password Found: " << password << "Attempt: " << i << std::endl;
                                foundPassword = true;
                                return;
                            }
                        }
                    });

                    for (std::thread& t : threads)
                    {
                        if (t.joinable())
                            t.join();
                    }
                    if (!foundPassword.load())
                    {
                        std::cout << "Password not found." << std::endl;
                    }
                }
            }
            break;
        default:
            std::cout << "Extension is not supported!" << std::endl;
    }



    return 0;
}