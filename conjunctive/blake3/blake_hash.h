#ifndef SHA512_H
#define SHA512_H

#include <string.h>
#include <stdint.h>
int Blake3(blake3_hasher *hasher, uint8_t *digest,uint8_t *message);
int Blake3_K(blake3_hasher *hasher, uint8_t *digest,uint8_t *message);

#endif // SIZEPARAMETERS_H
