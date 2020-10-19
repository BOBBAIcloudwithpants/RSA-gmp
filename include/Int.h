#ifndef INT_H
#define INT_H
#include <stdio.h>
#include "gmp.h"

// c = a + b
void Add(mpz_t c, mpz_t a, mpz_t b);

// c = a - b
void Minus(mpz_t c, mpz_t a, mpz_t b);

// c = a * b
void Product(mpz_t c, mpz_t a, mpz_t b);

// c = a / b
void Div(mpz_t c, mpz_t a, mpz_t b);

// c = a % b
void Mod(mpz_t c, mpz_t a, mpz_t b);

// c = a^b mod n
void PowerMod(mpz_t c, mpz_t a, mpz_t b, mpz_t n);

// 用整数初始化 a
void Init_val(mpz_t a, long long num);

// 用数字字符串字面量来初始化 a
void Init_str(mpz_t a, const char *str);

// a = b * c(long long 类型)
void Product_val(mpz_t a, mpz_t b, long long c);

// c = a + b(long long 类型)
void Add_val(mpz_t c, mpz_t a, long long b);

// c = a / b(long long 类型)
void Div_val(mpz_t c, mpz_t a, long long b);

// c = a - b(long long 类型)
void Minus_val(mpz_t c, mpz_t a, long long b);

// a > b返回正数；a == b 返回0；a < b 返回负数
int Compare_mpz(mpz_t a, mpz_t b);

// b 为 long long 类型。a > b返回正数；a == b 返回0；a < b 返回负数
int Compare_val(mpz_t a, long long b);

// 将 a 转换成 int
int Int(mpz_t a);

// c = a ^ b
void Power(mpz_t c, mpz_t a, int b);

#endif
