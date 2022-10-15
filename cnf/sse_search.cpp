#define _GNU_SOURCE

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include <set>
#include <unordered_set>
#include <algorithm>
#include <functional>


#include "mainwindow.h"
#include "aes.h"

using namespace std;

int N_keywords = 0;//Number of meta-keywords
int N_max_ids = 0;//Number of maximum ids for a meta-keyword

int N_row_ids = N_max_ids;

string widxdb_file = "../databases/meta_db6k.dat";//path to meta-keyword database
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

int ReadConfAll(std::string db_conf_filename)
{
    std::ifstream input_file(db_conf_filename);
    std::string line;

    if (input_file.good()) {
        //Line 1 -- plain database file name
        line.clear();
        getline(input_file,line);
        widxdb_file = line;
        
        //Line 2 -- number of cores
        line.clear();
        getline(input_file,line);
        N_threads = std::stoi(line);
        
        //Line 3 -- number of keywords in the plain database
        line.clear();
        getline(input_file,line); 
        N_keywords = std::stoi(line);

        //Line 4 -- maximum number of docuement identifiers for a keyword in the plain database
        line.clear();
        getline(input_file,line);
        N_max_ids = std::stoi(line);

        //Line 5 -- Plain DB Bloom filter size -- typically a power of 2, next to the total number keyword-id pairs
        line.clear();
        getline(input_file,line);
        MAX_BF_BIN_SIZE = std::stoi(line);

        //Line 6 -- Number of bits to address plain DB Bloom filter -- typically the n in 2^n of BF size
        line.clear();
        getline(input_file,line);
        N_BF_BITS = std::stoi(line);

        ///////////////////////////////////////////////////////////////////////
        //Needs to read meta-db parameters
        //Existing values will get overwritten! I know not a good way to do this, but fine for now.

        //Line 7 -- meta database file name
        line.clear();
        getline(input_file,line);
        widxdb_file = line;
        
        //Line 8 -- number of cores to use for metadb
        line.clear();
        getline(input_file,line);
        N_threads = std::stoi(line);
        
        //Line 9 -- number of metakeywords in the meta database
        line.clear();
        getline(input_file,line); 
        N_keywords = std::stoi(line);

        //Line 10 -- maximum number of docuement identifiers for a metakeyword in the plain database
        line.clear();
        getline(input_file,line);
        N_max_ids = std::stoi(line);

        //Line 11 -- meta DB Bloom filter size -- typically a power of 2, next to the total number metakeyword-id pairs
        line.clear();
        getline(input_file,line);
        MAX_BF_BIN_SIZE = std::stoi(line);

        //Line 12 -- Number of bits to address meta DB Bloom filter -- typically the n in 2^n of meta BF size
        line.clear();
        getline(input_file,line);
        N_BF_BITS = std::stoi(line);

        ///////////////////////////////////////////////////////////////////////

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

    ReadConfAll("../configuration/db6k.conf");

    UIDX = new unsigned char[16*N_max_ids];
    ::memset(UIDX,0x00,16*N_max_ids);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::map<std::string, unsigned int> kw_frequency;
    std::vector<std::string> keyword_vec;
    std::vector<std::string> query;

    unsigned int n_keywords = 0;
    unsigned int n_testvectors = 0;
    unsigned int n_iterations = 50;//not used here, depends upon provived test vectors
    unsigned int NUM_CLAUSE = 2; // Number of disjunctive clauses per CNF expression

    std::vector<std::string> raw_qv_data;
    std::vector<std::string> raw_tv_data;
    std::vector<std::string> raw_fq_data;
    std::vector<std::string> raw_bv_data;

    //----------------------------------------------------------------------------------------------
    
    std::string queryvector_file = "./test_vectors/HW2_query/HW2_hex_queryvector.csv";
    std::string testvector_file = "./test_vectors/HW2_query/HW2_testvector.csv";
    std::string freqvector_file = "./test_vectors/HW2_query/HW2_freqvector.csv";
    std::string binvector_file = "./test_vectors/HW2_query/HW2_binvector.csv";

    std::string res_freq_file = "./results/res_freq.csv";
    std::string res_id_file = "./results/res_id.csv";
    std::string res_time_file = "./results/res_time.csv";

    std::ifstream qv_file(queryvector_file);
    std::ifstream tv_file(testvector_file);
    std::ifstream fq_file(freqvector_file);
    std::ifstream bv_file(binvector_file);

    std::ofstream res_freq_file_handle(res_freq_file);
    std::ofstream res_id_file_handle(res_id_file);
    std::ofstream res_time_file_handle(res_time_file);

    //----------------------------------------------------------------------------------------------
    std::vector<std::string> raw_row_data;
    std::string fline;

    std::stringstream ss;
    std::string s;
    std::string kw;
    std::string kw_freq_str;

    //----------------------------------------------------------------------------------------------

    n_testvectors = 0;
    while(getline(tv_file,fline)){
        raw_tv_data.push_back(fline);
        fline.clear();
        n_testvectors++;
    }

    while(getline(fq_file,fline)){
        raw_fq_data.push_back(fline);
        fline.clear();
    }

    while(getline(bv_file,fline)){
        raw_bv_data.push_back(fline);
        fline.clear();
    }

    while(getline(qv_file,fline)){
        raw_qv_data.push_back(fline);
        fline.clear();
    }

    qv_file.close();
    tv_file.close();
    fq_file.close();
    bv_file.close();

    //----------------------------------------------------------------------------------------------

    vector<std::string> row_tv_data;
    vector<unsigned int> row_fq_data;
    vector<unsigned int> row_bv_data;
    vector<unsigned int> row_qv_data;

    string q_mkws;
    unsigned int bin_idx;
    unsigned int mkw_freq;

    vector<std::string> mkws;
    int search_count = 0;
    int nm = 0;
    int result_set_size = 0;

    unsigned char row_vec[2048]; //meta-query
    int n_vec = 0;

    //----------------------------------------------------------------------------------------------

    std::set<std::string> result_union;
    std::set<std::string> result_union_temp;
    std::unordered_set<std::string> result_final;

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
    auto search_total_time = chrono::duration_cast<chrono::microseconds>(search_stop_time - search_start_time).count();

    //----------------------------------------------------------------------------------------------


    for(unsigned int i=0;i<(n_testvectors/2-1);++i){

        std::cout << "----------------" << endl;
        std::cout << "Test Vector " << i << endl;

        search_total_time = 0;
        result_set_size = 0;

        for(unsigned int n_clause = 0; n_clause < NUM_CLAUSE; ++n_clause){
            //=============================================================================================

            result_union.clear();
            mkws.clear();

            ss.clear();
            ss << raw_tv_data.at(2*i+n_clause);//Get the current testvector row
            // std::getline(ss,s,',');// Skip the 0000, if present in meta_db
            row_tv_data.clear();
            while(std::getline(ss,s,',') && !ss.eof()) {
                if(!s.empty()){
                    row_tv_data.push_back(s);
                }
            }

            ss.clear();
            ss << raw_bv_data.at(2*i+n_clause);//Get the current bin index row
            row_bv_data.clear();
            while(std::getline(ss,s,',') && !ss.eof()) {
                if(!s.empty()){
                    row_bv_data.push_back(std::stoi(s));
                }
            }

            ss.clear();
            ss << raw_qv_data.at(2*i+n_clause);//Get the current bin index row
            row_qv_data.clear();
            while(std::getline(ss,s,',') && !ss.eof()) {
                if(!s.empty()){
                    row_qv_data.push_back(std::stoi(s));
                }
            }

            std::cout << "Querying keywords at index ";
            for(auto v:row_qv_data){
                std::cout << v << ", ";
            }
            std::cout << std::endl;

            for(unsigned int j=0;j<row_bv_data.size();++j){
                if(j<row_bv_data.size()-1){
                    if(row_bv_data.at(j) == row_bv_data.at(j+1)){//Push the current mkw to vector if the next mkw is from the same bin
                        mkws.push_back(row_tv_data.at(j));
                    }
                    else{//Search for previous mkws when an mkw from different bin is encounterd
                        mkws.push_back(row_tv_data.at(j));

                        ::memset(row_vec,0x00,2048);
                        n_vec = 0;

                        for(auto rs:mkws){
                            StrToHexBVec(row_vec+(16*n_vec),rs.data());//Defined in mainwindow.cpp file
                            n_vec++;
                        }

                        cout << "1 Querying: ";

                        for(auto v:mkws){
                            cout << v.substr(0,8) << ", ";
                        }
                        cout << endl;
                        
                        //-----------------------------------
                        ::memset(UIDX,0x00,16*N_max_ids);
                        result_union_temp.clear();
                        
                        search_start_time = chrono::high_resolution_clock::now();
                        nm = EDB_Search(row_vec,n_vec-1);//minus 1
                        search_stop_time = chrono::high_resolution_clock::now();
                        search_time_elapsed = chrono::duration_cast<chrono::microseconds>(search_stop_time - search_start_time).count();
                        search_total_time += search_time_elapsed;
                        cout << "Time elapsed: " << search_time_elapsed << endl;

                        result_set_size += nm;

                        for(unsigned int k=0;k<nm;++k){
                            result_union_temp.insert(DB_HexToStr_N(UIDX+(16*k),16));
                        }
                        //-----------------------------------

                        mkws.clear();
                    }
                }
                else{//Need to handle separately when last mkw is same with the preceeding ones
                    if(row_bv_data.at(j-1) == row_bv_data.at(j)){
                        // mkws.push_back(row_tv_data.at(j-1));
                        mkws.push_back(row_tv_data.at(j));

                        ::memset(row_vec,0x00,2048);
                        n_vec = 0;

                        for(auto rs:mkws){
                            StrToHexBVec(row_vec+(16*n_vec),rs.data());//Defined in mainwindow.cpp file
                            n_vec++;
                        }

                        cout << "2 Querying: ";
                        for(auto v:mkws){
                            cout << v.substr(0,8) << ", ";
                        }
                        cout << endl;
                        
                        //-----------------------------------
                        ::memset(UIDX,0x00,16*N_max_ids);
                        result_union_temp.clear();
                        
                        search_start_time = chrono::high_resolution_clock::now();
                        nm = EDB_Search(row_vec,n_vec-1);//minus 1
                        search_stop_time = chrono::high_resolution_clock::now();
                        search_time_elapsed = chrono::duration_cast<chrono::microseconds>(search_stop_time - search_start_time).count();
                        search_total_time += search_time_elapsed;
                        cout << "Time elapsed: " << search_time_elapsed << " us" << endl;

                        result_set_size += nm;

                        for(unsigned int k=0;k<nm;++k){
                            result_union_temp.insert(DB_HexToStr_N(UIDX+(16*k),16));
                        }

                        //-----------------------------------

                        mkws.clear();
                    }
                    else{//This is when only the last mkw is from a different bin
                        cout << "3 Querying: ";
                        cout << row_tv_data.at(j).substr(0,8) << endl;

                        for(auto rs:mkws){
                            StrToHexBVec(row_vec+(16*n_vec),rs.data());
                            n_vec++;
                        }

                        //-----------------------------------
                        ::memset(UIDX,0x00,16*N_max_ids);
                        result_union_temp.clear();

                        search_start_time = chrono::high_resolution_clock::now();
                        nm = EDB_Search(row_vec,n_vec-1);//minus 1
                        search_stop_time = chrono::high_resolution_clock::now();
                        search_time_elapsed = chrono::duration_cast<chrono::microseconds>(search_stop_time - search_start_time).count();
                        search_total_time += search_time_elapsed;
                        cout << "Time elapsed: " << search_time_elapsed << " us" << endl;

                        result_set_size += nm;

                        for(unsigned int k=0;k<nm;++k){
                            result_union_temp.insert(DB_HexToStr_N(UIDX+(16*k),16));
                        }
                        //-----------------------------------

                        mkws.clear();
                    }
                }
                // result union for each bin
                result_union.insert(result_union_temp.begin(),result_union_temp.end());
                result_union_temp.clear();
            }

            result_union_temp.clear();

            //Insert results into final set, take intersection
            set_intersection(result_union.begin(), result_union.end(), result_final.begin(), result_final.end(), std::inserter(result_union_temp, result_union_temp.begin()));
            // result_final.insert(result_union.begin(),result_union.end());

            result_final.clear();
            result_final.insert(result_union_temp.begin(),result_union_temp.end());

            result_union.clear();
            result_union_temp.clear();

            //=============================================================================================

        }

        //Write results to file
        for(auto v:result_final){
            res_id_file_handle << v << ",";
        }
        res_id_file_handle << endl;

        cout << "Result size: " << result_final.size() << endl;
        cout << "Total time elapsed: " << search_total_time << endl;
        res_time_file_handle << search_total_time << "," << endl;

        res_freq_file_handle << result_final.size() << "," << endl;

        result_final.clear();
    }

    res_freq_file_handle.close();
    res_id_file_handle.close();
    res_time_file_handle.close();

    //----------------------------------------------------------------------------------------------
    // Thread Release
    Sys_Clear();
    delete [] UIDX;
    //----------------------------------------------------------------------------------------------

    cout << "Program finished!" << endl;

    return 0;
}
