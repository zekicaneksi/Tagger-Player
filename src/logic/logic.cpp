#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

std::vector<std::string> GetFiles(){
    std::vector<std::string> files;
    std::string path = "./";

    for (const fs::directory_entry & entry : fs::directory_iterator(path))
        files.push_back(entry.path());

    return files;
}
