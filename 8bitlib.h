#include "fontx.h"

typedef struct {
  uint8_t rst;
  uint8_t cs;
  uint8_t rs;
  uint8_t wr;
  uint8_t rd;
  uint8_t d0;
  uint8_t d1;
  uint8_t d2;
  uint8_t d3;
  uint8_t d4;
  uint8_t d5;
  uint8_t d6;
  uint8_t d7;
} TFTPin;

#define RED             0xf800
#define GREEN           0x07e0
#define BLUE            0x001f
#define BLACK           0x0000
#define WHITE           0xffff
#define GRAY            0x8c51
#define YELLOW          0xFFE0
#define CYAN            0x07FF
#define PURPLE          0xF81F

#define DIRECTION0      0
#define DIRECTION90     1
#define DIRECTION180    2
#define DIRECTION270    3

void lcdWriteByte(uint8_t data);
void lcdWriteDataWord(uint16_t data);
void lcdWriteDataByte(uint8_t data);
void lcdWriteCommandWord(uint16_t command);
void lcdWriteCommandByte(uint8_t command);
void lcdWriteRegisterWord(uint16_t addr, uint16_t data);
void lcdWriteRegisterByte(uint8_t addr, uint16_t data);
void init_table(const void *table, int16_t size);
void lcdInit(uint16_t model, uint16_t width, uint16_t height, TFTPin pin);
void lcdReset(void);
void lcdSetup(void);
void lcdDrawPixel(uint16_t x, uint16_t y, uint16_t color);
void lcdDrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDisplayOff(void);
void lcdDisplayOn(void);
void lcdFillScreen(uint16_t color);
void lcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcdDrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void lcdDrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color);
void lcdDrawArrow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color);
void lcdDrawFillArrow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color);
uint16_t rgb565_conv(uint16_t r,uint16_t g,uint16_t b);
int lcdDrawSJISChar(FontxFile *fx, uint16_t x,uint16_t y,uint16_t sjis,uint16_t color);
int lcdDrawUTF8Char(FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utf8,uint16_t color);
int lcdDrawUTF8String(FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utfs,uint16_t color);
void lcdSetFontDirection(uint16_t);
void lcdSetFontFill(uint16_t color);
void lcdUnsetFontFill(void);
void lcdSetFontUnderLine(uint16_t color);
void lcdUnsetFontUnderLine(void);
