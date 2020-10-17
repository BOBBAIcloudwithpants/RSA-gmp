#include "Int.h"

void Add(mpz_t c, mpz_t a, mpz_t b)
{
  mpz_add(c, a, b);
}

void Minus(mpz_t c, mpz_t a, mpz_t b)
{
  mpz_sub(c, a, b); /* c = a - b */
}

void Product(mpz_t c, mpz_t a, mpz_t b)
{
  mpz_mul(c, a, b); /* c = a - b */
}

void Div(mpz_t c, mpz_t a, mpz_t b)
{
  mpz_div(c, a, b);
}

void Init_val(mpz_t a, long long num)
{
  mpz_init_set_ui(a, num);
}

void Add_val(mpz_t c, mpz_t a, long long b)
{
  mpz_add_ui(c, a, b);
}

void Init_str(mpz_t a, const char *str)
{

  mpz_init_set_str(a, str, 10);
}

int Compare_mpz(mpz_t a, mpz_t b)
{
  return mpz_cmp(a, b);
}

int Compare_val(mpz_t a, long long val)
{
  return mpz_cmp_ui(a, val);
}

void PowerMod(mpz_t c, mpz_t a, mpz_t b, mpz_t n)
{
  mpz_powm(c, a, b, n);
};

void Power(mpz_t c, mpz_t a, int b)
{
  mpz_pow_ui(c, a, b);
}

void Mod(mpz_t c, mpz_t a, mpz_t b)
{
  mpz_mod(c, a, b);
};

int Int(mpz_t a)
{
  return mpz_get_ui(a);
}

void Div_val(mpz_t c, mpz_t a, long long b)
{
  mpz_div_ui(c, a, b);
}

void Minus_val(mpz_t c, mpz_t a, long long b)
{
  mpz_sub_ui(c, a, b);
};