//---------------------------------------------------------------
#include "TFTLCD.h"

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0 
// optional
#define LCD_RESET A4

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
//---------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  Serial.println("Paint!");
  tft.initDisplay(); 
}
//---------------------------------------------------------------
void loop()
{
  //  tft.fillScreen(RED);
  //tft.fillScreen(GREEN);
  //  tft.fillScreen(BLUE);
  //  tft.fillScreen(CYAN);
  //  tft.fillScreen(MAGENTA);
  //  tft.fillScreen(YELLOW);
  tft.fillScreen(WHITE);

  int w = tft.width();
  int h = tft.height();

  int max_x = 8;
  int max_y = 16;

  int size_x = w / max_x;
  int size_y = h / max_y;

  char flag = 0;
  for(int y=0; y<max_y; y++)
  {
    //tft.drawVerticalLine(0, y*size_y, w, RED);
    for(int x=0; x<max_x; x++)
    {
      //tft.drawHorizontalLine(x*size_x, 0, h, RED);

      flag = !flag;
      int color = 0;
      if(flag)
      {
        color = RED;
      }
      else
      {
        color = BLUE;
      }
      tft.fillRect(x*size_x, y*size_y, size_x, size_y, color);
    }
    flag = !flag;
  }

  while(1)
  {
  }
}
//---------------------------------------------------------------




