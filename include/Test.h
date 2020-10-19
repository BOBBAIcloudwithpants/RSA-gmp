// 测试用例的定义

#ifndef TEST_H
#define TEST_H

#include "OctetString.h"
#include "Rsa.h"
#include "KeyProducing.h"
#include "Encryption.h"

// 测试输入为长度较短的明文的情况
void TestCase1();

// 测试输入为长度较长的明文情况
void TestCase2();

// 测试出现长度异常的情况
void TestCase3();

// 测试出现UTF8字符也可以正常加密解密
void TestCase4();

#endif