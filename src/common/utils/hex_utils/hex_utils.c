#include "hex_utils.h"

#include <math.h>
#include <stdio.h>

/*
  ┌───────────────────────────────┐
  │   HEX2INT                     │
  └───────────────────────────────┘
 */

uint64_t hex2int(char *hex, int length) {
  uint64_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
    }
    // get shift amount from length (multiplying by 4 since hex_str_len returns length in bytes)
    val = val << 64 - (hex_str_len(&hex) * 4);
    
    printf("%llX\n", val);
    return val;
}

/*
  ┌───────────────────────────────┐
  │   UINT2HEX                    │
  └───────────────────────────────┘
 */

char *uint2hex(uint64_t *val) {
  char *hex = NULL;
    return hex;
}

void reverse_string(char *str, uint32_t length) {
    uint32_t i, j;
    for (i = 0, j = length - 1; i < j; ++i, --j) {
        uint8_t c;
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

/*
  ┌───────────────────────────────┐
  │   HEX LENGTH                  │
  └───────────────────────────────┘
 */

int hex_uint64_len(uint64_t *src) {
  // get length of src in base 16
  int x = ceil(log(*src + 1) / log(16));
  // if length is odd, add one else return x
  // accounts for bytes that start at 0 (e.g. 01...0F)
  // only care about whole bytes, not individual digits
  return (x % 2 ? x += 1 : x) / 2;
}

int hex_str_len(char *hex_str) {
    int len = 0;

    for(; *hex_str != '\0'; ++hex_str) {
        ++len;
    }

  // if len is odd, add one else return len
  return len % 2 ? len += 1 : len;
}