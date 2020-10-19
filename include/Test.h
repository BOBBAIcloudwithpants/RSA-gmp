#include "OctetString.h"
#include "Rsa.h"
#include "KeyProducing.h"
#include "Base64.h"

// 测试输入为空串的情况
void TestCase1();

// 测试输入为 ascii 字符的情况
void TestCase2();

// 测试输入为 UTF8 字符和  ascii 字符的情况
void TestCase3();

// 测试输入长度较长的情况