#include "gen_mkw.h"

  
int resultact(string query,std::vector<std::set<unsigned int>> &raw_db,vector<unsigned int> &ra);
   
vector <unsigned int> getIntersection(vector < vector <unsigned int> > &sets);
 
vector<string> DB_find_MKW(string q,int n, int prev, int bin_stride);

// vector <unsigned int> set_of_s_intersection(vector<std::string>& mkw_bin,vector<std::string> & db_mkw,vector<std::set<unsigned int>> &meta_db,std:: vector<unsigned int> &frequency);
vector <unsigned int> set_of_s_intersection(vector<std::string>& mkw_bin,vector<std::string> & db_mkw,vector<std::set<unsigned int>> &meta_db,std:: vector<unsigned int> &frequency, std::vector<unsigned int> &freq_vec);

// int DB_ConvertQueryToMetaKeyword(std::vector<vector<std::string>> &mkws_hash, std::vector<unsigned int> &bin_boundaries,std::string query, int bin_stride, vector<unsigned int> &bin,std::vector<unsigned int>& s_vec, vector<std::string> & db_mkw_hash,vector<std::set<unsigned int>> &meta_db,std::vector<unsigned int> &frequency);
int DB_ConvertQueryToMetaKeyword(std::vector<vector<std::string>> &mkws_hash, std::vector<unsigned int> &bin_boundaries,std::string query, int bin_stride, vector<vector<unsigned int>> &bin,std::vector<unsigned int>& s_vec, vector<std::string> & db_mkw_hash,vector<std::set<unsigned int>> &meta_db,std::vector<unsigned int> &frequency, std::vector<unsigned int> &freq_vec);

int DB_WritetoHamFile(std::string ham_file, std::vector<std::set<string>> ham);

string gen(int ham,int  bits);

float average(std::vector<float> & v);

double precision(std::vector<unsigned int> ra, std::vector<unsigned int> rs);

std::string string_to_hex(const std::string& input);
