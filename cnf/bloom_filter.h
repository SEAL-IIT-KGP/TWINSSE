#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "size_parameters.h"

int BloomFilter_Init(unsigned char** &BF);
int BloomFilter_Set(unsigned char** &BF, unsigned int* indices);
int BloomFilter_Set_N(unsigned char** &BF, unsigned int** indices, int n_idx);
int BloomFilter_Match(unsigned char** &BF, unsigned int* indices, bool* is_present);
int BloomFilter_Match_N(unsigned char** &BF, unsigned int** indices, unsigned int n_words, bool* is_present);
int BloomFilter_Clean(unsigned char** &BF);

int BloomFilter_WriteBFtoFile(std::string bloomfilter_file, unsigned char** &BF);
int BloomFilter_ReadBFfromFile(std::string bloomfilter_file, unsigned char** &BF);
