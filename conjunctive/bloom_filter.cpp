#include "bloom_filter.h"

int BloomFilter_Init(unsigned char** &BF)
{
    BF = new unsigned char* [N_HASH];
    for(unsigned int k=0;k<N_HASH;++k){
        BF[k] = new unsigned char [MAX_BF_BIN_SIZE];
        ::memset(BF[k],0x00,MAX_BF_BIN_SIZE);
    }
    return 0;
}

int BloomFilter_Set(unsigned char** &BF, unsigned int* indices)
{
    for(unsigned int k=0;k<N_HASH;++k){
        BF[k][indices[k]] = 0x01;
    }
    return 0;
}

int BloomFilter_Set_N(unsigned char** &BF, unsigned int** indices, int n_idx)
{
    for(unsigned int k=0;k<N_HASH;++k){
        BF[k][indices[k][n_idx]] = 0x01;
    }
    return 0;
}

int BloomFilter_Match(unsigned char** &BF, unsigned int* indices, bool* is_present)
{
    bool is_in_part = true;
    for(size_t k=0;k<N_HASH;++k){
        is_in_part &= ((BF[k])[indices[k]] == 0x01);
    }
    *is_present = is_in_part;
    return 0;
}

int BloomFilter_Match_N(unsigned char** &BF, unsigned int** indices, unsigned int n_words, bool* is_present)
{
    bool is_in_part = true;
    for(size_t k=0;k<N_HASH;++k){
        for(size_t l=0;l<n_words;++l){
            is_in_part &= ((BF[k])[indices[k][l]] == 0x01);
        }
    }
    *is_present = is_in_part;
    return 0;
}

int BloomFilter_Clean(unsigned char** &BF)
{
    for(size_t k=0;k<N_HASH;++k){
        delete [] BF[k];
    }
    delete [] BF;
    return 0;
}

int BloomFilter_WriteBFtoFile(std::string bloomfilter_file, unsigned char** &BF)
{
    std::ofstream outputfile;
    outputfile.open(bloomfilter_file,std::ios_base::out);

    for(unsigned int i=0;i<N_HASH;++i){
        for(unsigned int j=0;j<MAX_BF_BIN_SIZE;++j){
            outputfile << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(BF[i][j]) & 0xFF) << "\n";
        }
    }

    outputfile.close();
    return 0;
}

int BloomFilter_ReadBFfromFile(std::string bloomfilter_file, unsigned char** &BF)
{
    std::ifstream inputfile;
    inputfile.open(bloomfilter_file,std::ios_base::in);

    std::string fline;
    char temp[2];
    const char *tstr;
    unsigned int n_hash = 0;
    unsigned int bf_idx = 0;

    while(std::getline(inputfile,fline) && (n_hash < N_HASH)){
        tstr = fline.data();
        temp[0] = tstr[0];
        temp[1] = tstr[1];
        BF[n_hash][bf_idx] = std::strtoul(temp,nullptr,16) & 0xFF;
        fline.clear();
        bf_idx++;
        if(bf_idx == MAX_BF_BIN_SIZE){
            n_hash++;
            bf_idx = 0;
        }
    }

    inputfile.close();

	return 0;
}
