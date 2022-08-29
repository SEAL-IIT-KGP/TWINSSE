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

//meta_db6k.dat
int N_keywords = 32621;//Number of meta-keywords
int N_max_ids = 1170;//Number of maximum ids for a meta-keyword

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

unsigned int N_threads = 24;

std::mutex mrun;
std::condition_variable dataReady;
std::condition_variable workComplete;

bool ready = false;
bool processed = false;

size_t nWorkerCount = N_threads; //Number of threads
int nCurrentIteration = 0;

std::vector<thread> thread_pool(N_threads);

int sym_block_size = N_threads * 16;
int ecc_block_size = N_threads * 32;
int hash_block_size = N_threads * 64;
int bhash_block_size = N_threads * 64;
int bhash_in_block_size = N_threads * 40;

//Keeping the keys same for debugging purpose
unsigned char KS[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
unsigned char KI[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
unsigned char KZ[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
unsigned char KX[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
unsigned char KT[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};

int main()
{
    cout << "Starting program..." << endl;

    UIDX = new unsigned char[16*N_max_ids];
    ::memset(UIDX,0x00,16*N_max_ids);

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Setup
    Sys_Init();
    
    EDB_SetUp();

    BloomFilter_WriteBFtoFile(bloomfilter_file, BF); //Store bloom filter in file
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Search
    std::map<std::string, unsigned int> kw_frequency;
    std::vector<std::string> keyword_vec;
    std::vector<std::string> query;

    unsigned int n_keywords = 0;
    unsigned int n_testvectors = 0;
    unsigned int n_iterations = 50;

    std::vector<std::string> raw_tv_data;
    std::vector<std::string> raw_fq_data;
    std::vector<std::string> raw_bv_data;

    //----------------------------------------------------------------------------------------------
    
    std::string testvector_file = "./test_vectors/HW2_query/HW2_testvector.csv";
    std::string freqvector_file = "./test_vectors/HW2_query/HW2_freqvector.csv";
    std::string binvector_file = "./test_vectors/HW2_query/HW2_binvector.csv";

    std::string res_freq_file = "./results/res_freq.csv";
    std::string res_id_file = "./results/res_id.csv";
    std::string res_time_file = "./results/res_time.csv";

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

    tv_file.close();
    fq_file.close();
    bv_file.close();

    //----------------------------------------------------------------------------------------------

    vector<std::string> row_tv_data;
    vector<unsigned int> row_fq_data;
    vector<unsigned int> row_bv_data;

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

    //----------------------------------------------------------------------------------------------

    srand(time(NULL));

    //----------------------------------------------------------------------------------------------
    // No need initialise or read bloom filter
    //----------------------------------------------------------------------------------------------
    // Search
    
    auto search_start_time = std::chrono::high_resolution_clock::now();
    auto search_stop_time = std::chrono::high_resolution_clock::now();
    auto search_time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(search_stop_time - search_start_time).count();
    auto search_total_time = chrono::duration_cast<chrono::microseconds>(search_stop_time - search_start_time).count();

    //----------------------------------------------------------------------------------------------


    for(unsigned int i=0;i<n_testvectors;++i){

        result_union.clear();

        cout << "----------------" << endl;
        cout << "Test Vector " << i << endl;

        ss.clear();
		ss << raw_tv_data.at(i);//Get the current testvector row
        std::getline(ss,s,',');// Skip the 0000
		row_tv_data.clear();
		while(std::getline(ss,s,',') && !ss.eof()) {
		    if(!s.empty()){
		        row_tv_data.push_back(s);
		    }
		}

        ss.clear();
		ss << raw_bv_data.at(i);//Get the current bin index row
		row_bv_data.clear();
		while(std::getline(ss,s,',') && !ss.eof()) {
		    if(!s.empty()){
		        row_bv_data.push_back(std::stoi(s));
		    }
		}

        search_total_time = 0;
        result_set_size = 0;
        mkws.clear();
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

                    cout << "Querying: ";

                    for(auto v:mkws){
                        cout << v << ", ";
                    }
                    cout << endl;
                    
                    //-----------------------------------
                    ::memset(UIDX,0x00,16*N_max_ids);
                    result_union_temp.clear();
                    
                    search_start_time = chrono::high_resolution_clock::now();
                    nm = EDB_Search(row_vec,n_vec-1);
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
                        cout << v << ", ";
                    }
                    cout << endl;
                    
                    //-----------------------------------
                    ::memset(UIDX,0x00,16*N_max_ids);
                    result_union_temp.clear();
                    
                    search_start_time = chrono::high_resolution_clock::now();
                    nm = EDB_Search(row_vec,n_vec-1);
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
                    cout << row_tv_data.at(j) << endl;

                    for(auto rs:mkws){
                        StrToHexBVec(row_vec+(16*n_vec),rs.data());
                        n_vec++;
                    }

                    //-----------------------------------
                    ::memset(UIDX,0x00,16*N_max_ids);
                    result_union_temp.clear();

                    search_start_time = chrono::high_resolution_clock::now();
                    nm = EDB_Search(row_vec,0);
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
        //Write results to file
        for(auto v:result_union){
            res_id_file_handle << v << ",";
        }
        res_id_file_handle << endl;

        cout << "Total time elapsed: " << search_total_time << endl;
        res_time_file_handle << search_total_time << "," << endl;

        res_freq_file_handle << result_set_size << "," << endl;

        result_union.clear();
        result_union_temp.clear();
    }

    res_freq_file_handle.close();
    res_id_file_handle.close();
    res_time_file_handle.close();

    ///////////////////////////////////////////////////////////////
    Sys_Clear();
    delete [] UIDX;
    
    cout << "Program finished!" << endl;

    return 0;
}
