#include "st7735.h"

extern SPI_HandleTypeDef hspi1;

void ST7735_SendByte(uint8_t data)
{
  while((SPI1->SR & SPI_SR_TXE) == RESET);
  SPI1->DR = data;
}

void ST7735_WaitLastData()
{
  while((SPI1->SR & SPI_SR_TXE) == RESET);
  while((SPI1->SR & SPI_SR_BSY) != RESET);
}

void ST7735_SendCommand(uint8_t data)
{
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_RESET);
  ST7735_SendByte(data);
  ST7735_WaitLastData();
}

void ST7735_SendData(uint8_t data)
{
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);
  ST7735_SendByte(data);
  ST7735_WaitLastData();
}

void ST7735_SendDataMultiple(uint8_t *data, uint32_t num)
{
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);

  for (uint32_t i = 0; i < num; i++)
  {
    ST7735_SendByte(*data);
    data++;
  }

  ST7735_WaitLastData();
}

void ST7735_Init()
{
  SPI1->CR1 |= SPI_CR1_SPE;
  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);

  ST7735_SendCommand(ST7735_SWRESET);
  HAL_Delay(150);

  ST7735_SendCommand(ST7735_SLPOUT);
  HAL_Delay(500);

  ST7735_SendCommand(ST7735_FRMCTR1);
  ST7735_SendData(0x01);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x2D);

  ST7735_SendCommand(ST7735_FRMCTR2);
  ST7735_SendData(0x01);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x2D);

  ST7735_SendCommand(ST7735_FRMCTR3);
  ST7735_SendData(0x01);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x2D);
  ST7735_SendData(0x01);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x2D);

  ST7735_SendCommand(ST7735_INVCTR);
  ST7735_SendData(0x07);

  ST7735_SendCommand(ST7735_PWCTR1);
  ST7735_SendData(0xA2);
  ST7735_SendData(0x02);
  ST7735_SendData(0x84);

  ST7735_SendCommand(ST7735_PWCTR2);
  ST7735_SendData(0xC5);

  ST7735_SendCommand(ST7735_PWCTR3);
  ST7735_SendData(0x0A);
  ST7735_SendData(0x00);

  ST7735_SendCommand(ST7735_PWCTR4);
  ST7735_SendData(0x8A);
  ST7735_SendData(0x2A);

  ST7735_SendCommand(ST7735_PWCTR5);
  ST7735_SendData(0x8A);
  ST7735_SendData(0xEE);

  ST7735_SendCommand(ST7735_VMCTR1);
  ST7735_SendData(0x0E);

  ST7735_SendCommand(ST7735_INVON);

  ST7735_SendCommand(ST7735_MADCTL);
  ST7735_SendData(0x68);

  ST7735_SendCommand(ST7735_CASET);
  ST7735_SendData(0x00); ST7735_SendData(0x00);
  ST7735_SendData(0x00); ST7735_SendData(0x9F);

  ST7735_SendCommand(ST7735_RASET);
  ST7735_SendData(0x00); ST7735_SendData(0x00);
  ST7735_SendData(0x00); ST7735_SendData(0x7F);

  ST7735_SendCommand(ST7735_COLMOD);
  ST7735_SendData(0x05);

  ST7735_SendCommand(ST7735_GMCTRP1);
  ST7735_SendData(0x02);
  ST7735_SendData(0x1c);
  ST7735_SendData(0x07);
  ST7735_SendData(0x12);
  ST7735_SendData(0x37);
  ST7735_SendData(0x32);
  ST7735_SendData(0x29);
  ST7735_SendData(0x2d);
  ST7735_SendData(0x29);
  ST7735_SendData(0x25);
  ST7735_SendData(0x2B);
  ST7735_SendData(0x39);
  ST7735_SendData(0x00);
  ST7735_SendData(0x01);
  ST7735_SendData(0x03);
  ST7735_SendData(0x10);

  ST7735_SendCommand(ST7735_GMCTRN1);
  ST7735_SendData(0x03);
  ST7735_SendData(0x1d);
  ST7735_SendData(0x07);
  ST7735_SendData(0x06);
  ST7735_SendData(0x2E);
  ST7735_SendData(0x2C);
  ST7735_SendData(0x29);
  ST7735_SendData(0x2D);
  ST7735_SendData(0x2E);
  ST7735_SendData(0x2E);
  ST7735_SendData(0x37);
  ST7735_SendData(0x3F);
  ST7735_SendData(0x00);
  ST7735_SendData(0x00);
  ST7735_SendData(0x02);
  ST7735_SendData(0x10);

  ST7735_SendCommand(ST7735_NORON);
  HAL_Delay(10);

  ST7735_SendCommand(ST7735_DISPON);
  HAL_Delay(100);

  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}

void ST7735_SetColAddr(uint16_t cStart, uint16_t cStop)
{
  uint8_t data[4];

  data[0] = (cStart & 0xFF00) >> 8;
  data[1] = cStart & 0x00FF;
  data[2] = (cStop & 0xFF00) >> 8;
  data[3] = cStop & 0x00FF;

  ST7735_SendCommand(ST7735_CASET);
  ST7735_SendDataMultiple(data, 4);
}

void ST7735_SetRowAddr(uint16_t rStart, uint16_t rStop)
{
  uint8_t data[4];

  data[0] = (rStart & 0xFF00) >> 8;
  data[1] = rStart & 0x00FF;
  data[2] = (rStop & 0xFF00) >> 8;
  data[3] = rStop & 0x00FF;

  ST7735_SendCommand(ST7735_RASET);
  ST7735_SendDataMultiple(data, 4);
}

void ST7735_DrawRect(uint16_t cStart, uint16_t rStart, uint16_t cStop, uint16_t rStop, uint16_t color)
{
  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
  ST7735_SetColAddr(cStart, cStop - 1);
  ST7735_SetRowAddr(rStart, rStop - 1);

  ST7735_SendCommand(ST7735_RAMWR);

  uint32_t size = (cStop - cStart) * (rStop - rStart);
  uint8_t colorBytes[2];
  colorBytes[0] = (color & 0xFF00) >> 8;
  colorBytes[1] = color & 0x00FF;

  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);

  for (uint32_t i = 0; i < size; i++)
  {
    ST7735_SendByte(colorBytes[0]);
    ST7735_SendByte(colorBytes[1]);
  }

  ST7735_WaitLastData();
  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}

void ST7735_SetBackgroundImage(uint16_t cStart, uint16_t rStart, uint16_t cStop, uint16_t rStop, uint16_t *image)
{
  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
  ST7735_SetColAddr(cStart, cStop - 1);
  ST7735_SetRowAddr(rStart, rStop - 1);

  ST7735_SendCommand(ST7735_RAMWR);

  uint32_t size = (cStop - cStart) * (rStop - rStart);
  uint8_t colorBytes[2];

  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);

  for (uint32_t i = 0; i < size; i++)
  {
    colorBytes[0] = (image[i] & 0xFF00) >> 8;
    colorBytes[1] = image[i] & 0x00FF;

    ST7735_SendByte(colorBytes[0]);
    ST7735_SendByte(colorBytes[1]);
  }

  ST7735_WaitLastData();
  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ST7735_X_SIZE || y >= ST7735_Y_SIZE) return;
    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
    ST7735_SetColAddr(x, x);
    ST7735_SetRowAddr(y, y);
    ST7735_SendCommand(ST7735_RAMWR);
    uint8_t data[2] = { (color >> 8) & 0xFF, color & 0xFF };
    ST7735_SendDataMultiple(data, 2);
    ST7735_WaitLastData();
    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}

void ST7735_DrawChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    if (ch < 32 || ch > 126) ch = 32;
    uint32_t index = (ch - 32) * font.height;

    if (x + font.width > ST7735_X_SIZE || y + font.height > ST7735_Y_SIZE) return;

    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
    ST7735_SetColAddr(x, x + font.width - 1);
    ST7735_SetRowAddr(y, y + font.height - 1);
    ST7735_SendCommand(ST7735_RAMWR);

    HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);

    for (uint8_t row = 0; row < font.height; row++) {
        uint16_t line = font.data[index + row];
        for (uint8_t col = 0; col < font.width; col++)
        {
            uint16_t pixel = (line & (0x8000 >> col)) ? color : bgcolor;
            ST7735_SendByte((pixel >> 8) & 0xFF);
            ST7735_SendByte(pixel & 0xFF);
        }
    }

    ST7735_WaitLastData();
    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
}

void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    while (*str) {
        ST7735_DrawChar(x, y, *str++, font, color, bgcolor);
        x += font.width;
        if (x + font.width > ST7735_X_SIZE) {
            x = 0;
            y += font.height;
            if (y + font.height > ST7735_Y_SIZE) break;
        }
    }
}

void ST7735_DrawSmallChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bgcolor) {
    const uint8_t *glyph = NULL;

    static const uint8_t font5x7_data[] = {
        // 0 (48)
        0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70,
        // 1 (49)
        0x20, 0x60, 0x20, 0x20, 0x20, 0x20, 0x70,
        // 2 (50)
        0x70, 0x88, 0x08, 0x10, 0x20, 0x40, 0xF8,
        // 3 (51)
        0x70, 0x88, 0x08, 0x30, 0x08, 0x88, 0x70,
        // 4 (52)
        0x10, 0x30, 0x50, 0x90, 0xF8, 0x10, 0x10,
        // 5 (53)
        0xF8, 0x80, 0x80, 0xF0, 0x08, 0x88, 0x70,
        // 6 (54)
        0x70, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x70,
        // 7 (55)
        0xF8, 0x08, 0x10, 0x20, 0x40, 0x40, 0x40,
        // 8 (56)
        0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70,
        // 9 (57)
        0x70, 0x88, 0x88, 0x78, 0x08, 0x08, 0x70,
        // C (67) - 'C'
        0x70, 0x88, 0x80, 0x80, 0x80, 0x88, 0x70,
        // H (72) - 'H'
        0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88,
        // T (84) - 'T'
        0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        // % (37) - '%'
        //0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // ° (код 248)
        0x20, 0x50, 0x20, 0x00, 0x00, 0x00, 0x00,
    };

    const char symbols[] = "0123456789CHT%\xF8";
    uint8_t index = 255;
    for (uint8_t i = 0; i < sizeof(symbols)-1; i++) {
        if (ch == symbols[i]) { index = i; break; }
    }
    if (index == 255) {
        ST7735_DrawRect(x, y, x + 5, y + 7, bgcolor);
        return;
    }
    glyph = &font5x7_data[index * 7];

    for (uint8_t row = 0; row < 7; row++) {
        uint8_t line = glyph[row];
        for (uint8_t col = 0; col < 5; col++) {
            if (line & (0x80 >> col))
                ST7735_DrawPixel(x + col, y + row, color);
            else
                ST7735_DrawPixel(x + col, y + row, bgcolor);
        }
    }
}

void ST7735_DrawSmallString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bgcolor) {
    while (*str) {
        ST7735_DrawSmallChar(x, y, *str++, color, bgcolor);
        x += 6;
    }
}

void DrawVBar(uint16_t cx, uint16_t cy, uint8_t width, uint8_t height,
              uint8_t value, uint8_t max_value, uint16_t color, const char* label)
{
    ST7735_DrawRect(cx - 2, cy - 2, cx + width + 3, cy + height + 12, ST7735_COLOR_BLACK);
    ST7735_DrawRect(cx - 1, cy - 1, cx + width + 2, cy + height + 2, 0x738E);

    uint8_t fill = (value * height) / max_value;
    if (fill > height) fill = height;
    if (fill > 0) {
        ST7735_DrawRect(cx, cy + height - fill, cx + width, cy + height, color);
    }

    char buf[6];
    sprintf(buf, "%s%d", label, value);

    ST7735_DrawSmallString(cx - 4, cy + height + 3, buf, color, ST7735_COLOR_BLACK);
}
