#include "Zip.h"
#include <zip.h>
#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>

bool Zip::PasswordCheck(const char* path, const char* password)
{
    int error = 0;
    zip_t* archive = zip_open(path, ZIP_RDONLY, &error);
    if (!archive) return false;

    if (zip_set_default_password(archive, password) != 0) {
        zip_close(archive);
        return false;
    }

    zip_stat_t stat;
    if (zip_stat_index(archive, 0, 0, &stat) != 0) {
        zip_close(archive);
        return false;
    }

    // Attempt to open and read the file
    zip_file_t* zf = zip_fopen_index(archive, 0, 0);
    if (!zf) {
        zip_close(archive);
        return false;
    }

    // Try to read a few bytes to confirm password
    char buffer[16];
    zip_int64_t bytesRead = zip_fread(zf, buffer, sizeof(buffer));
    zip_fclose(zf);
    zip_close(archive);

    return bytesRead > 0;
}

std::optional<std::string> Zip::CrackPassword(const std::string& path, const std::vector<std::string>& dictionary)
{
    const int numThreads = std::thread::hardware_concurrency();
    const int total = dictionary.size();
    const int chunkSize = (total + numThreads - 1) / numThreads;

    std::string correctPassword = std::string();
    std::atomic<bool> foundPassword(false);

    std::vector<std::thread> threads;
    {
        std::mutex coutMutex;
        for (int i = 0; i < numThreads; i++)
        {
            int startIndex = i * chunkSize;
            int end = std::min(startIndex + chunkSize, total);

            threads.emplace_back([startIndex, end, &path, &foundPassword, &dictionary, &coutMutex, &correctPassword]()
                {
                    for (size_t j = startIndex; j < end; j++)
                    {
                        if (foundPassword.load()) return;

                        const std::string& password = dictionary[j];
                        //std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
                        //std::cout << password << std::endl;
                        if (Zip::PasswordCheck(path.c_str(), password.c_str()))
                        {
                            {
                                std::lock_guard<std::mutex> lock(coutMutex);
                                correctPassword = password;
                                std::cout << "Thread ID [" << std::this_thread::get_id() << "] => Password Found: " << password << "Attempt: " << j << std::endl;
                            }
                            foundPassword = true;
                            return;
                        }
                    }
                });
        }
    }
    for (std::thread& t : threads)
    {
        if (t.joinable())
            t.join();
    }
   return foundPassword.load() ? std::make_optional(correctPassword) : std::nullopt;
}
