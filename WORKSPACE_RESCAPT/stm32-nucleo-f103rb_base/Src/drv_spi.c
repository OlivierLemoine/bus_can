/*
 * drv_spi.c
 */


#include "drv_spi.h"

SPI_HandleTypeDef SpiHandle;

//=================================================================
//	SPI INIT
//=================================================================

void spi1_Init()
{
	SpiHandle.Instance               = SPI1;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE; //SPI_PHASE_2EDGE
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_HARD_OUTPUT ;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;
	SpiHandle.Init.Mode = SPI_MODE_MASTER;

	HAL_SPI_Init(&SpiHandle);

	__HAL_SPI_ENABLE(&SpiHandle);
}

//=================================================================
//	SPI TRANSFER
//	Send OUT on MOSI, return data from MISO
//=================================================================

uint8_t spi1_Transfer(uint8_t out)
{
uint8_t aTxBuffer[1];
uint8_t aRxBuffer[1];

aTxBuffer[0]=out;
//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, 1, 100);
//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
return aRxBuffer[0];
}
//=========================================================================
