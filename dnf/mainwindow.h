#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iomanip>
#include <bits/stdc++.h>
#include <thread>
#include <algorithm>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>

#include <random>
#include <cassert>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

#include <sched.h>
#include <pthread.h>

#include <gmpxx.h>

#include "size_parameters.h"
#include "aes.h"
#include "rawdatautil.h"
#include "ecc_x25519.h"
#include "bloom_filter.h"
#include "./blake3/blake3.h" 
#include "./blake3/blake_hash.h"

#include <sw/redis++/redis++.h>
#include <sw/redis++/connection.h>
#include <sw/redis++/redis_cluster.h>

using namespace std;
using namespace sw::redis;

extern sw::redis::ConnectionOptions connection_options;
extern sw::redis::ConnectionPoolOptions pool_options;

extern string widxdb_file;
extern string eidxdb_file;
extern string bloomfilter_file;

extern int sym_block_size;
extern int ecc_block_size;
extern int hash_block_size;
extern int bhash_block_size;
extern int bhash_in_block_size;

extern mpz_class Prime;
extern mpz_class InvExp;

extern unsigned char ecc_basep[32];

extern unsigned char KS[16];
extern unsigned char KI[16];
extern unsigned char KZ[16];
extern unsigned char KX[16];
extern unsigned char KT[16];

extern unsigned char **BF;
extern unsigned char *UIDX;

extern unsigned int N_threads;

extern unsigned char *GL_AES_PT;
extern unsigned char *GL_AES_KT;
extern unsigned char *GL_AES_CT;

extern unsigned char *GL_ECC_INVA;
extern unsigned char *GL_ECC_INVP;

extern unsigned char *GL_ECC_SCA;
extern unsigned char *GL_ECC_BP;
extern unsigned char *GL_ECC_SMP;

extern unsigned char *GL_ECC_INA;
extern unsigned char *GL_ECC_INB;
extern unsigned char *GL_ECC_PRD;

extern unsigned char *GL_HASH_MSG;
extern unsigned char *GL_HASH_DGST;

extern unsigned char *GL_BLM_MSG;
extern unsigned char *GL_BLM_DGST;

extern unsigned char *GL_MGDB_RES;
extern unsigned char *GL_MGDB_BIDX;
extern unsigned char *GL_MGDB_JIDX;
extern unsigned char *GL_MGDB_LBL;

extern unsigned int *GL_OPCODE;

extern std::mutex mrun;
extern std::condition_variable dataReady;
extern std::condition_variable workComplete;

extern bool ready;
extern bool processed;

extern size_t nWorkerCount;//Number of threads
extern int nCurrentIteration;

extern std::vector<thread> thread_pool;

int Sys_Init();
int Sys_Clear();

int SetUpThreads();
int ReleaseThreads();

int WorkerThread(
    unsigned char *AES_PT,
    unsigned char *AES_KT,
    unsigned char *AES_CT,
    unsigned char *ECC_INVA,
    unsigned char *ECC_INVP,
    unsigned char *ECC_SCA,
    unsigned char *ECC_BP,
    unsigned char *ECC_SMP,
    unsigned char *ECC_INA,
    unsigned char *ECC_INB,
    unsigned char *ECC_PRD,
    unsigned char *HASH_MSG,
    unsigned char *HASH_DGST,
    unsigned char *BLM_MSG,
    unsigned char *BLM_DGST,
    unsigned char *MGDB_RES,
    unsigned char *MGDB_BIDX,
    unsigned char *MGDB_JIDX,
    unsigned char *MGDB_LBL,
    unsigned int *OPCODE
);

int TSet_SetUp();
int TSet_GetTag(unsigned char *word,unsigned char *stag);
int TSet_Retrieve(unsigned char *stag,unsigned char *tset_row, int *n_ids_tset);

int EDB_SetUp();
int EDB_Search(unsigned char *query_str, int NWords);

int FPGA_AES_ENC(unsigned char *ptext,unsigned char *key, unsigned char *ctext);
int FPGA_PRF(unsigned char *ptext,unsigned char *key, unsigned char *ctext);

int FPGA_HASH(unsigned char *msg, unsigned char *digest);
int FPGA_BLOOM_HASH(unsigned char *msg, unsigned char *digest);

int FPGA_ECC_MUL(unsigned char *in_A, unsigned char *in_B, unsigned char *prod);
int FPGA_ECC_FPINV(unsigned char *fp_x, unsigned char *fp_invx);
int FPGA_ECC_SCAMUL(unsigned char *sca, unsigned char *prod);
int FPGA_ECC_SCAMUL_BASE(unsigned char *sca, unsigned char *basep, unsigned char *prod);

int MGDB_QUERY(unsigned char *RES, unsigned char *BIDX, unsigned char *JIDX, unsigned char *LBL);

int SHA3_HASH(blake3_hasher *hasher,unsigned char *msg, unsigned char *digest);
int SHA3_HASH_K(blake3_hasher *hasher,unsigned char *msg, unsigned char *digest);
int ECC_FPINV(unsigned char *fp_x, unsigned char *fp_invx);
int ECC_MUL(unsigned char *in_A,unsigned char *in_B,unsigned char *prod);

std::string HexToStr(unsigned char *hexarr,int len=16);
std::string NumToHexStr(int num);
int StrToHex(unsigned char *hexarr,string numin);
int StrToHexBVec(unsigned char *hexarr,string bvec);
unsigned int BFIdxConv(unsigned char *hex_arr,unsigned int n_bits);

#endif // MAINWINDOW_H
