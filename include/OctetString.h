#ifndef OCTETSTRING_H
#define OCTETSTRING_H

#include "Int.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*  Array 结构体的定义以及相关的方法  */
typedef struct
{
  int *arr; // 数组指针
  int len;  // 数组声明时的长度
  int pos;  // 当前最后一个元素的位置
} OctetString;

// 根据给定的长度初始化一个 Array
OctetString *Octet_init(int len);

// 反转 Array 中的 arr
void Octet_reverse(OctetString *array);

// 为 Octet 添加字节
void Octet_appendVal(OctetString *array, int val);

// 根据索引获取值
int Octet_getValByIndex(OctetString *array, int index);

// 获取当前 octets 中的字节数
int Octet_size(OctetString *array);

// 打印该 octets
void Octet_print(OctetString *array);

// 根据整数长度获得对应的 OctetString 的长度
int Octet_getOctetlengthForInteger(mpz_t num);

// Generate PS
OctetString *Octet_generatePS(int length);

// 将字符转换成 Octets 字节流
OctetString *Octet_ConvertTextToOctets(char *text);

// 将Octets字节流转换为字符串
char *Octet_ConvertOctetsToText(OctetString *octets);

// 释放 Octet 所占的空间
void Octet_free(OctetString *os);

// 求一个整数在字节流下的大小
int Octet_ValSize(mpz_t val);

// 将用16进制表示的密钥转换为大整数
void Octet_ConvertKeyToInt(const char *key, mpz_t m);

#endif