#include "OctetString.h"
#include <stdlib.h>
#include <time.h>

// 根据给定的长度初始化一个 Array
OctetString *Octet_init(int len)
{

  OctetString *arr = (OctetString *)malloc(sizeof(OctetString));
  arr->arr = (int *)malloc(sizeof(int) * len);

  arr->len = len;
  arr->pos = 0;
  for (int i = 0; i < len; i++)
  {
    arr->arr[i] = 0;
  }

  return arr;
}

// 反转 Array 中的 arr
void Octet_reverse(OctetString *array)
{
  int start = 0;
  int end = array->pos - 1;
  while (start < end)
  {
    int temp = array->arr[start];
    array->arr[start] = array->arr[end];
    array->arr[end] = temp;
    start++;
    end--;
  }
}

void Octet_appendVal(OctetString *array, int val)
{
  array->arr[array->pos] = val;
  array->pos++;
}

int Octet_getValByIndex(OctetString *array, int index)
{
  return array->arr[index];
}

int Octet_size(OctetString *array)
{
  return array->pos;
}

void Octet_print(OctetString *array)
{
  printf("[ ");
  for (int i = 0; i < array->len; i++)
  {
    printf("%d ", Octet_getValByIndex(array, i));
  }
  printf("]\n");
}

// 根据整数长度获得对应的 OctetString 的长度
int Octet_getOctetlengthForInteger(mpz_t num)
{
  int length = 0;
  mpz_t base256;
  mpz_init(base256);

  Init_val(base256, 256);
  while (Compare_val(num, 0) != 0)
  {
    length++;
    Div(num, num, base256);
  }

  mpz_clear(base256);
  return length;
}

// Generate PS
OctetString *Octet_generatePS(int length)
{
  OctetString *ps = Octet_init(length);

  srand((unsigned)time(NULL));
  for (int i = 0; i < length; i++)
  {
    int num = rand() % 256;
    Octet_appendVal(ps, num);
  }

  return ps;
}

void Octet_free(OctetString *os)
{
  free(os->arr);
  free(os);
}

int Octet_ValSize(mpz_t val)
{
  if (Compare_val(val, 0) == 0)
  {
    return 1;
  }

  mpz_t x;
  mpz_init(x);
  mpz_set(x, val);

  int ret = 0;

  while (Compare_val(x, 0) > 0)
  {
    ret++;
    Div_val(x, x, 256);
  }

  mpz_clear(x);
  return ret;
}

int Octet_ConvertCharToInt(char a)
{

  // 0到9
  if (a < 65)
  {
    return a - '0';
  }
  else if (a >= 65 && a <= 90)
  {
    return 10 + a - 'A';
  }
  else
  {
    return 10 + a - 'a';
  }
}

int Octet_ConvertByteToInt(char h, char l)
{
  return Octet_ConvertCharToInt(h) * 16 + Octet_ConvertCharToInt(l);
}

void Octet_ConvertKeyToInt(const char *key, mpz_t a)
{
  int len = strlen(key);

  int i = 0;
  while (i < len)
  {
    Product_val(a, a, 256);
    unsigned char h = key[i];
    unsigned char l = key[i + 1];
    Add_val(a, a, Octet_ConvertByteToInt(h, l));
    i += 3;
  }
};

OctetString *Octet_ConvertTextToOctets(char *text)
{
  long long len = strlen(text);
  OctetString *ret = Octet_init(len);
  for (int i = 0; i < len; i++)
  {
    unsigned char t = text[i];
    Octet_appendVal(ret, t);
  }
  return ret;
}

char *Octet_ConvertOctetsToText(OctetString *octets)
{
  char *ret = (char *)malloc(sizeof(char) * (octets->len));
  int i = 0;
  for (i = 0; i < octets->len; i++)
  {
    ret[i] = octets->arr[i];
  }
  return ret;
}

void Octet_printHex(OctetString *oc)
{
  for (int i = 0; i < oc->len; i++)
  {
    printf("%02x", Octet_getValByIndex(oc, i));
    if (i < oc->len - 1)
    {
      printf(":");
    }
  }
  printf("\n");
}