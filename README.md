# RSA 设计报告

## 目录结构

## 数据结构设计
RSA的加密，解密流程中主要涉及到两类结构: **大整数** 以及 **字节串**.      
对于大整数，由于时间有限我并没有能够实现出效率高并且运算结果正确的大整数结构，于是选用了 `gmp` 这个c语言库。gmp库提供了存放大整数的数据类型和一些运算方法，为了方便使用，我也对于 gmp 库做了一定的封装，封装后的方法如下:
```c++
// c = a + b
void Add(mpz_t c, mpz_t a, mpz_t b);

// c = a - b
void Minus(mpz_t c, mpz_t a, mpz_t b);

// c = a * b
void Product(mpz_t c, mpz_t a, mpz_t b);

// c = a / b
void Div(mpz_t c, mpz_t a, mpz_t b);

// c = a % b
void Mod(mpz_t c, mpz_t a, mpz_t b);

// c = a^b mod n
void PowerMod(mpz_t c, mpz_t a, mpz_t b, mpz_t n);

// 用整数初始化 a
void Init_val(mpz_t a, long long num);

// 用数字字符串字面量来初始化 a
void Init_str(mpz_t a, const char *str);

// a = b * c(long long 类型)
void Product_val(mpz_t a, mpz_t b, long long c);

// c = a + b(long long 类型)
void Add_val(mpz_t c, mpz_t a, long long b);

// c = a / b(long long 类型)
void Div_val(mpz_t c, mpz_t a, long long b);

// c = a - b(long long 类型)
void Minus_val(mpz_t c, mpz_t a, long long b);

// a > b返回正数；a == b 返回0；a < b 返回负数
int Compare_mpz(mpz_t a, mpz_t b);

// b 为 long long 类型。a > b返回正数；a == b 返回0；a < b 返回负数
int Compare_val(mpz_t a, long long b);

// 将 a 转换成 int
int Int(mpz_t a);

// c = a ^ b
void Power(mpz_t c, mpz_t a, int b);
```
利用这些方法，我创建了另一个数据结构: `OctetString`.     
OctetString 的结构体定义如下：
```c++
typedef struct
{
  int *arr; // 数组指针
  int len;  // 数组声明时的长度
  int pos;  // 当前最后一个元素的位置
} OctetString;
```
根据字节串的特性，每一个字节的取值范围是 `0-255` 的，字节对应的十进制数存放在 OctetString 下的 `arr` 中。同时，`len` 和 `pos` 方便输出字节串以及为字节串添加新的字节。不直接使用 `char*` 的原因是：**有可能出现值为0的字节**。    
`OctetString` 具有以下的方法（由于 C 语言对面向对象的特性支持较差，所以并没有所谓 `某个结构体下的方法` 的概念，因此此处的 `OctetString 结构的方法` 是指一系列以 OctetString 类型的指针作为参数的函数）:
```c++
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
```

## 密钥生成
RSA 中的密钥有两种类型:    
- 公钥: 用 **(n, d)** 来表示，其中 n 是模数，d 是一个整数。我在参考了 [openssl](https://www.openssl.org/) 的 RSA 公私钥产生以及蔡老师的课件之后，决定将 `d` 固定为十进制数 `65537`.    
- 私钥: 用 **(n, e)** 来表示，其中 n 也是模数，与公钥中的n相同，e 也是一个整数。d 和 e具有这样的关系:
$$
de\ \equiv\ 1 (mod \phi(N))
$$

可以看到，由于 d 为一个常数，则如何求解 `n` 是 RSA 密钥生成中的要点，求解 n 的算法流程如下:

1. 选定 n 在二进制形式下的位数 k，比如 256 位，512 位，1024 位等等
2. 随机生成两个 `在二进制下的位数为 n/2` 的大整数 p，q
3. 令 $N = p * q$, 判断 $N$ 的二进制形式下的位数是否等于 k。如果不等于，则返回`步骤2`
4. 计算: $distance = log|p - q|$, 判断 $distance$ 是否处于 $(\frac{k}{2} - 100, \frac{k}{3})$ 这个范围内。如果不在，则返回`步骤2`。要注意的是，这一步是保证p, q的距离，以防p，q很容易被破解，如果不满足要求，并不会影响密钥的合法性，但会造成安全性的降低
5. 令 $n = p * q, \phi(n) = (p - 1) * (q - 1), d = 65537$, 利用 $de\ \equiv\ 1 (mod \phi(N))$ 的性质来求解得到 e
6. 至此，我们完成了公钥 (n, d) 和 私钥 (e, d) 的求解。

代码实现如下：
```c
void ProduceKey(int k, mpz_t n, mpz_t e, mpz_t d)
{
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));
  mpz_t phiN;
  mpz_init(phiN);
  while (1)
  {
    mpz_t temp_p;
    mpz_init(temp_p);
    mpz_t temp_q;
    mpz_init(temp_q);
    mpz_t temp_n;
    mpz_init(temp_n);

    RandomNumber(temp_p, k / 2);
    RandomNumber(temp_q, k / 2);
    Product(temp_n, temp_p, temp_q);

    if (PQCheck(temp_p, temp_q, k) && IsKbit(temp_n, k))
    {
      PhiN(phiN, temp_p, temp_q);
      mpz_set(n, temp_n);
      mpz_clear(temp_n);
      mpz_clear(temp_p);
      mpz_clear(temp_q);
      break;
    }

    mpz_clear(temp_n);
    mpz_clear(temp_p);
    mpz_clear(temp_q);
  }

  // 公钥e固定为65537
  mpz_set_ui(e, 65537);

  // 用逆元求d
  mpz_invert(d, e, phiN);
}
```
## 加密

在开始加密之前，首先需要一些辅助函数的铺垫，下面对与用到的辅助函数做介绍。

### 1. OS2IP
- 定义: 
```c
void OS2IP(OctetString *EM, mpz_t m);
```
- 作用: 将字节串 EM 变成整数 m
- 实现:
```c
void OS2IP(OctetString *EM, mpz_t m)
{
  mpz_t base256;
  mpz_init(base256);
  Init_val(base256, 256);
  if (EM->pos == 0)
  {
    return;
  }
  for (int i = 0; i < EM->pos; i++)
  {
    Product(m, m, base256);
    Add_val(m, m, EM->arr[i]);
  }

  mpz_clear(base256);
};
```
- 示例
```
e.g.1:
EM: [255, 255]    <---->    m: 65535

e.g.2:
EM: [1, 5, 84]    <---->    m: 67000

```

### 2. I2OSP
- 定义: 
```c
void I2OSP(mpz_t c, int k, OctetString *C);
```
- 功能: 将 c 转换为长度为 k 的字节串C
- 实现: 
```c
void I2OSP(mpz_t c, int k, OctetString *C)
{

  mpz_t x, base256;
  mpz_init(x);
  mpz_init(base256);
  mpz_set(x, c);
  Init_val(base256, 256);

  while (Compare_val(x, 0) != 0)
  {
    mpz_t mod;
    mpz_init(mod);
    Mod(mod, x, base256);
    Div(x, x, base256);
    Octet_appendVal(C, Int(mod));
    mpz_clear(mod);
  }

  int pad_num = k - Octet_size(C);
  for (int i = 0; i < pad_num; i++)
  {
    Octet_appendVal(C, 0);
  }
  Octet_reverse(C);

  mpz_clear(base256);
  mpz_clear(x);
}
```
- 示例:
```
e.g.1:
m: 65535, k = 2  <--->  EM: [255, 255]

e.g.2:
m: 65536, k = 3  <--->  EM: [0, 1, 0]
```

### 3. RSADP
- 定义:
```c
void RSADP(mpz_t n, mpz_t d, mpz_t M, mpz_t c);
```
- 作用: c = (d^M)modn
- 实现:
```c
void RSADP(mpz_t n, mpz_t c, mpz_t d, mpz_t m)
{
  PowerMod(m, c, d, n);
}
```

### 4. RSAEP
- 定义:
```c
void RSAEP(mpz_t n, mpz_t c, mpz_t d, mpz_t m)
```

- 作用: c = (m^e)mod n
- 实现:
```c
void RSAEP(mpz_t n, mpz_t c, mpz_t d, mpz_t m)
{
  PowerMod(m, c, d, n);
};
```

### 5. EME_Encoding
- 定义: 
```c
void EME_Encoding(OctetString *M, int k, OctetString *EM)
```

- 作用: 将字节串 M 变成 k 位长的填充后的字节串 EM, 填充的规则如下:
![](https://tva1.sinaimg.cn/large/007S8ZIlgy1gjul20qzd4j30qu09cjt1.jpg)

- 实现:
```c
void EME_Encoding(OctetString *M, int k, OctetString *EM)
{
  OctetString *ps = Octet_generatePS(k - M->len - 3);
  // 0x00
  Octet_appendVal(EM, 0);
  // 0x02
  Octet_appendVal(EM, 2);
  // PS
  for (int i = 0; i < ps->len; i++)
  {
    Octet_appendVal(EM, ps->arr[i]);
  }
  // 0x00
  Octet_appendVal(EM, 0);
  // M
  for (int i = 0; i < M->len; i++)
  {
    Octet_appendVal(EM, M->arr[i]);
  }
}
```
示例:
```
e.g.1:
M: [ 97 98 97 115 100 102 97 100 102 65 83 70 68 70 71 ]
k = 128
EM: 
[ 0 2 209 182 183 207 176 177 0 12 139 8 104 246 52 122 154 111 41 157 50 224 163 177 51 47 3 172 6 125 140 68 69 59 190 207 200 42 217 198 36 51 133 61 18 37 225 222 161 72 159 73 114 3 35 86 130 213 41 91 167 159 64 34 207 104 10 55 52 112 171 54 36 149 184 209 163 172 134 63 89 36 104 129 10 155 96 218 15 20 49 61 95 68 180 129 15 221 11 51 199 48 112 157 170 183 245 254 156 150 210 9 0 97 98 97 115 100 102 97 100 102 65 83 70 68 70 71 ]
```

### 6. EME_decoding
- 定义:
```c
void EME_decoding(OctetString *EM, OctetString *M, int ps_pos)
```
- 作用: 将符合格式的EM字符串(也就是开头两个字节是0x00, 0x02, PS 长度大于等于8)转换为M
- 实现:
```c
void EME_decoding(OctetString *EM, OctetString *M, int ps_pos)
{
  // ps_pos为ps串的结束位置
  int ps_len = ps_pos - 2;
  int mlen = EM->len - ps_len - 2;
  for (int i = 0; i < mlen; i++)
  {
    Octet_appendVal(M, EM->arr[ps_pos + i + 1]);
  }
}
```
在看过这些函数之后，终于到了加密的实现部分，加密算法的流程如下:    
1. 对于明文的字节串进行 **EME填充**
2. 通过 **OS2IP** 转换为整数
3. 整数与私钥经过 **RSADP** 运算得到一串整数
4. 将这串整数通过 **I2OSP** 得到字节串
5. 返回密文

实际的算法实现如下:
```c
void Encryption(mpz_t n, mpz_t e, OctetString *M, OctetString *C)
{
  int k = Octet_ValSize(n);

  if (M->len > k - 11)
  {
    printf("Message too long\n");
    return;
  }

  OctetString *EM = Octet_init(k);
  EME_Encoding(M, k, EM);

  mpz_t m;
  mpz_init(m);
  Init_val(m, 0);
  mpz_t c;
  mpz_init(c);
  Init_val(c, 0);

  OS2IP(EM, m);

  if (Compare_mpz(m, n) > 0)
  {
    printf("Message representative out of range\n");
    Octet_free(EM);
    mpz_clear(m);
    mpz_clear(c);
    return;
  }

  RSAEP(n, m, e, c);

  I2OSP(c, k, C);

  mpz_clear(m);
  mpz_clear(c);
  Octet_free(EM);
};
```


## 解密
解密算法与加密算法用到的工具函数基本一致，解密算法的流程如下:     
1. 将密文C的字节串通过 **OS2IP** 转为整数
2. 将这个整数与公钥一起通过 **RSADP** 运算变成一串整数
3. 将这串整数通过 **I2OSP** 变成字节串，此时的字节串应当与加密时的 EM 相同
4. 将 EM 通过 **EME_decoding** 还原到明文
5. 返回明文

具体实现如下：
```c
void Decryption(mpz_t n, mpz_t d, OctetString *C, OctetString *M)
{
  int k = Octet_ValSize(n);
  if (C->len != k)
  {
    printf("decryption error\n");
    return;
  }

  mpz_t c;
  mpz_init(c);
  OS2IP(C, c);

  mpz_t n_1;
  mpz_init(n_1);
  Minus_val(n_1, n, 1);

  if (Compare_val(c, 0) <= 0 || Compare_mpz(c, n_1) >= 0)
  {
    mpz_clear(n_1);
    mpz_clear(c);
    printf("ciphertext representative out of range\n");
    return;
  }

  mpz_t m;
  mpz_init(m);

  RSADP(n, c, d, m);

  OctetString *EM = Octet_init(k);
  I2OSP(m, k, EM);

  Octet_print(EM);
  int ps_pos = isValidEM(EM);
  if (!ps_pos)
  {
    mpz_clear(c);
    mpz_clear(m);
    mpz_clear(n_1);
    Octet_free(EM);
    printf("message too long\n");
    return;
  }
  EME_decoding(EM, M, ps_pos);

  mpz_clear(c);
  mpz_clear(m);
  mpz_clear(n_1);
  Octet_free(EM);
}
```

## 测试


