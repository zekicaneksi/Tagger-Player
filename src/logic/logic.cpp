#include "logic.h"
#include <filesystem>
#include <iostream>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <vector>
namespace fs = std::filesystem;

namespace logic {

const std::string dbName = ".tagger.db";

std::string dirPath;

sqlite3 *db;

int initDB() {

  // Used for Database Operation Result
  int rc;
  char *zErrMsg = 0;

  // -- Creating or Opening the Sqlite Database File --
  //
  const std::string dbPath = dirPath + "/" + dbName;

  // Create or open the sqlite database file
  rc = sqlite3_open(dbPath.c_str(), &db);
  if (rc) {
    std::cerr << "Can't open database: " + std::string(sqlite3_errmsg(db))
              << std::endl;
    sqlite3_close(db);
    return (1);
  }

  // -- Creating Database Tables If Not Exists --
  //
  std::string query_tableCreate = R"(
        CREATE TABLE IF NOT EXISTS file (
            id INTEGER PRIMARY KEY NOT NULL,
            name varchar(255) UNIQUE NOT NULL
        );
        CREATE TABLE IF NOT EXISTS tag (
            id INTEGET PRIMARY KEY NOT NULL,
            name varchar(255) UNIQUE NOT NULL
        );
        CREATE TABLE IF NOT EXISTS file_tag (
            file_id INT NOT NULL,
            tag_id INT NOT NULL,
            FOREIGN KEY(file_id) REFERENCES file(id),
            FOREIGN KEY(tag_id) REFERENCES tag(id),
            UNIQUE(file_id, tag_id)
        );
    )";

  // Create tables
  rc = sqlite3_exec(db, query_tableCreate.c_str(), NULL, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " + std::string(zErrMsg) << std::endl;
    sqlite3_free(zErrMsg);
    sqlite3_close(db);
    return 1;
  }

  return 0;
}

int updateFiles() {
  std::vector<std::string> files;

  // Get List of Files
  for (const fs::directory_entry &entry : fs::directory_iterator(dirPath)) {
    const std::string filename = entry.path().filename();
    if (filename == dbName || !entry.is_regular_file())
      continue;
    files.push_back(filename);
  }

  // Insert all files into database
  // Basically, insert if not exists.
  const std::string sqlInsertQuery = R"(
INSERT INTO file (name)
SELECT new_files.name
FROM (SELECT ? AS name) AS new_files
LEFT OUTER JOIN file existing_files
ON new_files.name = existing_files.name
WHERE existing_files.name IS NULL;
        )";
  for (int i = 0; i < files.size(); i++) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sqlInsertQuery.c_str(), sqlInsertQuery.length(),
                       &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, files[i].c_str(), files[i].length(),
                      SQLITE_STATIC);
    int errCode = sqlite3_step(stmt);
    if (errCode != 0 && errCode != 101) {
      std::cerr << "Insert into db failed, error code: " << errCode
                << std::endl;
      return 1;
    }
  }

  return 0;
}

int GetFilesCallback(void *Used, int argc, char **argv, char **azColName) {

  std::vector<File> *fileArr = (std::vector<File> *)Used;

  File fileToPush = {
      std::stoi(argv[0]),
      argv[1],
  };

  fileArr->push_back(fileToPush);

  return 0;
}

std::vector<File> GetFiles() {
  // Used for Database Operation Result
  int rc;
  char *zErrMsg = 0;

  std::vector<File> files;

  // Get files
  const std::string query_getFiles = "SELECT * FROM file";
  rc = sqlite3_exec(db, query_getFiles.c_str(), GetFilesCallback, &files,
                    &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " + std::string(zErrMsg) << std::endl;
    sqlite3_free(zErrMsg);
    throw "cannot get files from database";
  }

  return files;
}

} // namespace logic
