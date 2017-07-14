
int PutChar(char *pStr, char c);
int PutUnsignedInt(
    char *pStr,
    char fill,
    int width,
    unsigned int value);

int PutHexa(
    char *pStr,
    char fill,
    int width,
    unsigned char maj,
    unsigned int value);

int PutString(char *pStr, const char *pSource);

int PutSignedInt(
    char *pStr,
    char fill,
    int width,
    int value);

int vsnprintf(char *pStr, size_t length, const char *pFormat, va_list ap);
int vsprintf(char *pString, const char *pFormat, va_list ap);
