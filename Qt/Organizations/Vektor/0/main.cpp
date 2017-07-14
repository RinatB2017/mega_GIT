//------------------------------------------------------------------------
// http://stackoverflow.com/questions/27939882/fast-crc-algorithm
//------------------------------------------------------------------------
#include <stddef.h>
#include <stdint.h>

/* CRC-32C (iSCSI) polynomial in reversed bit order. */
#define POLY 0x82f63b78

/* CRC-32 (Ethernet, ZIP, etc.) polynomial in reversed bit order. */
/* #define POLY 0xedb88320 */

uint32_t crc32c(uint32_t crc, const unsigned char *buf, size_t len)
{
    int k;

    crc = ~crc;
    while (len--) {
        crc ^= *buf++;
        for (k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ POLY : crc >> 1;
    }
    return ~crc;
}
//------------------------------------------------------------------------

#include <stdio.h>

int main(void)
{
    uint32_t crc = 0;
    unsigned char buf[100] = { 0 };
    size_t len = 100;
    
    uint32_t crc32 = crc32c(crc, &buf[0], len);
    printf("crc32 0x%x\n", crc32);
    
    return 0;
}
