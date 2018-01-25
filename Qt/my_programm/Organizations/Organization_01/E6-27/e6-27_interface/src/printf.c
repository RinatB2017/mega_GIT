//--------------------------------------------------------------------------------------------------------------
#include <stdarg.h>
#include <stdio.h>

#include "printf.h"
//--------------------------------------------------------------------------------------------------------------
/**
 * @brief  Writes a character inside the given string. Returns 1.
 *
 * @param  pStr    Storage string.
 * @param  c    Character to write.
 */
int PutChar(char *pStr, char c)
{
    *pStr = c;
    return 1;
}

/**
 * @brief  Writes an unsigned int inside the given string, using the provided fill &
 *         width parameters.
 *
 * @param  pStr  Storage string.
 * @param  fill  Fill character.
 * @param  width  Minimum integer width.
 * @param  value  Integer value.  
 */
int PutUnsignedInt(
    char *pStr,
    char fill,
    int width,
    unsigned int value)
{
    int num = 0;

    /* Take current digit into account when calculating width */
    width--;

    /* Recursively write upper digits */
    if ((value / 10) > 0) 
    {
        num = PutUnsignedInt(pStr, fill, width, value / 10);
        pStr += num;
    }
   
    /* Write filler characters */
    else 
    {
        while (width > 0) 
	{
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
    }

    /* Write lower digit */
    num += PutChar(pStr, (value % 10) + '0');

    return num;
}
//--------------------------------------------------------------------------------------------------------------
/**
 * @brief  Writes an hexadecimal value into a string, using the given fill, width &
 *         capital parameters.
 *
 * @param pStr   Storage string.
 * @param fill   Fill character.
 * @param width  Minimum integer width.
 * @param maj    Indicates if the letters must be printed in lower- or upper-case.
 * @param value  Hexadecimal value.
 *
 * @return  The number of char written
 */
int PutHexa(
    char *pStr,
    char fill,
    int width,
    unsigned char maj,
    unsigned int value)
{
    int num = 0;

    /* Decrement width */
    width--;

    /* Recursively output upper digits */
    if ((value >> 4) > 0) 
    {
        num += PutHexa(pStr, fill, width, maj, value >> 4);
        pStr += num;
    }
    /* Write filler chars */
    else 
    {
        while (width > 0) 
	{
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
    }

    /* Write current digit */
    if ((value & 0xF) < 10) 
    {
        PutChar(pStr, (value & 0xF) + '0');
    }
    else if (maj) {
        PutChar(pStr, (value & 0xF) - 10 + 'A');
    }
    else 
    {
        PutChar(pStr, (value & 0xF) - 10 + 'a');
    }
    num++;

    return num;
}
//--------------------------------------------------------------------------------------------------------------
/**
 * @brief  Writes a string inside the given string.
 *
 * @param  pStr     Storage string.
 * @param  pSource  Source string.
 * @return  The size of the written
 */
int PutString(char *pStr, const char *pSource)
{
    int num = 0;

    while (*pSource != 0) 
    {
        *pStr++ = *pSource++;
        num++;
    }

    return num;
}
//--------------------------------------------------------------------------------------------------------------
/**
 * @brief  Writes a signed int inside the given string, using the provided fill & width
 *         parameters.
 *
 * @param pStr   Storage string.
 * @param fill   Fill character.
 * @param width  Minimum integer width.
 * @param value  Signed integer value.
 */
int PutSignedInt(
    char *pStr,
    char fill,
    int width,
    int value)
{
    int num = 0;
    unsigned int absolute;

    /* Compute absolute value */
    if (value < 0) 
    {
        absolute = -value;
    }
    else 
    {
        absolute = value;
    }

    /* Take current digit into account when calculating width */
    width--;

    /* Recursively write upper digits */
    if ((absolute / 10) > 0)
    {
        if (value < 0) 
	{
            num = PutSignedInt(pStr, fill, width, -(absolute / 10));
        }
        else 
	{
            num = PutSignedInt(pStr, fill, width, absolute / 10);
        }
        pStr += num;
    }
    else 
    {
        /* Reserve space for sign */
        if (value < 0) 
	{
            width--;
        }

        /* Write filler characters */
        while (width > 0) 
	{
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }

        /* Write sign */
        if (value < 0) 
	{
            num += PutChar(pStr, '-');
            pStr++;
        }
    }

    /* Write lower digit */
    num += PutChar(pStr, (absolute % 10) + '0');

    return num;
}
//--------------------------------------------------------------------------------------------------------------
/**
 * @brief  Stores the result of a formatted string into another string. Format
 *         arguments are given in a va_list instance.
 *
 * @param pStr    Destination string.
 * @param length  Length of Destination string.
 * @param pFormat Format string.
 * @param ap      Argument list.
 *
 * @return  The number of characters written.
 */
int vsnprintf(char *pStr, size_t length, const char *pFormat, va_list ap)
{
    char   fill;
    unsigned char width;
    int    num = 0;
    int    size = 0;

    /* Clear the string */
    if (pStr) 
    {
        *pStr = 0;
    }

    /* Phase string */
    while (*pFormat != 0 && size < length) 
    {
        /* Normal character */
        if (*pFormat != '%') 
	{
            *pStr++ = *pFormat++;
            size++;
        }
        /* Escaped '%' */
        else if (*(pFormat+1) == '%') 
	{
            *pStr++ = '%';
            pFormat += 2;
            size++;
        }
        /* Token delimiter */
        else 
	{
            fill = ' ';
            width = 0;
            pFormat++;

            /* Parse filler */
            if (*pFormat == '0') 
	    {
                fill = '0';
                pFormat++;
            }

            /* Parse width */
            while ((*pFormat >= '0') && (*pFormat <= '9')) 
	    {
                width = (width*10) + *pFormat-'0';
                pFormat++;
            }

            /* Check if there is enough space */
            if (size + width > length) 
	    {
                width = length - size;
            }
       
            /* Parse type */
            switch (*pFormat) {
            case 'd':
            case 'i': num = PutSignedInt(pStr, fill, width, va_arg(ap, signed int)); break;
            case 'u': num = PutUnsignedInt(pStr, fill, width, va_arg(ap, unsigned int)); break;
            case 'x': num = PutHexa(pStr, fill, width, 0, va_arg(ap, unsigned int)); break;
            case 'X': num = PutHexa(pStr, fill, width, 1, va_arg(ap, unsigned int)); break;
            case 's': num = PutString(pStr, va_arg(ap, char *)); break;
            case 'c': num = PutChar(pStr, va_arg(ap, unsigned int)); break;
            default:
                return EOF;
            }

            pFormat++;
            pStr += num;
            size += num;
        }
    }

    /* NULL-terminated (final \0 is not counted) */
    if (size < length) 
    {
        *pStr = 0;
    }
    else 
    {
        *(--pStr) = 0;
        size--;
    }

    return size;
}
//--------------------------------------------------------------------------------------------------------------
/**
 * @brief  Stores the result of a formatted string into another string. Format
 *         arguments are given in a va_list instance.
 *
 * @param pString  Destination string.
 * @param length   Length of Destination string.
 * @param pFormat  Format string.
 * @param ap       Argument list.
 *
 * @return  The number of characters written.
 */
signed int vsprintf(char *pString, const char *pFormat, va_list ap)
{
    return vsnprintf(pString, 256, pFormat, ap);
}
//--------------------------------------------------------------------------------------------------------------
