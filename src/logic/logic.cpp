#include <filesystem>
#include <iostream>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <vector>
namespace fs = std::filesystem;

namespace logic {

std::string dirPath;
sqlite3 *db;
int rc; // Used for Database Operation Result

std::vector<std::wstring> GetFiles() {
  std::vector<std::wstring> files;

  for (const fs::directory_entry &entry : fs::directory_iterator(dirPath))
    files.push_back(entry.path().filename().wstring());

  return files;
}

int initDB() {

  // -- Creating the Sqlite Database File --
  //
  const std::string dbName = ".tagger.db";
  const std::string dbPath = dirPath + "/" + dbName;

  // Create or open the sqlite database file
  rc = sqlite3_open(dbPath.c_str(), &db);
  if (rc) {
    std::cerr << "Can't open database: " + std::string(sqlite3_errmsg(db))
              << std::endl;
    sqlite3_close(db);
    return (1);
  }

  // -- Creating Database Tables --
  //
  std::string query_tableCreate = R"(
        CREATE TABLE IF NOT EXISTS file (
            id INT PRIMARY KEY,
            name varchar(255) UNIQUE NOT NULL
        );
        CREATE TABLE IF NOT EXISTS tag (
            id INT PRIMARY KEY,
            name varchar(255) UNIQUE NOT NULL
        );
        CREATE TABLE IF NOT EXISTS file_tag (
            file_id INT,
            tag_id INT,
            FOREIGN KEY(file_id) REFERENCES file(id),
            FOREIGN KEY(tag_id) REFERENCES tag(id),
            UNIQUE(file_id, tag_id)
        );
    )";

  // Create tables
  char *zErrMsg = 0;
  rc = sqlite3_exec(db, query_tableCreate.c_str(), NULL, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " + std::string(zErrMsg) << std::endl;
    sqlite3_free(zErrMsg);
    sqlite3_close(db);
    return 1;
  }

  return 0;
}

} // namespace logic
