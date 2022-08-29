#include <iostream>

using namespace std;

#include "blake3.h"
#include <stdlib.h>
#include <cstring>

int Blake3(blake3_hasher *hasher, uint8_t *digest,uint8_t *message)
{
    blake3_hasher_init(hasher);
    blake3_hasher_update(hasher, message, 16);
    blake3_hasher_finalize(hasher, digest, BLAKE3_OUT_LEN);
    return 0;
}

int Blake3_K(blake3_hasher *hasher, uint8_t *digest,uint8_t *message)
{
    blake3_hasher_init(hasher);
    blake3_hasher_update(hasher, message, 40);
    blake3_hasher_finalize(hasher, digest, BLAKE3_OUT_LEN);
    return 0;
}