#pragma once 
#include <optional>
#include <string>
#include <memory>
#include <vector>
namespace Zip
{
    bool PasswordCheck(const char* path, const char* password);
    std::optional<std::string> CrackPassword(const std::string& path, const std::vector<std::string>& dictionary);

}