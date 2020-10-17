#ifndef KEYPRODUCING_H
#define KEYPRODUCING_H

#include "gmp.h"
#include "Int.h"

// 生成一个n位(2进制)的随机数
void RandomNumber(mpz_t c, int n);

// 检查p, q的距离是否符合要求
int PQCheck(mpz_t p, mpz_t q, int k);

// 是否是k bit整数
int IsKbit(mpz_t n, int k);

// 求 Phi(n) = (p-1)(q-1)
void PhiN(mpz_t phi, mpz_t p, mpz_t q);

// 产生公私钥，(e, n) 为公钥，(d, n) 为私钥, k为产生模数的位数
void ProduceKey(int k, mpz_t n, mpz_t e, mpz_t d);

#endif