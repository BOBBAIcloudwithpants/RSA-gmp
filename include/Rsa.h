// RSA 加密，解密的主体部分

#ifndef RSA_H
#define RSA_H

#include "Int.h"
#include "OctetString.h"

// c = (m^e)mod n
void RSAEP(mpz_t n, mpz_t e, mpz_t M, mpz_t c);

// 将 M 通过填充变成 k 位的 EM
void EME_Encoding(OctetString *M, int k, OctetString *EM);

// 将 c 转换为长度为 k 的字节串C
void I2OSP(mpz_t c, int k, OctetString *C);

// 将字节串 EM 变成整数 m
void OS2IP(OctetString *EM, mpz_t m);

// c = (d^M)modn
void RSADP(mpz_t n, mpz_t d, mpz_t M, mpz_t c);

// 判断EM是否符合格式，如果符合格式则返回 ps 的位置，否则返回0
int isValidEM(OctetString *EM);

// 从EM中拿出M
void EME_decoding(OctetString *EM, OctetString *M, int ps_pos);

// (n, e) 为 public key，M为明文的字节串，C为密文的字节串
void Encryption(mpz_t n, mpz_t e, OctetString *M, OctetString *C);

// (n, d) 为 private key，C为密文, M为解密结果的字节串
void Decryption(mpz_t n, mpz_t d, OctetString *C, OctetString *M);

#endif