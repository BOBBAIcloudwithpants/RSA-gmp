#include "Int.h"
#include "OctetString.h"
#include "Rsa.h"
int main()
{
  mpz_t a, b, c, n;

  mpz_init(a);
  mpz_init(b);
  mpz_init(c);
  mpz_init(n);

  Init_str(a, "13513246463452342");
  Init_str(b, "13241354565556331239847813513487913491341354135");
  Init_val(c, 65536);
  OctetString *str = Octet_init(3);
  I2OSP(c, 3, str);
  OctetString *C;
  Encryption(b, a, str, C);
}
