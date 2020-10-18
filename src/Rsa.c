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
  gmp_printf("c: %Zd\nCypher: ", c);

  I2OSP(c, k, C);
  Octet_print(C);
  printf("aaaadsfa");

  // mpz_clear(m);
  // mpz_clear(c);
  // Octet_free(EM);
};

int isValidEM(OctetString *EM)
{
  int ps_pos = EM->pos - 1;
  while (ps_pos > 1 && EM->arr[ps_pos] != 0)
  {
    ps_pos--;
  }

  if (EM->arr[0] != 0 || EM->arr[1] != 2 || ps_pos <= 1 || ps_pos - 2 < 8)
  {
    return 0;
  }
  else
  {
    return ps_pos;
  }
}

void EME_decoding(OctetString *EM, OctetString *M, int ps_pos)
{
  int ps_len = ps_pos - 2;
  int mlen = EM->len - ps_len - 2;
  M = Octet_init(mlen);

  for (int i = 0; i < mlen; i++)
  {
    Octet_appendVal(M, EM->arr[ps_pos + i + 1]);
  }
}

void Decryption(mpz_t n, mpz_t d, OctetString *C, OctetString *M)
{
  printf("aaaa");
  int k = Octet_ValSize(n);
  printf("\nDecryption\nk: %d\nC-ken: %d\n", k, C->len);
  if (C->len != k)
  {
    printf("decryption error");
    return;
  }

  mpz_t c;
  mpz_init(c);
  OS2IP(C, c);

  gmp_printf("OS2IP->c: %Zd", c);
  mpz_t n_1;
  mpz_init(n_1);
  Minus_val(n_1, n, 1);

  if (Compare_val(c, 0) <= 0 || Compare_mpz(c, n_1) >= 0)
  {
    mpz_clear(n_1);
    mpz_clear(c);
    printf("ciphertext representative out of range\n");
    return;
  }

  mpz_t m;
  mpz_init(m);

  RSADP(n, d, c, m);

  OctetString *EM = Octet_init(k);
  I2OSP(m, k, EM);

  int ps_pos = isValidEM(EM);
  if (!ps_pos)
  {
    mpz_clear(c);
    mpz_clear(m);
    mpz_clear(n_1);
    Octet_free(EM);
    printf("message too long\n");
    return;
  }
  EME_decoding(EM, M, ps_pos);

  mpz_clear(c);
  mpz_clear(m);
  mpz_clear(n_1);
  Octet_free(EM);
}
