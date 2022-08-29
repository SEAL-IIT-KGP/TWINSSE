#include "utils.h"

std::string hexStr(::byte *data, int len);

int convert_mkws_digest(std::vector<std::string> &db_mkw,
                        vector<string>& db_mkw_hash );

int DB_WriteFromDatabaseToFile(std::string db_file,
                               std::vector<std::string> db_kw,
                               std::vector<std::set<unsigned int>> db,
                               KW_TYPE dtype);

int DB_ReadFromFileToDatabase(std::string db_file,
                              std::vector<std::string> &db_kw,
                              std::vector<std::set<unsigned int>> &db);

int DB_GenMKW(std::vector<std::set<unsigned int>> &meta_db,
              std::vector<std::string> &db_mkw,
              std::vector<unsigned int> &bin_boundaries,
              std::vector<std::set<unsigned int>> raw_db,
              unsigned int bin_stride, std::vector<std::string> &db_mkw_hash,
              std::vector<unsigned int> &frequency);