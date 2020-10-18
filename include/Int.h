#ifndef INT_H
#define INT_H
#include <stdio.h>
#include "gmp.h"

void Add(mpz_t c, mpz_t a, mpz_t b);

void Minus(mpz_t c, mpz_t a, mpz_t b);

void Product(mpz_t c, mpz_t a, mpz_t b);

void Div(mpz_t c, mpz_t a, mpz_t b);

void Mod(mpz_t c, mpz_t a, mpz_t b);

// c = a^b mod n
void PowerMod(mpz_t c, mpz_t a, mpz_t b, mpz_t n);

void Init_val(mpz_t a, long long num);

void Init_str(mpz_t a, const char *str);

void Product_val(mpz_t a, mpz_t b, long long c);

void Add_val(mpz_t c, mpz_t a, long long b);

void Div_val(mpz_t c, mpz_t a, long long b);

void Minus_val(mpz_t c, mpz_t a, long long b);

int Compare_mpz(mpz_t a, mpz_t b);

int Compare_val(mpz_t a, long long val);

int Int(mpz_t a);

// c=a^b
void Power(mpz_t c, mpz_t a, int b);

#endif
