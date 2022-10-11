#define _GNU_SOURCE

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include <set>
#include <algorithm>
#include <functional>


#include "mainwindow.h"
#include "aes.h"

using namespace std;

int N_keywords = 0; //Number of keywords
int N_max_ids = 0; // Maximum number of ids for a kw -- or maximum frequency of a kw

int N_row_ids = N_max_ids;

string widxdb_file = "../databases/db6k.csv";//Raw enron database
string eidxdb_file = "eidxdb.csv";//Encrypted meta-keyword database
string bloomfilter_file = "bloom_filter.dat";//Bloom filter file

sw::redis::ConnectionOptions connection_options;
sw::redis::ConnectionPoolOptions pool_options;

mpz_class Prime{"7237005577332262213973186563042994240857116359379907606001950938285454250989",10};//Curve25519 curve order
mpz_class InvExp{"7237005577332262213973186563042994240857116359379907606001950938285454250987",10};

unsigned char ecc_basep[32] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09};

unsigned char **BF;

unsigned char *UIDX;

unsigned char *GL_AES_PT;
unsigned char *GL_AES_KT;
unsigned char *GL_AES_CT;

unsigned char *GL_ECC_INVA;
unsigned char *GL_ECC_INVP;

unsigned char *GL_ECC_SCA;
unsigned char *GL_ECC_BP;
unsigned char *GL_ECC_SMP;

unsigned char *GL_ECC_INA;
unsigned char *GL_ECC_INB;
unsigned char *GL_ECC_PRD;

unsigned char *GL_HASH_MSG;
unsigned char *GL_HASH_DGST;

unsigned char *GL_BLM_MSG;
unsigned char *GL_BLM_DGST;

unsigned char *GL_MGDB_RES;
unsigned char *GL_MGDB_BIDX;
unsigned char *GL_MGDB_JIDX;
unsigned char *GL_MGDB_LBL;

unsigned int *GL_OPCODE;

unsigned int N_threads = 16;

std::mutex mrun;
std::condition_variable dataReady;
std::condition_variable workComplete;

bool ready = false;
bool processed = false;

size_t nWorkerCount = N_threads; //Number of threads
int nCurrentIteration = 0;

std::vector<thread> thread_pool;

int sym_block_size = 0;
int ecc_block_size = 0;
int hash_block_size = 0;
int bhash_block_size = 0;
int bhash_in_block_size = 0;
int N_HASH = 0;
int MAX_BF_BIN_SIZE = 0;
int N_BF_BITS = 0;

//Keeping all same for the debugging and experimental purpose
unsigned char KS[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
unsigned char KI[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
unsigned char KZ[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
unsigned char KX[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
unsigned char KT[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};

int ReadConf(std::string db_conf_filename)
{
    std::ifstream input_file(db_conf_filename);
    std::string line;

    if (input_file.good()) {
        line.clear();
        getline(input_file,line);
        widxdb_file = line;
        
        line.clear();
        getline(input_file,line);
        N_threads = std::stoi(line);
        
        line.clear();
        getline(input_file,line); 
        N_keywords = std::stoi(line);

        line.clear();
        getline(input_file,line);
        N_max_ids = std::stoi(line);

        line.clear();
        getline(input_file,line);
        MAX_BF_BIN_SIZE = std::stoi(line);

        line.clear();
        getline(input_file,line);
        N_BF_BITS = std::stoi(line);

        nWorkerCount = N_threads;
        N_row_ids = N_max_ids;

        sym_block_size = N_threads * 16;
        ecc_block_size = N_threads * 32;
        hash_block_size = N_threads * 64;
        bhash_block_size = N_threads * 64;
        bhash_in_block_size = N_threads * 40;

        N_HASH = N_threads;
    }
    else{
        std::cout << "Error reading database configuration file" << std::endl;
        return -1;
    }

    input_file.close();


    std::cout << "File path: " << widxdb_file << std::endl;
    std::cout << "N_threads: " << N_threads << std::endl;
    std::cout << "Number of keywords: " << N_keywords << std::endl;
    std::cout << "Maximum number of ids: " << N_max_ids << std::endl;
    std::cout << "Bloom filter size: " << MAX_BF_BIN_SIZE << std::endl;
    std::cout << "Bloom filter address bits: " << N_BF_BITS << std::endl;

    return 0;
}

int main()
{
    cout << "Starting program..." << endl;

    ReadConf("../configuration/db6k.conf");

    UIDX = new unsigned char[16*N_max_ids];
    ::memset(UIDX,0x00,16*N_max_ids);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::map<std::string, unsigned int> kw_frequency;
    std::vector<std::string> keyword_vec;
    std::vector<std::string> query;

    unsigned int n_keywords = 0;
    unsigned int n_iterations = 1000;//Number of text vectors to search

    //----------------------------------------------------------------------------------------------
    std::string kw_freq_file = "db_kw_freq.csv";
    std::string res_query_file = "./results/res_query.csv";
    std::string res_id_file = "./results/res_id.csv";
    std::string res_time_file = "./results/res_time.csv";

    std::ifstream kw_freq_file_handle(kw_freq_file);

    std::ofstream res_query_file_handle(res_query_file);
    std::ofstream res_id_file_handle(res_id_file);
    std::ofstream res_time_file_handle(res_time_file);

    //----------------------------------------------------------------------------------------------
    std::vector<std::string> raw_row_data;
    std::string widxdb_row;

    std::stringstream ss;
    std::string kw;
    std::string kw_freq_str;

    //----------------------------------------------------------------------------------------------

    // kw_freq_file_handle.open(kw_freq_file,std::ios_base::in);
    widxdb_row.clear();
    raw_row_data.clear();

    while(getline(kw_freq_file_handle,widxdb_row)){
        raw_row_data.push_back(widxdb_row);
        widxdb_row.clear();
        n_keywords++;
    }

    kw_freq_file_handle.close();

    for(auto v: raw_row_data){
        ss.clear();
        ss << v;
        
        kw.clear();
        kw_freq_str.clear();

        std::getline(ss,kw,',');
        std::getline(ss,kw_freq_str,',');
        
        kw_frequency[kw] = std::stoi(kw_freq_str);
        keyword_vec.push_back(kw);
    }

    //----------------------------------------------------------------------------------------------

    srand(time(NULL));

    //----------------------------------------------------------------------------------------------
    //Initialise
    Sys_Init();
    
    std::cout << "Reading Bloom Filter from disk..." << std::endl;
    BloomFilter_ReadBFfromFile(bloomfilter_file, BF); //Load bloom filter from file
    //----------------------------------------------------------------------------------------------
    // Search
    
    auto search_start_time = std::chrono::high_resolution_clock::now();
    auto search_stop_time = std::chrono::high_resolution_clock::now();
    auto search_time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(search_stop_time - search_start_time).count();

    unsigned int kw_idx = 0;
    unsigned int kw_freq = 0;
    unsigned int n_q_kw = 2;//Number of keywords in a query to search for

    std::vector<unsigned int> idx_vec;
    std::map<unsigned int,unsigned int> freq_map;
    std::vector<unsigned int> idx_sorted;
    std::vector<unsigned int> freq_sorted;
    std::vector<std::string> kw_sorted;

    unsigned int nm = 0;
    unsigned char row_vec[2048]; //16 bytes * Number of keywords in the query
    int n_vec = 0;
    std::set<std::string> result_temp;

    for(unsigned int q_idx=0;q_idx<n_iterations;++q_idx){
        query.clear();
        freq_map.clear();

        for(unsigned int i=0;i<n_q_kw;++i){
            kw_idx = rand() % n_keywords;
            kw_freq = kw_frequency[keyword_vec[kw_idx]];
            freq_map[kw_freq] = kw_idx;
        }

        for(auto v:freq_map){
            query.push_back(keyword_vec[v.second]);
        }

        if(query.size() < n_q_kw) continue;

        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Searching for  ";
        
        for(auto v:query){
            std::cout << v << " ";
        }

        std::cout << " with frequency ";

        for(auto v:query){
            std::cout << kw_frequency[v] << " ";
        }

        ::memset(row_vec,0x00,2048);
        n_vec = 0;

        for(auto rs:query){
            StrToHexBVec(row_vec+(16*n_vec),rs.data());//Defined in mainwindow.cpp file
            n_vec++;
        } 

        std::cout << n_vec << std::endl;
        
        ::memset(UIDX,0x00,16*N_max_ids);
        result_temp.clear();

        //-------------------------------------------------------------------------------
        search_start_time = std::chrono::high_resolution_clock::now();

        nm = EDB_Search(row_vec,(n_vec-1));

        search_stop_time = std::chrono::high_resolution_clock::now();
        search_time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(search_stop_time - search_start_time).count();

        std::cout << "Search done!" << std::endl;

        for(unsigned int k=0;k<nm;++k){
            result_temp.insert(DB_HexToStr_N(UIDX+(16*k),16));
        }
        //-----------------------------------------------------------------------------

        for(auto v:result_temp){
            res_id_file_handle << v.substr(0,8) << ",";
        }
        res_id_file_handle << std::endl;

        for(auto v:query){
            res_query_file_handle << v << ",";
        }

        for(auto v:query){
            res_query_file_handle << kw_frequency[v] << ",";
        }

        res_query_file_handle << result_temp.size() << "," << std::endl;
        res_time_file_handle << search_time_elapsed << "," << std::endl;

        result_temp.clear();
        query.clear();
    }

    res_query_file_handle.close();
    res_id_file_handle.close();
    res_time_file_handle.close();

    //----------------------------------------------------------------------------------------------
    // Thread Release
    Sys_Clear();
    delete [] UIDX;
    //----------------------------------------------------------------------------------------------

    return 0;

    cout << "Program finished!" << endl;

    return 0;
}
