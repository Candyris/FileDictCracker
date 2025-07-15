#pragma once
namespace Pdf
{
    bool PasswordCheck(const char* path, const char* password);
};
class ScopedStderrSuppressor {
public:
    ScopedStderrSuppressor();

    ~ScopedStderrSuppressor();

private:
    int saved_fd = -1;
};