
//--------------------------------------------------------------------------------------------------------------
#define LCD_COLOR_BLACK		0x0000
#define LCD_COLOR_WHITE		0xffff
#define LCD_COLOR_GRAY		0x8410

#define LCD_COLOR_BLUE		0x001f
#define LCD_COLOR_GREEN		0x07e0
#define LCD_COLOR_RED		0xf800
	
#define LCD_COLOR_LIGHTRED	0xfc10

#define LCD_COLOR_YELLOW	0xffe0
#define LCD_COLOR_MAGENTA	0xf81f
#define LCD_COLOR_CYAN		0x07ff

#define bool unsigned char
#define true 	1
#define false 	0

#define LCD_WIDTH	320
#define LCD_HEIGHT	240

#include "../font/common.h"

extern void LCD_Init(void);
extern void LCD_Clear(u16 bgcolor);
extern void LCD_WriteRAM(u16 color);
extern void LCD_ConSetColor(u16 bgcolor, u16 fgcolor);
extern void LCD_ConSetPos(unsigned int row, unsigned int col);
extern void LCD_DrawRect(int x, int y, int w, int h, u16 color);
extern void LCD_DrawLine(int x0, int y0, int x1, int y1, u16 color);
extern void LCD_SetPixel(unsigned int x, unsigned int y, u16 color);
extern void LCD_SetCursor(unsigned int x, unsigned int y);
extern void LCD_Printf(const char *fmt, ...);
extern void LCD_DrawChar(unsigned int row, unsigned int col, char c, u16 bgcolor, u16 fgcolor);
extern void LCD_SetTestPattern(void);

extern void LCD_GetParam(tFont *font, int code, int *w, int *h);
extern void LCD_SetColorBigChar(u16 bgcolor, u16 fgcolor);
extern void LCD_SetCurrentBigFont(tFont *new_font);
extern void LCD_DrawBigChar(int pos_x, int pos_y, long int code, int *w, int *h);
extern void LCD_BigPrintf(const char *fmt, ...);
extern void LCD_BigPrintXY(int pos_x, int pos_y);

extern void LCD_DrawGrid(u16 step, u16 color);

extern void LCD_DrawPicture(int x, int y, int w, int h, const uint16_t *data);

extern void LCDBUS_WriteReg(u16 regn, u16 val);

extern void draw_chessboard(void);
extern void test_dma(void);
extern void lcd_test(int code);
//--------------------------------------------------------------------------------------------------------------
