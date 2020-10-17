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
  for (int i = 0; i < Octet_size(array); i++)
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