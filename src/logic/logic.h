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
  std::vector<int> tag_ids;
};

struct Tag {
  int id;
  std::string name;
};

int initDB();
int updateFiles();

std::vector<File> GetFiles();
std::vector<Tag> GetTags();

int createTag(std::string tagName);
int attachTag(int fileId, int tagId);
int detachTag(int fileId, int tagId);
int deleteTag(int tagId);
} // namespace logic

#endif
