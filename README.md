# STM32_DHT11_TempHum 🌡️💧
<img width="1280" height="960" alt="DHT11" src="https://github.com/user-attachments/assets/b2889851-9771-4561-be23-85b1d5e08cba" />
Compact weather station based on STM32F401 and DHT11 sensor. Displays temperature and humidity on ST7735S TFT with animated bars and error diagnostics.

## Description 🗨
A compact indoor weather station. Every 2 seconds the MCU sends a start pulse to the DHT11, receives 40 bits of data, and updates the screen.  
The display is divided by a white separator: on the left – current temperature and humidity values, on the right – animated bar indicators. In case of communication failure, a red error screen shows the error code and raw bytes.

## Hardware
- **STM32F401RCT6**  
- **ST7735S** TFT display   
- **DHT11** temperature and humidity sensor  
- **Breadboard**

## Schematic ❗
<img width="622" height="627" alt="DHT11_SCHEMATIC" src="https://github.com/user-attachments/assets/7d1805cb-8825-4cc7-88de-8b922aa63075" />

## How it works 💬
1. After reset, `main()` initialises the display, draws a permanent white frame with a separator, and turns on the backlight.
2. Inside `while(1)`:
   - The MCU sends a **20 ms low pulse** on PB10, then releases the line.
   - `DHT11_ReadData()` waits for the sensor response, reads 40 bits with microsecond delays, and verifies the checksum.
   - On success, the left part of the screen is updated with `Temp:xx C` and `Hum:xx %`, and two vertical bars are drawn (red for temperature, cyan for humidity) with compact 5×7 labels underneath.
   - The bars are **animated** – their height changes proportionally to the measured value every 2 seconds, because the whole indicator area is redrawn each cycle.
3. If any error occurs, the whole displays:
   - `DHT Error`
   - Error code (1‑6)
   - Raw bytes (HEX)
   - Short error description (e.g., `No response`, `CRC error`)

## Author
👾 **SergeyKisa228** 👾
