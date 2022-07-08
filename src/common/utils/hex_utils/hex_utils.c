#include "hex_utils.h"

/*
  ┌───────────────────────────────┐
  │   HEX2INT                     │
  └───────────────────────────────┘
 */

uint64_t hex2int(char *hex) {
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
    return val;
}

/*
  ┌───────────────────────────────┐
  │   UINT2HEX                    │
  └───────────────────────────────┘
 */

char *uint2hex(uint64_t *val) {
    // named like this to avoid accidental overwrites
    static char uint2hexuint2hexuint2hex[16];

    for (int i = 0; i < 8; ++i) {
        uint2hexuint2hexuint2hex[i] = val[i] % 16;
        if (uint2hexuint2hexuint2hex[i] > 9) {
        uint2hexuint2hexuint2hex[i] += 'A' - 10;
        } else {
        uint2hexuint2hexuint2hex[i] += '0';
        }
    }
    printf("%s\n", uint2hexuint2hexuint2hex);
    uint2hexuint2hexuint2hex[16] = '\0';

    return uint2hexuint2hexuint2hex;
}

void reverse_string(char *str, uint32_t length) {
    uint32_t i, j;
    for (i = 0, j = length - 1; i < j; i++, j--) {
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

int hex_length(uint64_t *src) {
  // get length of src in base 16
  int x = ceil(log(*src + 1) / log(16));
  // if length is odd, add one else return x
  // accounts for bytes that start at 0 (e.g. 01...0F)
  x % 2 ? x += 1 : x;
  // only care about whole bytes, not individual digits
  return x / 2;
}