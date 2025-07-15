#pragma once
enum FileExtensionOption
{
    NOTSUPPORTED = 0, PDF, ZIP
};

FileExtensionOption ParseExtension(const std::string& path)
{
    if (std::string::npos != path.rfind(".pdf"))
    {
        return FileExtensionOption::PDF;
    }
    else if (std::string::npos != path.rfind(".zip"))
    {
        return FileExtensionOption::ZIP;
    }
    return FileExtensionOption::NOTSUPPORTED;
}