#include "logic.h"
#include <filesystem>
#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
namespace fs = std::filesystem;

namespace logic {

// -- Utility Functions --

// Generates Question marks to be used with queries.
enum questionMarkType {
  normal,         // ?,?,?
  withParenthesis // (?),(?),(?)
};

std::string generateQuestionMarks(int amount, questionMarkType type) {

  std::string questionMarkToInsert;
  switch (type) {
  case 0:
    questionMarkToInsert = "?,";
    break;
  case 1:
    questionMarkToInsert = "(?),";
    break;
  }

  std::ostringstream stream_parameterQuestionMarks;
  for (int i = 0; i < amount; i++) {
    stream_parameterQuestionMarks << questionMarkToInsert;
  }
  std::string parameterQuestionMarks = stream_parameterQuestionMarks.str();
  parameterQuestionMarks.pop_back();

  return parameterQuestionMarks;
}

// -- Utility Functions End --

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
            id INTEGER PRIMARY KEY NOT NULL,
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

  // -- Insert all files in directory into database --
  //
  // Because of query limit restirictions, the records are inserted in bulks
  const int bulkSizeInsert = 25;
  for (int i = 0; i < files.size(); i += bulkSizeInsert) {
    // The query basically is insert if not exists.
    const std::string sqlInsertQuery =
        std::string(R"(
WITH new_files (name) AS (VALUES )") +
        generateQuestionMarks(files.size() - i >= bulkSizeInsert
                                  ? bulkSizeInsert
                                  : files.size() - i,
                              questionMarkType::withParenthesis) +
        std::string(R"()
INSERT INTO file (name)
SELECT new_files.name FROM new_files
LEFT OUTER JOIN file existing_files
ON new_files.name = existing_files.name
WHERE existing_files.name IS NULL;
)");

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sqlInsertQuery.c_str(), sqlInsertQuery.length(),
                       &stmt, nullptr);
    for (int y = 0; (y < files.size() - i && y < bulkSizeInsert); y++) {
      sqlite3_bind_text(stmt, y + 1, files[i + y].c_str(),
                        files[i + y].length(), SQLITE_STATIC);
    }

    int resultCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (resultCode != 0 && resultCode != 101) {
      std::cerr << "Insert into db failed, last result code: " << resultCode
                << std::endl;
      return 1;
    }
  }

  // -- Remove the files from database that don't exist in the directory anymore
  // --
  //
  std::vector<File> filesInDB = GetFiles();

  // Remove elements from "filesInDB" that also exist in "files"
  for (int i = 0; i < files.size(); i++) {
    for (int y = 0; y < filesInDB.size(); y++) {
      if (files[i] == filesInDB[y].name) {
        filesInDB.erase(filesInDB.begin() + y);
        break;
      }
    }
  }

  // Because of query limit restirictions, the records are deleted in bulks
  const int bulkSizeDelete = 50;
  for (int i = 0; i < filesInDB.size(); i += bulkSizeDelete) {

    const std::string sqlDeleteQuery =
        "DELETE FROM file WHERE id IN (" +
        generateQuestionMarks(filesInDB.size() - i >= bulkSizeDelete
                                  ? bulkSizeDelete
                                  : filesInDB.size() - i,
                              questionMarkType::normal) +
        ");";

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sqlDeleteQuery.c_str(), sqlDeleteQuery.length(),
                       &stmt, nullptr);
    for (int y = 0; (y < filesInDB.size() - i && y < bulkSizeDelete); y++) {
      sqlite3_bind_int(stmt, y + 1, filesInDB[i + y].id);
    }

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != 101) {
      std::cerr
          << "Removing unexistent files from db failed, last result code: "
          << rc << std::endl;
      return 1;
    }
  }

  return 0;
}

int createTag(std::string tagName) {
  const std::string sqlInsertQuery = "INSERT INTO tag (name) VALUES (?);";

  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(db, sqlInsertQuery.c_str(), sqlInsertQuery.length(), &stmt,
                     nullptr);
  sqlite3_bind_text(stmt, 1, tagName.c_str(), tagName.length(), SQLITE_STATIC);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != 101) {
    std::cerr << "Inserting tag into db failed, last result code: " << rc
              << std::endl;
    return 1;
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

int GetTagsCallback(void *Used, int argc, char **argv, char **azColName) {

  std::vector<Tag> *tagArr = (std::vector<Tag> *)Used;

  Tag tagToPush = {
      std::stoi(argv[0]),
      argv[1],
  };

  tagArr->push_back(tagToPush);

  return 0;
}

std::vector<Tag> GetTags() {
  // Used for Database Operation Result
  int rc;
  char *zErrMsg = 0;

  std::vector<Tag> tags;

  // Get tags
  const std::string query_getTags = "SELECT * FROM tag";
  rc =
      sqlite3_exec(db, query_getTags.c_str(), GetTagsCallback, &tags, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " + std::string(zErrMsg) << std::endl;
    sqlite3_free(zErrMsg);
    throw "cannot get tags from database";
  }

  return tags;
}

} // namespace logic
