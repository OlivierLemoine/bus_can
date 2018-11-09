#include "main.h"
#include "stm32f1xx_hal.h"

void HAL_UARTCOM_MspInit(void);


void HAL_CANBUS_MspInit(void);
void HAL_spi_MspInit(void);
void HAL_timer1_MspInit(void);
void HAL_tickTimer_MspInit(void);
void HAL_i2c1_MspInit(void);
void HAL_pushButton_MspInit(void);
void HAL_vl6180x_GPIO1_MspInit(void);

//===================================================================
void HAL_MspInit(void)
{
	HAL_UARTCOM_MspInit();
	HAL_CANBUS_MspInit();
	HAL_spi_MspInit();
	HAL_timer1_MspInit();
	HAL_tickTimer_MspInit();
	HAL_pushButton_MspInit();
	HAL_vl6180x_GPIO1_MspInit();
	HAL_i2c1_MspInit();
}

void HAL_MspDeInit(void)
{

}

//===================================================================
//			USART2 : ST-LINK virtual Port
// TX --> PA2
// RX --> PA3
//===================================================================

void HAL_UARTCOM_MspInit(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_USART2_CLK_ENABLE();

	  GPIO_InitStruct.Pin       = GPIO_PIN_2;		// USART 2 TX PIN
	  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull      = GPIO_PULLUP;
	  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin =  GPIO_PIN_3;			// USART 2 RX PIN
	  GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;

	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//===================================================================
//			GPIO BUTTON - PC13 (USER BUTTON)
// interrupt mode
//===================================================================
/*void HAL_GPIO_BUTTON_MspInit(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  GPIO_InitStruct.Pin = GPIO_PIN_13;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;

	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  Enable and set EXTI Line0 Interrupt to the lowest priority
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 6, 0);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
*/

//===================================================================
//			CAN BUS
//
// CAN_TX (Transmit data line)		Alternate function push-pull
// CAN_RX (Receive data line)		Input floating / Input pull-up
// CAN1 alternate function remapping
// RX PA11	PB8	(PD0)
// TX PA12	PB9	(PD1)
//===================================================================
void HAL_CANBUS_MspInit(void)
{
	  GPIO_InitTypeDef   GPIO_InitStruct;

	  __HAL_RCC_CAN1_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_AFIO_REMAP_CAN1_1();
	  __HAL_RCC_AFIO_CLK_ENABLE();

	  /* CAN1 TX GPIO pin configuration --> PA_12 */
	  GPIO_InitStruct.Pin =  GPIO_PIN_12;		//CANx_TX_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;

	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_11;		//CANx_RX_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;	//GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;		//GPIO_PULLUP;

	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 5, 0);
	  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

}
//===================================================================
// 			SPI 1 (Pressure Sensor MPL115A1)
// SPI1_SCK		--> PA5
// SPI1_MISO	--> PA6
// SPI1_MOSI	--> PA7
// SPI1 NSS		--> PA4
//===================================================================
void HAL_spi_MspInit(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;

	   __HAL_RCC_SPI1_CLK_ENABLE();
	   __HAL_AFIO_REMAP_SPI1_DISABLE();
	   __HAL_RCC_GPIOA_CLK_ENABLE();

	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;//GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;

	  GPIO_InitStruct.Pin = GPIO_PIN_5;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_6;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_7;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  	  GPIO_InitStruct.Pin = GPIO_PIN_4;
  	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	  GPIO_InitStruct.Pull = GPIO_PULLUP;
  	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//===================================================================
//			TIMER 1 Anemometer
// TIM_CLK		--> 	PA8 (PWM1_1)
//===================================================================
void HAL_timer1_MspInit(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;
	   __HAL_RCC_TIM1_CLK_ENABLE();
	   __HAL_RCC_GPIOA_CLK_ENABLE();

	   __HAL_AFIO_REMAP_TIM1_DISABLE();

	  GPIO_InitStruct.Pin = GPIO_PIN_8;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;

 	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//===================================================================
//			TIMER 3 TickTimer
//===================================================================
void HAL_tickTimer_MspInit(void)
{
	  __HAL_RCC_TIM3_CLK_ENABLE();
	  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
	  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
//===================================================================
//			I2C1
// PB8 : I2C1 SCL
// PB9 : I2C1 SDA
//===================================================================
void HAL_i2c1_MspInit(void)
{

	  GPIO_InitTypeDef  GPIO_InitStruct;

	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();

	  __HAL_RCC_AFIO_CLK_ENABLE();
	  __HAL_AFIO_REMAP_I2C1_ENABLE();

	  __HAL_RCC_I2C1_CLK_ENABLE();

	  GPIO_InitStruct.Pin       = GPIO_PIN_8;
	  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
	  GPIO_InitStruct.Pull      = GPIO_PULLUP;
	  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

	  HAL_GPIO_Init( GPIOB, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin       = GPIO_PIN_9;
	  HAL_GPIO_Init( GPIOB, &GPIO_InitStruct);

	  HAL_NVIC_SetPriority(I2C1_ER_IRQn, 4, 1);
	  HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	  HAL_NVIC_SetPriority(I2C1_EV_IRQn, 4, 2);
	  HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
}
//===================================================================
//			GPIO USER PUSH BUTTON
// PC13
//===================================================================
void HAL_pushButton_MspInit(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOC_CLK_ENABLE();

	  /*Configure GPIO pin : PC13 */
	  GPIO_InitStruct.Pin = GPIO_PIN_13;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
//===================================================================
//			vl6180x_GPIO1
// PB0 = INT
//===================================================================
void HAL_vl6180x_GPIO1_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 7, 1);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^END OF FILE^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
