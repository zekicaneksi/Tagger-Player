#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <string>
#include <vector>

namespace logic {
extern std::string dirPath;

struct File {
  int id;
  std::string name;
};

int initDB();
int updateFiles();
std::vector<File> GetFiles();
} // namespace logic

#endif
