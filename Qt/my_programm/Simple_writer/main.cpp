
#include <cstdint>
#include <stdio.h>

FILE *handle = 0;

bool add_uint8_t(uint8_t data)
{
    size_t bytes = fwrite(&data, sizeof(data), 1, handle);
    if(bytes != sizeof(data))
    {
        printf("Cannot write %d.\n", data);
        return false;
    }
    return true;
}

bool add_uint16_t(uint16_t data)
{
    size_t bytes = fwrite(&data, 1, sizeof(data), handle);
    if(bytes != sizeof(data))
    {
        printf("Cannot write %d.\n", data);
        return false;
    }
    return true;
}

bool add_uint32_t(uint32_t data)
{
    size_t bytes = fwrite(&data, 1, sizeof(data), handle);
    if(bytes != sizeof(data))
    {
        printf("Cannot write %d.\n", data);
        return false;
    }
    return true;
}

bool add_buffer(char *data, size_t len)
{
    size_t bytes = fwrite(data, 1, len, handle);
    if(bytes != len)
    {
        printf("Cannot write %s.\n", data);
        return false;
    }
    return true;
}

bool add_seek(uint32_t offset)
{
    int res = fseek(handle,  offset, SEEK_CUR);
    if(res == 0)    return true;
    return false;
}

int main(void)
{
    handle = fopen("/tmp/output.bin", "w");
    if(handle == NULL)
    {
        printf("Cannot open file.\n");
        return -1;
    }
    if(!add_uint8_t(0x00))  return -1;
    if(!add_uint8_t(0x01))  return -1;
    if(!add_uint8_t(0x02))  return -1;
    if(!add_uint8_t(0x03))  return -1;
    if(!add_uint8_t(0x04))  return -1;
    if(!add_seek(10))       return -1;

    if(!add_uint16_t(0x1234))       return -1;
    if(!add_uint32_t(0x12345678))   return -1;
    if(!add_buffer("Hello", 5))     return -1;

    fclose(handle);
    printf("OK\n");
    return 0;
}
