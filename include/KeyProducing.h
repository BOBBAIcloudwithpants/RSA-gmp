#ifndef KEYPRODUCING_H
#define KEYPRODUCING_H

#include "gmp.h"
#include "Int.h"

// 生成一个n位(2进制)的随机数
void RandomNumber(mpz_t c, int n)
{
  gmp_randstate_t state;
  gmp_randinit_default(state);

  mp_bitcnt_t v;
  v = n;
  mpz_rrandomb(c, state, v);
}

// 检查p, q的距离是否符合要求
int PQCheck(mpz_t p, mpz_t q, int k)
{
  mpz_t c;
  mpz_init(c);

  // c = p-q
  Minus(c, p, q);

  mpz_abs(c, c);

  mpz_t t1;
  mpz_init(t1);
  Init_val(t1, k);
  mpz_t t2;
  mpz_init(t2);
  Init_val(t2, k);

  Div_val(t1, t1, 2);
  // t2 = k/3
  Div_val(t2, t2, 3);

  // t1 = k/2-100
  Div_val(t1, t1, 100);
  if (Compare_mpz(c, t1) <= 0)
  {
    return 0;
  }

  if (Compare_mpz(c, t2) <= 0)
  {
    return 0;
  }

  return 1;
}
#endif