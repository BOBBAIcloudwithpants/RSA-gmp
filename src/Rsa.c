#include "Rsa.h"

void RSAEP(mpz_t n, mpz_t e, mpz_t M, mpz_t c)
{
  PowerMod(c, e, M, n);
};

void EME_Encoding(OctetString *M, int k, OctetString *EM)
{
  OctetString *ps = Octet_generatePS(k - M->len - 3);
  // 0x00
  Octet_appendVal(EM, 0);
  // 0x02
  Octet_appendVal(EM, 2);
  // PS
  for (int i = 0; i < ps->len; i++)
  {
    Octet_appendVal(EM, ps->arr[i]);
  }
  // 0x00
  Octet_appendVal(EM, 0);
  // M
  for (int i = 0; i < M->len; i++)
  {
    Octet_appendVal(EM, M->arr[i]);
  }
}

void I2OSP(mpz_t c, int k, OctetString *C)
{

  mpz_t x, base256;
  mpz_init(x);
  mpz_init(base256);
  mpz_set(x, c);
  Init_val(base256, 256);

  while (Compare_val(x, 0) != 0)
  {
    mpz_t mod;
    mpz_init(mod);
    Mod(mod, x, base256);
    Div(x, x, base256);
    Octet_appendVal(C, Int(mod));
    mpz_clear(mod);
  }

  int pad_num = k - Octet_size(C);
  for (int i = 0; i < pad_num; i++)
  {
    Octet_appendVal(C, 0);
  }
  Octet_reverse(C);

  mpz_clear(base256);
  mpz_clear(x);
}

void OS2IP(OctetString *EM, mpz_t m)
{
  mpz_t base256;
  mpz_init(base256);
  Init_val(base256, 256);
  if (EM->pos == 0)
  {
    return;
  }
  for (int i = 0; i < EM->pos; i++)
  {
    Product(m, m, base256);
    Add_val(m, m, EM->arr[i]);
  }

  mpz_clear(base256);
};

void RSADP(mpz_t n, mpz_t d, mpz_t M, mpz_t c)
{
  PowerMod(c, d, M, n);
}

void Encryption(mpz_t n, mpz_t e, OctetString *M, OctetString *C)
{
  int k = Octet_ValSize(n);
  C = Octet_init(k);
  printf("k: %d\n", k);

  gmp_printf("n: %Zd\ne: %Zd\n", n, e);

  printf("M: ");
  Octet_print(M);
  printf("%d\n", M->len);
  if (M->len > k - 11)
  {
    printf("Message too long\n");
    return;
  }

  OctetString *EM = Octet_init(k);
  EME_Encoding(M, k, EM);
  printf("EM: ");
  Octet_print(EM);

  mpz_t m;
  mpz_init(m);
  Init_val(m, 0);
  mpz_t c;
  mpz_init(c);
  Init_val(c, 0);

  OS2IP(EM, m);
  gmp_printf("m: %Zd\n", m);

  if (Compare_mpz(m, n) > 0)
  {
    printf("Message representative out of range\n");
    Octet_free(EM);
    mpz_clear(m);
    mpz_clear(c);
    return;
  }

  RSAEP(n, e, m, c);
  gmp_printf("c: %Zd\n", c);

  I2OSP(c, k, C);
  Octet_print(C);
};

