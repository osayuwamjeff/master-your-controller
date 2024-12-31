#include "main.h"
#include "flip_font.h"
#include "ssd1306.h"
#include <stdio.h>
#include <stdbool.h>

#define FRAME_0		0u
#define FRAME_1		1u
#define FRAME_2 	2u
#define FRAME_3 	3u


I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;



void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);


int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  SSD1306_Init();

  RTC_TimeTypeDef sTime = {0};
  char oldTimeStr[3];
  char newTimeStr[3];
  uint8_t prevMin = 0, prevHour = 0, minFrame = 0,hourFrame = 0;
  SSD1306_DrawFilledRectangle(1, 2, 60, 60, SSD1306_COLOR_BLUE);
  SSD1306_DrawFilledRectangle(67, 2, 60, 60, SSD1306_COLOR_BLUE);


  while (1)
  {
	  //Obtain time from RTC
	  HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BCD);
	  /*sprintf(hour,"%X",sTime.Hours);
	  sprintf(min,"%X",sTime.Minutes);
	  SSD1306_GotoXY(3,14);
	  SSD1306_Puts(hour, &Font_24x32, SSD1306_COLOR_BLACK);
	  SSD1306_GotoXY(68,14);
	  SSD1306_customPutc(min[0], &Font_24x32, SCALED_BY_TWO, PRINT_BOTH, SSD1306_COLOR_BLACK);
	  //SSD1306_Puts(min, &Font_24x32, SSD1306_COLOR_BLACK);*/


	  //check if current time is equal to previous time

	  //For minute
	  if(sTime.Minutes != prevMin)
	  {
		  //flip animation
		  if(minFrame == FRAME_0){ //frame 0
		  	  //draw top half of new item
			  sprintf(newTimeStr,"%02X",sTime.Minutes);
			  SSD1306_GotoXY(68,14);
			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

		  	  //draw bottom half of old item
			  sprintf(oldTimeStr,"%02X",prevMin);
			  SSD1306_GotoXY(68,14);
			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_BOTTOM_HALF, SSD1306_COLOR_BLACK);

		  	  // draw top 1/2 scaled half of old time
			  SSD1306_GotoXY(68,14 + Font_24x32.FontHeight/(2*SCALED_BY_TWO));
			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_TWO, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

			  //increment minFrame
			  minFrame = (minFrame + 1)%4;

		  }
		  else if (minFrame == FRAME_1){ //frame 1
	  	  	  //draw top half of new item
			  sprintf(newTimeStr,"%02X",sTime.Minutes);
			  SSD1306_GotoXY(68,14);
			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

	  	  	  //draw bottom half of old item
			  sprintf(oldTimeStr,"%02X",prevMin);
			  SSD1306_GotoXY(68,14);
			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_BOTTOM_HALF, SSD1306_COLOR_BLACK);

	  	  	  // draw top 1/3 scaled half of old time
			  SSD1306_GotoXY(68,14 + 2*(Font_24x32.FontHeight/(2*SCALED_BY_THREE)));
			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_THREE, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

			  //increment minFrame
			  minFrame = (minFrame + 1)%4;

		  }
		  else if(minFrame == FRAME_2){ // frame 2
	  	  	  //draw top half of new item
			  sprintf(newTimeStr,"%02X",sTime.Minutes);
			  SSD1306_GotoXY(68,14);
			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

	  	  	  //draw bottom half of old item
			  sprintf(oldTimeStr,"%02X",prevMin);
			  SSD1306_GotoXY(68,14);
			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_BOTTOM_HALF, SSD1306_COLOR_BLACK);

	  	  	  // draw bottom 1/2 scaled half of new time
			  SSD1306_GotoXY(68,14 + Font_24x32.FontHeight/(2*SCALED_BY_TWO));
			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_TWO, PRINT_BOTTOM_HALF, SSD1306_COLOR_BLACK);

			  //increment minFrame
			  minFrame = (minFrame + 1)%4;

		  }

		  else if(minFrame == FRAME_3){
	  	  	  //draw top half of new item
	  	  	  //draw bottom half of new item
			  sprintf(newTimeStr,"%02X",sTime.Minutes);
			  SSD1306_GotoXY(68,14);
			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_BOTH, SSD1306_COLOR_BLACK);

			  //update prevMin
			  prevMin = sTime.Minutes;

			  //increment minFrame
			  minFrame = (minFrame + 1)%4;
		  }
	  }

	  //For Hour
	  if(sTime.Hours != prevHour)
	  	  {
	  		  //flip animation
	  		  if(hourFrame == FRAME_0){ //frame 0
	  		  	  //draw top half of new item
	  			  sprintf(newTimeStr,"%02X",sTime.Hours);
	  			  SSD1306_GotoXY(3,14);
	  			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

	  		  	  //draw bottom half of old item
	  			  sprintf(oldTimeStr,"%02X",prevHour);
	  			  SSD1306_GotoXY(3,14);
	  			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_BOTTOM_HALF, SSD1306_COLOR_BLACK);

	  		  	  // draw top half of old time scaled 1/2
	  			  SSD1306_GotoXY(3,14 + Font_24x32.FontHeight/(2*SCALED_BY_TWO));
	  			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_TWO, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

				  //increment hourFrame
				  hourFrame = (hourFrame + 1)%4;

	  		  }
	  		  else if (hourFrame == FRAME_1){ //frame 1
	  	  	  	  //draw top half of new item
	  			  sprintf(newTimeStr,"%02X",sTime.Hours);
	  			  SSD1306_GotoXY(3,14);
	  			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

	  	  	  	  //draw bottom half of old item
	  			  sprintf(oldTimeStr,"%02X",prevHour);
	  			  SSD1306_GotoXY(3,14);
	  			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_BOTTOM_HALF, SSD1306_COLOR_BLACK);

	  	  	  	  // draw top half of old time scaled 1/3
	  			  SSD1306_GotoXY(3,14 + 2*(Font_24x32.FontHeight/(2*SCALED_BY_THREE)));
	  			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_THREE, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

				  //increment hourFrame
				  hourFrame = (hourFrame + 1)%4;

	  		  }
	  		  else if(hourFrame == FRAME_2){ // frame 2
	  	  	  	  //draw top half of new item
	  			  sprintf(newTimeStr,"%02X",sTime.Hours);
	  			  SSD1306_GotoXY(3,14);
	  			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_TOP_HALF, SSD1306_COLOR_BLACK);

	  	  	  	  //draw bottom half of old item
	  			  sprintf(oldTimeStr,"%02X",prevHour);
	  			  SSD1306_GotoXY(3,14);
	  			  SSD1306_customPuts(oldTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_BOTTOM_HALF, SSD1306_COLOR_BLACK);

	  	  	  	  // draw bottom half of new time scaled 1/2
	  			  SSD1306_GotoXY(3,14 + Font_24x32.FontHeight/(2*SCALED_BY_TWO));
	  			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_TWO, PRINT_BOTTOM_HALF, SSD1306_COLOR_BLACK);

				  //increment hourFrame
				  hourFrame = (hourFrame + 1)%4;

	  		  }

	  		  else if(hourFrame == FRAME_3){
	  	  	  	  //draw top half of new item
	  	  	  	  //draw bottom half of new item
	  			  sprintf(newTimeStr,"%02X",sTime.Hours);
	  			  SSD1306_GotoXY(3,14);
	  			  SSD1306_customPuts(newTimeStr, &Font_24x32, SCALED_BY_ONE, PRINT_BOTH, SSD1306_COLOR_BLACK);

				  //update prevHour
				  prevHour = sTime.Hours;

				  //increment hourFrame
				  hourFrame = (hourFrame + 1)%4;
	  		  }
	  	  }
	  //update Screen
	  SSD1306_DrawLine(0, 31, 127, 31, SSD1306_COLOR_BLACK);
	  SSD1306_UpdateScreen();
	  HAL_Delay(100);

  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x18;
  sTime.Minutes = 0x59;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_SATURDAY;
  DateToUpdate.Month = RTC_MONTH_MAY;
  DateToUpdate.Date = 0x18;
  DateToUpdate.Year = 0x24;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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
