#ifndef ECC_X25519_H
#define ECC_X25519_H

#include <iostream>
#include <cstring>
#include <string>
#include <gmpxx.h>

using namespace std;

extern mpz_class ECC_Prime;
extern mpz_class ECC_InvExp;

int DoubleAndAdd(mpz_class &P4x,mpz_class &P4z,mpz_class &P5x,mpz_class &P5z, mpz_class P2x,mpz_class P2z,mpz_class P3x,mpz_class P3z,mpz_class X1);
int ScalarMul(unsigned char *product, unsigned char *scalar, unsigned char *basep);

#endif // ECC_X25519_H
