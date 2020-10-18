#include "OctetString.h"
#include "Rsa.h"
#include "KeyProducing.h"
int main()
{
  mpz_t d, e, n;

  mpz_init(d); // d 公钥
  mpz_init(e); // e 私钥
  mpz_init(n); // n 模数

  ProduceKey(1024, n, e, d);

  char *text = "abasdfadfASFDFG"; // 明文
  OctetString *str = Octet_ConvertTextToOctets(text);
  OctetString *C = Octet_init(Octet_ValSize(n)); // 密文
  OctetString *M = Octet_init(strlen(text) - 1); // 密文解密以后得到的结果

  Encryption(n, e, str, C);

  Decryption(n, d, C, M);

  gmp_printf("n: %Zd\n", n);
  gmp_printf("e: %Zd\n", e);
  gmp_printf("d: %Zd\n", d);
  Octet_print(M);

  // char *out = Octet_ConvertOctetsToText(M);
  // for (int i = 0; i < M->len; i++)
  // {
  //   printf("%c", out[i]);
  // }
  // printf("\n");
}
