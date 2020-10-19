#include "OctetString.h"
#include "Encryption.h"
#include "Rsa.h"
#include "KeyProducing.h"

index_t MAX_CHARACTER = 100000;
int main()
{
  mpz_t d, e, n;

  mpz_init(d); // d 公钥
  mpz_init(e); // e 私钥
  mpz_init(n); // n 模数

  ProduceKey(1024, n, e, d);

  // char *text = "abasdfadfASFDFGajkdshflujdhlkjqwhtioe"; // 明文
  // OctetString *str = Octet_ConvertTextToOctets(text);   // 将明文转换为字节串的形式
  // printf("明文: ");
  // Octet_print(str);
  // printf("\n");
  // OctetString *C = Octet_init(Octet_ValSize(n)); // 密文
  // OctetString *M = Octet_init(strlen(text));     // 密文解密以后得到的结果

  // Encryption(n, e, str, C);

  // Decryption(n, d, C, M);

  // printf("n: ");
  // PrintKey(n);
  // printf("e: ");
  // PrintKey(e);
  // printf("d: ");
  // PrintKey(d);
  // Octet_print(M);

  char text[MAX_CHARACTER];
  printf("请输入明文，为了保证最后的结果是可视字符，这里仅支持输入英文字母，英文符号和阿拉伯数字，最大长度为 100000: ");
  scanf("%s", text);
  index_t len = strlen(text);
  // 分段数量
  int seg_num = (len % SegmentSize == 0) ? len / SegmentSize : len / SegmentSize + 1;
  index_t capacity = K / 8 * seg_num;

  OctetString *cipher = Octet_init(capacity);
  TextEncryption(text, cipher, e, n);
  printf("加密后的密文的字节串: ");
  Octet_print(cipher);
  char output[capacity];
  int size = CipherDecryption(cipher, output, d, n);

  printf("模数n: ");
  PrintKey(n);
  printf("公钥e: ");
  PrintKey(e);
  printf("私钥d: ");
  PrintKey(d);

  printf("\n经过加密再解密后的明文: ");
  for (int i = 0; i < size; i++)
  {
    printf("%c", output[i]);
  }
  printf("\n");
}
