/*
 * systemClock.c
 *
 *  Created on: 31 mars 2016
 *      Author: kerhoas
 */

#include "systemClock.h"


void systemClock_Config(void)
{
	 RCC_OscInitTypeDef RCC_OscInitStruct;
	 RCC_ClkInitTypeDef RCC_ClkInitStruct;

	 RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	 RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	 RCC_OscInitStruct.HSICalibrationValue = 16;
	 RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	 RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	 RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	 HAL_RCC_OscConfig(&RCC_OscInitStruct);

	 RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
	 RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	 RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	 RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	 RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	 HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

	 HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	 HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	  /* SysTick_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}
