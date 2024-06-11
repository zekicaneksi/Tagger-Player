#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;


namespace logic
{
// Must be set before using any functions in this file.
std::string dirPath;

std::vector<std::string> GetFiles(){
    std::vector<std::string> files;

    for (const fs::directory_entry & entry : fs::directory_iterator(dirPath))
        files.push_back(entry.path());

    return files;
}
}
