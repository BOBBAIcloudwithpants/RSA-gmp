#include "Int.h"
#include "OctetString.h"
#include "Rsa.h"
#include "KeyProducing.h"
int main()
{
  mpz_t a, d, e, n, c;

  mpz_init(a);
  mpz_init(d);
  mpz_init(e);
  mpz_init(n);
  mpz_init(c);

  ProduceKey(1024, n, e, d);
  Init_val(c, 67000);

  OctetString *str = Octet_init(3);
  I2OSP(c, 3, str);
  OctetString *C;
  OctetString *M;
  Encryption(n, e, str, C);
  Decryption(n, d, C, M);

  gmp_printf("n: %Zd\n", n);
  gmp_printf("e: %Zd\n", e);
  gmp_printf("d: %Zd\n", d);
}
