#include "KeyProducing.h"

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

int IsKbit(mpz_t n, int k)
{
  mpz_t base;
  mpz_init(base);
  Init_val(base, 2);

  // 2^(k-1)
  mpz_t low;
  mpz_init(low);
  Power(low, base, k - 1);

  // 2^k - 1
  mpz_t high;
  mpz_init(high);
  Power(high, base, k);
  Minus_val(high, high, 1);

  if (Compare_mpz(n, low) >= 0 && Compare_mpz(n, high) <= 0)
  {
    return 1;
  }
  return 0;
}

void PhiN(mpz_t phi, mpz_t p, mpz_t q)
{
  mpz_t p_1;
  mpz_init(p_1);
  Minus_val(p_1, p, 1);
  mpz_t q_1;
  mpz_init(q_1);
  Minus_val(q_1, q, 1);

  Product(phi, p_1, q_1);
}

void ProduceKey(int k, mpz_t n, mpz_t e, mpz_t d)
{

  while (1)
  {
    mpz_t temp_p;
    mpz_init(temp_p);
    mpz_t temp_q;
    mpz_init(temp_q);
    mpz_t temp_n;
    mpz_init(temp_n);

    RandomNumber(temp_p, (k + 1) / 2);
    RandomNumber(temp_q, (k - 1) / 2);
    PhiN(temp_n, temp_p, temp_q);

    if (PQCheck(temp_p, temp_q, k) && IsKbit(temp_n, k))
    {
      mpz_set(n, temp_n);
      mpz_clear(temp_n);
      mpz_clear(temp_p);
      mpz_clear(temp_q);
      break;
    }

    mpz_clear(temp_n);
    mpz_clear(temp_p);
    mpz_clear(temp_q);
  }

  mpz_set_ui(e, 65537);
}