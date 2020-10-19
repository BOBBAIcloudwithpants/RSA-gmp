#include <stdint.h>
#include <stdlib.h>
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t b64_encoded_size(size_t inlen);

char *b64_encode(const unsigned char *in, size_t len);

