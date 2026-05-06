/* USER CODE BEGIN Header */
/*
  DHT11_TempHum
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t dht11_raw[5];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_mk(uint32_t mk)
{
	uint32_t mk_count = mk * (SystemCoreClock / 1000000U);

	DWT->CYCCNT = 0U;

	while(DWT->CYCCNT < mk_count);
}

uint8_t DHT11_ReadData(uint8_t *hum, uint8_t *temp)
{
    uint8_t buf[5] = {0};
    uint8_t byte_cnt, bit_cnt;
    uint16_t timeout;

    timeout = 200;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET)
    {
        if (--timeout == 0) return 1;
        delay_mk(1);
    }

    timeout = 200;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET)
    {
        if (--timeout == 0) return 2;
        delay_mk(1);
    }

    __disable_irq();

    for (byte_cnt = 0; byte_cnt < 5; byte_cnt++)
    {
        for (bit_cnt = 0; bit_cnt < 8; bit_cnt++)
        {
            timeout = 200;
            while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET)
            {
                if (--timeout == 0)
                {
                    __enable_irq();
                    return 3;
                }
                delay_mk(1);
            }

            timeout = 200;
            while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET)
            {
                if (--timeout == 0)
                {
                    __enable_irq();
                    return 4;
                }
                delay_mk(1);
            }

            delay_mk(40);

            buf[byte_cnt] <<= 1;
            if (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET)
            {
                buf[byte_cnt] |= 1;

                timeout = 200;
                while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET)
                {
                    if (--timeout == 0)
                    {
                        __enable_irq();
                        return 5;
                    }
                    delay_mk(1);
                }
            }
        }
    }

    __enable_irq();

    for (uint8_t i = 0; i < 5; i++) {
        dht11_raw[i] = buf[i];
    }

    if (buf[4] == (uint8_t)(buf[0] + buf[1] + buf[2] + buf[3]))
    {
        *hum  = buf[0];
        *temp = buf[2];
        return 0;
    }

    return 6;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  DWT_Init();

  ST7735_Init();
  HAL_GPIO_WritePin(BLK_GPIO_Port, BLK_Pin, GPIO_PIN_SET);
  HAL_Delay(150);

  ST7735_DrawRect(0, 0, 160, 128, ST7735_COLOR_WHITE);
  ST7735_DrawRect(4, 2, 160, 126, ST7735_COLOR_BLACK);
  ST7735_DrawRect(94, 2, 96, 126, ST7735_COLOR_WHITE);
  ST7735_DrawRect(158, 2, 160, 126, ST7735_COLOR_WHITE);
  ST7735_DrawRect(2, 105, 160, 107, ST7735_COLOR_WHITE);
  ST7735_DrawRect(2, 26, 160, 28, ST7735_COLOR_WHITE);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	   HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
	   HAL_Delay(20);
	   HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);


	   delay_mk(45);

	   uint8_t humidity = 0, temperature = 0;
	   uint8_t res = DHT11_ReadData(&humidity, &temperature);

	   if (res == 0)
	   {
	     char buf[16];
	     sprintf(buf, "Hum: %d %%", humidity);
	     ST7735_DrawString(10, 45, buf, Font_8x16, ST7735_COLOR_GREEN, ST7735_COLOR_BLACK);

	     sprintf(buf, "Temp: %d C", temperature);
	     ST7735_DrawString(10, 70, buf, Font_8x16, ST7735_COLOR_YELLOW, ST7735_COLOR_BLACK);
	     DrawVBar(110, 40, 6, 50, temperature, 50, 0xF800, "T");
	     DrawVBar(135, 40, 6, 50, humidity,    100, 0x07FF, "H");
	   }
	   else
	   {
	     ST7735_DrawRect(0, 0, 158, 126, ST7735_COLOR_BLACK);
	     ST7735_DrawString(10, 10, "DHT Error", Font_8x16, ST7735_COLOR_WHITE, ST7735_COLOR_RED);

	     char err[16];
	     sprintf(err, "Code: %d", res);
	     ST7735_DrawString(10, 30, err, Font_8x16, ST7735_COLOR_WHITE, ST7735_COLOR_RED);

	     char raw_str[32];
	     sprintf(raw_str, "%02X %02X %02X %02X %02X",
	             dht11_raw[0], dht11_raw[1], dht11_raw[2],
	             dht11_raw[3], dht11_raw[4]);
	     ST7735_DrawString(10, 55, raw_str, Font_8x16, ST7735_COLOR_YELLOW, ST7735_COLOR_RED);

	      switch(res)
	      {
	        case 1: ST7735_DrawString(10, 90, "No response", Font_8x16, ST7735_COLOR_WHITE, ST7735_COLOR_RED); break;
	        case 2: ST7735_DrawString(10, 90, "Low stuck", Font_8x16, ST7735_COLOR_WHITE, ST7735_COLOR_RED); break;
	        case 3: ST7735_DrawString(10, 90, "Bit start fail", Font_8x16, ST7735_COLOR_WHITE, ST7735_COLOR_RED); break;
	        case 4: ST7735_DrawString(10, 90, "Low too long", Font_8x16, ST7735_COLOR_WHITE, ST7735_COLOR_RED); break;
	        case 5: ST7735_DrawString(10, 90, "High too long", Font_8x16, ST7735_COLOR_WHITE, ST7735_COLOR_RED); break;
	        case 6: ST7735_DrawString(10, 90, "CRC error", Font_8x16, ST7735_COLOR_WHITE, ST7735_COLOR_RED); break;
	        default: break;
	      }
	    }
	    HAL_Delay(2000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
