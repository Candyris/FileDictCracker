#pragma once
#include <string>
#include <optional>
#include <memory>
#include <vector>
namespace Pdf
{
    bool PasswordCheck(const char* path, const char* password);
    std::optional<std::string> CrackPassword(const std::string& documentPath, std::unique_ptr<std::vector<std::string>>& dictionaryPasswords);
};
