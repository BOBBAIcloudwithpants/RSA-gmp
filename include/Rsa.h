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
int isValidEM(OctetString * EM);

// 从EM中拿出M
void EME_decoding(OctetString *EM, OctetString *M, int ps_pos);

// (e, n) 为 接收者的 public key
void Encryption(mpz_t n, mpz_t e, OctetString *M, OctetString *C);

// (n, d) 为接收者的私钥，C为密文
void Decryption(mpz_t n, mpz_t d, OctetString *C, OctetString *M);


#endif