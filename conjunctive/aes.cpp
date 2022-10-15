#include <stdint.h>
#include <string.h>
#include "aes.h"

//Replace with AES-CTR
int AESENC(uint8_t *ctext,uint8_t *ptext,uint8_t *key){
    __m128i key_schedule[20];
    aes128_load_key(key,key_schedule);
    aes128_enc(key_schedule,ptext,ctext);
    return 0;
}

//Replace with AES-CTR
int AESDEC(uint8_t *ptext,uint8_t *ctext,uint8_t *key){
    __m128i key_schedule[20];
    aes128_load_key(key,key_schedule);
    aes128_dec(key_schedule,ctext,ptext);
    return 0;
}

//Replace with AES-CMAC
int PRF_K(uint8_t *prf_out,uint8_t *ptext){
    __m128i key_schedule[20];
    aes128_load_key(prf_key,key_schedule);
    aes128_enc(key_schedule,ptext,prf_out);
    return 0;
}

//For experimental/debugging purpose only
//In practice, replace with AES-CMAC
int PRF(uint8_t *ctext,uint8_t *ptext,uint8_t *key){
    __m128i key_schedule[20];
    aes128_load_key(key,key_schedule);
    aes128_enc(key_schedule,ptext,ctext);
    return 0;
}