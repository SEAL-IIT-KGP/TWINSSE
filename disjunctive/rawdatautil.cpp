#include "rawdatautil.h"

int DB_StrToHex2(unsigned char *hexarr,unsigned char *text)
{
    char temp[2];
    temp[0] = text[0];
    temp[1] = text[1];
    hexarr[0] = ::strtoul(temp,nullptr,16) & 0xFF;
    return 0;
}

int DB_StrToHex(unsigned char *hexarr,unsigned char *text)
{
    char temp[2];
    for (int j=0; j<2; j++)
    {
        temp[0] = text[2*j];
        temp[1] = text[2*j+1];
        hexarr[j] = ::strtoul(temp,nullptr,16) & 0xFF;
    }
    return 0;
}

int DB_StrToHex8(unsigned char *hexarr,unsigned char *text)
{
    char temp[2];
    for (int j=0; j<4; j++)
    {
        temp[0] = text[2*j];
        temp[1] = text[2*j+1];
        hexarr[j] = ::strtoul(temp,nullptr,16) & 0xFF;
    }
    return 0;
}

int DB_StrToHex8(unsigned char *hexarr,const char *text)
{
    char temp[2];
    for (int j=0; j<4; j++)
    {
        temp[0] = text[2*j];
        temp[1] = text[2*j+1];
        hexarr[j] = ::strtoul(temp,nullptr,16) & 0xFF;
    }
    return 0;
}

int DB_StrToHex12(unsigned char *hexarr,unsigned char *text)
{
    char temp[2];
    for (int j=0; j<12; j++)
    {
        temp[0] = text[2*j];
        temp[1] = text[2*j+1];
        hexarr[j] = ::strtoul(temp,nullptr,16) & 0xFF;
    }
    return 0;
}

int DB_StrToHex16(unsigned char *hexarr,unsigned char *text)
{
    char temp[2];
    for (int j=0; j<16; j++)
    {
        temp[0] = text[2*j];
        temp[1] = text[2*j+1];
        hexarr[j] = ::strtoul(temp,nullptr,16) & 0xFF;
    }
    return 0;
}

int DB_StrToHex32(unsigned char *hexarr,unsigned char *text)
{
    char temp[2];
    for (int j=0; j<32; j++)
    {
        temp[0] = text[2*j];
        temp[1] = text[2*j+1];
        hexarr[j] = ::strtoul(temp,nullptr,16) & 0xFF;
    }
    return 0;
}

int DB_StrToHex49(unsigned char *hexarr,unsigned char *text)
{
    char temp[2];
    for (int j=0; j<49; j++)
    {
        temp[0] = text[2*j];
        temp[1] = text[2*j+1];
        hexarr[j] = ::strtoul(temp,nullptr,16) & 0xFF;
    }
    return 0;
}

int DB_StrToHex48(unsigned char *hexarr,const char *text)
{
    char temp[2];
    for (int j=0; j<48; j++)
    {
        temp[0] = text[2*j];
        temp[1] = text[2*j+1];
        hexarr[j] = ::strtoul(temp,nullptr,16) & 0xFF;
    }
    return 0;
}

std::string DB_HexToStr(unsigned char *hexarr)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 16; ++i)
        ss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hexarr[i]);
    return ss.str();
}

std::string DB_HexToStr_N(unsigned char *hexarr, unsigned int n)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < n; ++i)
        ss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hexarr[i]);
    return ss.str();
}

std::string DB_HexToStr32(unsigned char *hexarr)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 32; ++i)
        ss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hexarr[i]);
    return ss.str();
}

std::string DB_HexToStr2(unsigned char *hexarr)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 2; ++i)
        ss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hexarr[i]);
    return ss.str();
}

std::string DB_HexToStr8(unsigned char *hexarr)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 4; ++i)
        ss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hexarr[i]);
    return ss.str();
}

std::string DB_HexToStr12(unsigned char *hexarr)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 12; ++i)
        ss << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hexarr[i]);
    return ss.str();
}

int DB_Callback_W_IDs(void *ReturnArr, int argc, char **argv, char **azColName){

	int max_idx = N_row_ids+3;

	unsigned char TW[16];
    unsigned char *TSTR = reinterpret_cast<unsigned char*>(ReturnArr);

    ::memset(TW,0x00,16);
    DB_StrToHex8(TW,reinterpret_cast<unsigned char*>(argv[1]));//Keywords are 32-bit long
    ::memcpy(TSTR,TW,16);

    TSTR += 16;

    ::memset(TW,0x00,2);
    DB_StrToHex8(TW,reinterpret_cast<unsigned char*>(argv[2]));//lengths are 8-bit long
    ::memcpy(TSTR,TW,2);

    TSTR -= 14;

    ::memset(TW,0x00,16);
    for(int i=3;i < max_idx;++i){
        TSTR += 16;
        DB_StrToHex8(TW,reinterpret_cast<unsigned char*>(argv[i]));
        ::memcpy(TSTR,TW,16);
    }

    return 0;
}

int DB_Callback_E_IDs(void *ReturnArr, int argc, char **argv, char **azColName){

    unsigned char TW[16];
    unsigned char TYID[32];
    unsigned char TEC[16];
    unsigned char *TSTR = reinterpret_cast<unsigned char*>(ReturnArr);

    ::memset(TW,0x00,16);
    DB_StrToHex8(TW,reinterpret_cast<unsigned char*>(argv[1]));//Keywords are 32-bit long
    ::memcpy(TSTR,TW,16);

    TSTR += 16;

    ::memset(TW,0x00,2);
    DB_StrToHex8(TW,reinterpret_cast<unsigned char*>(argv[2]));//lengths are 8-bit long
    ::memcpy(TSTR,TW,16);

    TSTR +=2;

    ::memset(TYID,0x00,32);
    ::memset(TEC,0x00,16);

    for(int i=0;i<128;++i){
        DB_StrToHex32(TYID,reinterpret_cast<unsigned char*>(argv[(2*i)+3]));
        ::memcpy(TSTR,TYID,32);
        DB_StrToHex16(TEC,reinterpret_cast<unsigned char*>(argv[(2*i)+4]));
        ::memcpy(TSTR+32,TEC,16);
        TSTR += 48;
    }

    return 0;
}

int DB_Callback_T_IDs(void *ReturnArr, int argc, char **argv, char **azColName){

    unsigned char TLBL[12];
    unsigned char TVAL[49];
    unsigned char *TSTR = reinterpret_cast<unsigned char*>(ReturnArr);

    ::memset(TLBL,0x00,12);
    DB_StrToHex12(TLBL,reinterpret_cast<unsigned char*>(argv[3]));
    ::memcpy(TSTR,TLBL,12);

    TSTR +=12;

    ::memset(TVAL,0x00,49);
    DB_StrToHex49(TVAL,reinterpret_cast<unsigned char*>(argv[4]));
    ::memcpy(TSTR,TVAL,49);

    return 0;
}
