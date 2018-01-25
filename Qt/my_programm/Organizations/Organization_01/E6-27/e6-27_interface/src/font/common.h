#ifndef DIGITS_H
#define DIGITS_H

#include <stdint.h>

typedef struct {
  const uint16_t *data;
  uint16_t width;
  uint16_t height;
} tImage;

typedef struct {
  long int code;
  const tImage *image;
} tChar;

typedef struct {
  int length;
  const tChar *chars;
} tFont;

extern const tFont Digits_72;
extern const tFont Digits_48;
extern const tFont lDigits_48;
extern const tFont iDigits_48;
extern const tFont Digits_36;
extern const tFont Digits_24;
extern const tFont Digits_14;

extern const tFont Big_Font;
#endif
