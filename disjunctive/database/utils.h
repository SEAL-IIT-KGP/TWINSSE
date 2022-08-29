#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <numeric>
#include <chrono>
#include <cstring>
#include <cstddef>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <bitset>
#include <random>
#include <algorithm>
#include <functional>
#include <stdexcept>


typedef unsigned char byte;

#include <cryptopp/sha.h>
#include <cryptopp/config.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <bits/stdc++.h>
#define Digest
#include <cryptopp/files.h>
#include <sstream>
#include <iomanip>


// #define N_KW 133958
// #define N_ID 517401

#define N_KW 6043
#define N_ID 10000

// #define N_KW 22087
// #define N_ID 25000

// #define N_KW 32
// #define N_ID 65

#define ID_HEX_LEN 8 //size in bytes
#define MKW_HEX_LEN 8 //size in bytes

using namespace std;
using namespace std::chrono;
using namespace CryptoPP;

enum KW_TYPE {DEC,HEX};

#endif