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

struct Tag {
  int id;
  std::string name;
};

int initDB();
int updateFiles();
int createTag(std::string tagName);

std::vector<File> GetFiles();
std::vector<Tag> GetTags();
} // namespace logic

#endif
