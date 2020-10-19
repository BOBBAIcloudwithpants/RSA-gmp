#include "Encryption.h"

index_t min(index_t a, index_t b)
{
  return a > b ? b : a;
}

void copy_segment(char *dst, char *src, index_t s, index_t e)
{
  for (int i = s; i < e; i++)
  {
    dst[i - s] = src[i];
  }
}

void TextEncryption(char *Text, OctetString *CipherText, mpz_t e, mpz_t n)
{
  // 明文长度
  index_t len = strlen(Text);

  // 分段数量
  int seg_num = (len % 117 == 0) ? len / 117 : len / 117 + 1;

  index_t i = 0;
  while (i < len)
  {
    index_t floor = min(i + SegmentSize, len);

    // 当前段的长度
    int length = floor - i;
    // 求出当前段内的内容
    char *text = (char *)malloc(sizeof(char) * (floor - i));
    copy_segment(text, Text, i, floor);
    // 求出当前段的字节串形式
    OctetString *octet_text = Octet_init(length);
    Octet_ConvertTextToOctets(octet_text, text, length);

    // 初始化分段密文的长度
    OctetString *cipher = Octet_init(K / 8);

    // 将这一组明文作为 RSA 的加密输入
    Encryption(n, e, octet_text, cipher);

    for (int j = 0; j < K / 8; j++)
    {
      Octet_appendVal(CipherText, Octet_getValByIndex(cipher, j));
    }
    if (len - i >= SegmentSize)
    {
      i += SegmentSize;
    }
    else
    {
      i = len;
    }

    Octet_free(cipher);
    free(text);
    Octet_free(octet_text);
  }
}

index_t CipherDecryption(OctetString *CipherText, char *Output, mpz_t d, mpz_t n)
{
  index_t i = 0;
  index_t idx = 0;
  while (i < CipherText->len)
  {
    OctetString *c = Octet_init(K / 8);
    for (int j = 0; j < K / 8; j++)
    {
      Octet_appendVal(c, CipherText->arr[j + i]);
    }

    OctetString *m = Octet_init(K / 8);
    Decryption(n, d, c, m);
    int floor = min(m->pos, K / 8);
    for (int j = 0; j < floor - 1; j++)
    {
      Output[idx++] = m->arr[j];
    }
    i += K / 8;
  }
  return idx;
};
