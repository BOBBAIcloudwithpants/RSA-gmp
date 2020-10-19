#include "OctetString.h"
#include "Rsa.h"
#include "KeyProducing.h"
#include "Test.h"

void TestCase1()
{
  mpz_t d, e, n;

  mpz_init(d); // d 私钥
  mpz_init(e); // e 公钥
  mpz_init(n); // n 模数
  ProduceKey(1024, n, e, d);

  printf("---------------- CASE 1 测试输入为长度较短的明文的情况 ---------------\n");
  char *text = "abasdfadfASFDFGajkdshflujdhlkjqwhtioe"; // 明文
  OctetString *str = Octet_init(strlen(text));
  Octet_ConvertTextToOctets(str, text, strlen(text)); // 将明文转换为字节串的形式
  printf("明文:          ");
  Octet_printHex(str);
  OctetString *C = Octet_init(Octet_ValSize(n)); // 密文
  OctetString *M = Octet_init(strlen(text));     // 密文解密以后得到的结果

  Encryption(n, e, str, C);
  Decryption(n, d, C, M);
  printf("密文:          ");
  Octet_printHex(C);
  printf("解密后的结果:  ");
  Octet_printHex(M);
  printf("--------------------------------------\n");
  printf("模数n: ");
  PrintKey(n);
  printf("公钥e: ");
  PrintKey(e);
  printf("私钥d: ");
  PrintKey(d);
  printf("--------------------------------------\n\n");
}

void TestCase2()
{

  mpz_t d, e, n;

  mpz_init(d); // d 私钥
  mpz_init(e); // e 公钥
  mpz_init(n); // n 模数
  ProduceKey(1024, n, e, d);

  printf("---------------- CASE 2 测试输入为长度较长的明文情况 ---------------\n");
  char *text = "adfjhalkjhcljkvzhlxkjvc,mxvbnmban,mdsbfkqkqerouiqwer19236497812657894398517yeuifghadksjbfhjkdbvnmzxbkvcbakdsjghqouiweyrtoiuqewytouigsdfkjgbhjkzxbckvjbkaskdbcasdgffafdshgadfgadsfasd";
  index_t len = strlen(text);
  // 分段数量
  int seg_num = (len % SegmentSize == 0) ? len / SegmentSize : len / SegmentSize + 1;
  index_t capacity = K / 8 * seg_num;
  OctetString *str = Octet_init(strlen(text));
  Octet_ConvertTextToOctets(str, text, strlen(text)); // 将明文转换为字节串的形式
  printf("明文:          ");
  Octet_printHex(str);

  OctetString *cipher = Octet_init(capacity);
  TextEncryption(text, cipher, e, n);
  printf("密文:          ");
  Octet_printHex(cipher);
  char output[capacity];
  int size = CipherDecryption(cipher, output, d, n);
  printf("解密后的结果:  ");
  for (int i = 0; i < size; i++)
  {
    printf("%02x", output[i]);
    if (i < size - 1)
    {
      printf(":");
    }
  }
  printf("\n--------------------------------------\n");

  printf("模数n: ");
  PrintKey(n);
  printf("公钥e: ");
  PrintKey(e);
  printf("私钥d: ");
  PrintKey(d);
  printf("--------------------------------------\n\n");
}

void TestCase3()
{
  mpz_t d, e, n;

  mpz_init(d); // d 私钥
  mpz_init(e); // e 公钥
  mpz_init(n); // n 模数
  ProduceKey(1024, n, e, d);

  printf("---------------- CASE 3 测试出现长度异常的情况 ---------------\n");
  char *text = "abasdfadfASFDFGajkdshflujdhlkjqwhtioe1234781937481798467834592345adsfaghfhsfhqadgasfaestqwertwqerywreqweradfadfdwdddsd"; // 明文
  OctetString *str = Octet_init(strlen(text));
  Octet_ConvertTextToOctets(str, text, strlen(text)); // 将明文转换为字节串的形式
  printf("明文:          ");
  Octet_printHex(str);
  OctetString *C = Octet_init(Octet_ValSize(n)); // 密文
  OctetString *M = Octet_init(strlen(text));     // 密文解密以后得到的结果

  Encryption(n, e, str, C);
  Decryption(n, d, C, M);
  printf("密文:          ");
  Octet_printHex(C);
  printf("解密后的结果:  ");
  Octet_printHex(M);
  printf("--------------------------------------\n");
  printf("模数n: ");
  PrintKey(n);
  printf("公钥e: ");
  PrintKey(e);
  printf("私钥d: ");
  PrintKey(d);
  printf("--------------------------------------\n\n");
}

void TestCase4()
{
  mpz_t d, e, n;

  mpz_init(d); // d 私钥
  mpz_init(e); // e 公钥
  mpz_init(n); // n 模数
  ProduceKey(1024, n, e, d);

  printf("---------------- CASE 4 测试出现UTF8字符也可以正常加密解密 ---------------\n");
  char *text = "abasdfadfASFDFGajkdshflujdhlkjqwhtioeRSA真好用我很喜欢"; // 明文
  OctetString *str = Octet_init(strlen(text));
  Octet_ConvertTextToOctets(str, text, strlen(text)); // 将明文转换为字节串的形式
  printf("明文:          ");
  Octet_printHex(str);
  OctetString *C = Octet_init(Octet_ValSize(n)); // 密文
  OctetString *M = Octet_init(strlen(text));     // 密文解密以后得到的结果

  Encryption(n, e, str, C);
  Decryption(n, d, C, M);
  printf("密文:          ");
  Octet_printHex(C);
  printf("解密后的结果:  ");
  Octet_printHex(M);
  printf("--------------------------------------\n");
  printf("模数n: ");
  PrintKey(n);
  printf("公钥e: ");
  PrintKey(e);
  printf("私钥d: ");
  PrintKey(d);
  printf("--------------------------------------\n\n");
}

int main()
{
  TestCase1();
  TestCase2();
  TestCase3();
  TestCase4();
}