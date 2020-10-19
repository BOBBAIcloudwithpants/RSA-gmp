// 对于较长明文的加密，解密

#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include "Rsa.h"

// n 的位数
static int K = 1024;

// 最大的能够处理的明文段
static int SegmentSize = 117;
typedef long long index_t;

void TextEncryption(char *Text, OctetString *CipherText, mpz_t e, mpz_t n);

index_t CipherDecryption(OctetString *CipherText, char *Output, mpz_t d, mpz_t n);

#endif