#ifndef SIZEPARAMETERS_H
#define SIZEPARAMETERS_H

extern int N_keywords;
extern int N_max_ids;
extern int N_row_ids;
extern int BF_length;

#define N_HASH 24 //Equal to number of threads

// #define MAX_BF_BIN_SIZE 134217728 //27 bits
#define MAX_BF_BIN_SIZE 131072 //17 bits
// #define MAX_BF_BIN_SIZE 1024 //10 bits

#endif // SIZEPARAMETERS_H
