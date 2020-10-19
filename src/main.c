#include "OctetString.h"
#include "Rsa.h"
#include "KeyProducing.h"
#include "Base64.h"
int main()
{
  mpz_t d, e, n;

  mpz_init(d); // d 公钥
  mpz_init(e); // e 私钥
  mpz_init(n); // n 模数

  ProduceKey(1024, n, e, d);

  char *text = "abasdfadfASFDFGajkdshflujdhlkjqwhtioe"; // 明文
  OctetString *str = Octet_ConvertTextToOctets(text);   // 将明文转换为字节串的形式
  printf("明文: ");
  Octet_print(str);
  printf("\n");
  OctetString *C = Octet_init(Octet_ValSize(n)); // 密文
  OctetString *M = Octet_init(strlen(text));     // 密文解密以后得到的结果

  Encryption(n, e, str, C);

  Decryption(n, d, C, M);

  printf("n: ");
  PrintKey(n);
  printf("e: ");
  PrintKey(e);
  printf("d: ");
  PrintKey(d);
  Octet_print(M);

  // char *out = Octet_ConvertOctetsToText(M);
  // for (int i = 0; i < M->len; i++)
  // {
  //   printf("%c", out[i]);
  // }
  // printf("\n");
}
