#include "Pdf.h" // Assuming this is needed for your Pdf class
#include <poppler/cpp/poppler-document.h> // Assuming this is needed
#include <memory>
#include <cstdio>
#include <io.h>     // _dup, _dup2, _close
#include <fcntl.h>  // _fileno, _O_WRONLY, _O_CREAT
#include <iostream>
#include <mutex> // Keep if it's used elsewhere, otherwise it can be removed if not needed.
// It was in your original includes, so keeping it for context.
#include <memory> 
class OutputSuppressor {
private:
    int oldStdout;
    int oldStderr;
    FILE* nullOut;

public:
    OutputSuppressor() {
        fflush(stdout);
        fflush(stderr);
        oldStdout = _dup(_fileno(stdout));
        oldStderr = _dup(_fileno(stderr));
        nullOut = fopen("nul", "w");
        if (nullOut) {
            _dup2(_fileno(nullOut), _fileno(stdout));
            _dup2(_fileno(nullOut), _fileno(stderr));
        }
    }

    ~OutputSuppressor() {
        fflush(stdout);
        fflush(stderr);
        if (oldStdout != -1) _dup2(oldStdout, _fileno(stdout));
        if (oldStderr != -1) _dup2(oldStderr, _fileno(stderr));
        if (nullOut) fclose(nullOut);
    }
};


bool Pdf::PasswordCheck(const char* path, const char* password)
{

    poppler::document* raw_doc = poppler::document::load_from_file(path, password);

    std::unique_ptr<poppler::document> doc(raw_doc);

    return raw_doc && !raw_doc->is_locked();
}

std::optional<std::string> Pdf::CrackPassword(const std::string& documentPath, std::unique_ptr<std::vector<std::string>>& dictionaryPasswords)
{
    std::cout << "documentPath" << documentPath << std::endl;
    const int numThreads = std::thread::hardware_concurrency();
    const int total = dictionaryPasswords->size();
    const int chunkSize = (total + numThreads - 1) / numThreads;

    std::string correctPassword = std::string();
    std::atomic<bool> foundPassword(false);

    std::vector<std::thread> threads;
    {
        OutputSuppressor o;
        std::mutex coutMutex;
        for (int i = 0; i < numThreads; i++)
        {
            int startIndex = i * chunkSize;
            int end = std::min(startIndex + chunkSize, total);

            threads.emplace_back([startIndex, end, &documentPath, &foundPassword, &dictionaryPasswords, &coutMutex, &correctPassword]()
                {
                    for (size_t j = startIndex; j < end; j++)
                    {
                        if (foundPassword.load()) return;
                        
                        const std::string& password = (*dictionaryPasswords)[j];
                        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
                        std::cout << password << std::endl;
                        if (Pdf::PasswordCheck(documentPath.c_str(), password.c_str()))
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
            for (std::thread& t : threads)
            {
                if (t.joinable())
                    t.join();
            }
            
            return foundPassword.load() ? std::make_optional(correctPassword) : std::nullopt;
    }
}