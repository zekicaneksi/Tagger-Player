#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;


namespace logic
{
// Must be set before using any functions in this file.
std::wstring dirPath;

std::vector<std::wstring> GetFiles(){
    std::vector<std::wstring> files;

    for (const fs::directory_entry & entry : fs::directory_iterator(dirPath))
        files.push_back(entry.path().filename().wstring());

    return files;
}
}
