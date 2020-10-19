#include "KeyProducing.h"

gmp_randstate_t state;

// 生成一个n位(2进制)的随机数
void RandomNumber(mpz_t c, int n)
{
  mpz_rrandomb(c, state, n);
  mpz_nextprime(c, c);
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
  if (mpz_sizeinbase(n, 2) == k)
    return 1;
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
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));
  mpz_t phiN;
  mpz_init(phiN);
  while (1)
  {
    mpz_t temp_p;
    mpz_init(temp_p);
    mpz_t temp_q;
    mpz_init(temp_q);
    mpz_t temp_n;
    mpz_init(temp_n);

    RandomNumber(temp_p, k / 2);
    RandomNumber(temp_q, k / 2);
    Product(temp_n, temp_p, temp_q);

    if (PQCheck(temp_p, temp_q, k) && IsKbit(temp_n, k))
    {
      PhiN(phiN, temp_p, temp_q);
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

  // 公钥e固定为65537
  mpz_set_ui(e, 65537);

  // 用逆元求d
  mpz_invert(d, e, phiN);
}

void PrintKey(mpz_t key)
{
  int k = Octet_ValSize(key);
  OctetString *octetKey = Octet_init(k);
  I2OSP(key, k, octetKey);
  Octet_printHex(octetKey);
  Octet_free(octetKey);
}
