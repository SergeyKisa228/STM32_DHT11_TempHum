#ifndef ST7735_H
#define ST7735_H

#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "fonts.h"
#include "stdio.h"

#define ST7735_COLOR_RED                                                0xF800
#define ST7735_COLOR_GREEN                                              0x07E0
#define ST7735_COLOR_BLUE                                               0x001F
#define ST7735_COLOR_YELLOW                                             0xFFE0
#define ST7735_COLOR_WHITE                                              0xFFFF
#define ST7735_COLOR_BLACK                                              0x0000
#define ST7735_COLOR_ORANGE                                             0xFA20

#define ST7735_RESET_PORT                                               (GPIOA)
#define ST7735_RESET_PIN                                                (GPIO_PIN_1)

#define ST7735_CS_PORT                                                  (GPIOA)
#define ST7735_CS_PIN                                                   (GPIO_PIN_4)

#define ST7735_DC_PORT                                                  (GPIOA)
#define ST7735_DC_PIN                                                   (GPIO_PIN_2)

#define ST7735_X_SIZE                                                   160
#define ST7735_Y_SIZE                                                   128

#define ST7735_XSTART  0
#define ST7735_YSTART  0

#define ST7735_NOP                                                      0x00
#define ST7735_SWRESET                                                  0x01
#define ST7735_RDDID                                                    0x04
#define ST7735_RDDST                                                    0x09

#define ST7735_SLPIN                                                    0x10
#define ST7735_SLPOUT                                                   0x11
#define ST7735_PTLON                                                    0x12
#define ST7735_NORON                                                    0x13

#define ST7735_INVOFF                                                   0x20
#define ST7735_INVON                                                    0x21
#define ST7735_DISPOFF                                                  0x28
#define ST7735_DISPON                                                   0x29
#define ST7735_CASET                                                    0x2A
#define ST7735_RASET                                                    0x2B
#define ST7735_RAMWR                                                    0x2C
#define ST7735_RAMRD                                                    0x2E

#define ST7735_PTLAR                                                    0x30
#define ST7735_COLMOD                                                   0x3A
#define ST7735_MADCTL                                                   0x36

#define ST7735_FRMCTR1                                                  0xB1
#define ST7735_FRMCTR2                                                  0xB2
#define ST7735_FRMCTR3                                                  0xB3
#define ST7735_INVCTR                                                   0xB4
#define ST7735_DISSET5                                                  0xB6

#define ST7735_PWCTR1                                                   0xC0
#define ST7735_PWCTR2                                                   0xC1
#define ST7735_PWCTR3                                                   0xC2
#define ST7735_PWCTR4                                                   0xC3
#define ST7735_PWCTR5                                                   0xC4
#define ST7735_VMCTR1                                                   0xC5

#define ST7735_RDID1                                                    0xDA
#define ST7735_RDID2                                                    0xDB
#define ST7735_RDID3                                                    0xDC
#define ST7735_RDID4                                                    0xDD

#define ST7735_PWCTR6                                                   0xFC

#define ST7735_GMCTRP1                                                  0xE0
#define ST7735_GMCTRN1                                                  0xE1

#define ST7735_SPI_TIMEOUT                                              100

extern void ST7735_Init();
extern void ST7735_DrawRect(uint16_t cStart, uint16_t rStart, uint16_t cStop, uint16_t rStop, uint16_t color);
extern void ST7735_SetBackgroundImage(uint16_t cStart, uint16_t rStart, uint16_t cStop, uint16_t rStop, uint16_t *image);
extern void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
extern void ST7735_DrawChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
extern void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
extern void DrawVBar(uint16_t cx, uint16_t cy, uint8_t width, uint8_t height,
              uint8_t value, uint8_t max_value, uint16_t color, const char* label);

#endif
