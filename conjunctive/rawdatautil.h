#ifndef RAWDATAUTIL_H
#define RAWDATAUTIL_H

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <iomanip>
#include <bits/stdc++.h>
#include <iostream>

#include "size_parameters.h"

using namespace  std;

int DB_StrToHex2(unsigned char *hexarr,unsigned char *text);
int DB_StrToHex(unsigned char *hexarr,unsigned char *text);
int DB_StrToHex8(unsigned char *hexarr,unsigned char *text);
int DB_StrToHex8(unsigned char *hexarr,const char *text);
int DB_StrToHex12(unsigned char *hexarr,unsigned char *text);
int DB_StrToHex16(unsigned char *hexarr,unsigned char *text);
int DB_StrToHex32(unsigned char *hexarr,unsigned char *text);
int DB_StrToHex48(unsigned char *hexarr,const char *text);
int DB_StrToHex49(unsigned char *hexarr,unsigned char *text);
std::string DB_HexToStr(unsigned char *hexarr);
std::string DB_HexToStr_N(unsigned char *hexarr, unsigned int n);
std::string DB_HexToStr32(unsigned char *hexarr);
std::string DB_HexToStr2(unsigned char *hexarr);
std::string DB_HexToStr8(unsigned char *hexarr);
std::string DB_HexToStr12(unsigned char *hexarr);
int DB_Callback_W_IDs(void *ReturnArr, int argc, char **argv, char **azColName);
int DB_Callback_E_IDs(void *ReturnArr, int argc, char **argv, char **azColName);
int DB_Callback_T_IDs(void *ReturnArr, int argc, char **argv, char **azColName);

#endif // RAWDATAUTIL_H
