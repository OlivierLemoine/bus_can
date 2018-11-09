/*
 * drv_i2c.c
 *
 *  Created on: 19 avr. 2016
 *      Author: kerhoas
 */


#include "drv_i2c.h"

I2C_HandleTypeDef hi2c1;

//======================================================================
// Init I2C1 Peripheral
//======================================================================
void i2c1_Init()
{
    hi2c1.Instance             = I2C1;
    hi2c1.Init.ClockSpeed      = I2C1_SPEED;		// cf config.h
    hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1     = 0x2;
    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2     = 0xFF;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&hi2c1);
}
//======================================================================
// Transmit n_data bytes to i2c slave
//======================================================================
int i2c1_WriteBuffer(uint16_t addrSlave, uint8_t *data, int n_data)
{
	int status;
	status = HAL_I2C_Master_Transmit(&hi2c1, addrSlave, data, n_data , 100);
	return status;
}
//======================================================================
// Receive n_data bytes from i2c slave
//======================================================================
int i2c1_ReadBuffer(uint16_t addrSlave, uint8_t *data, int n_data)
{
	int status;
	status = HAL_I2C_Master_Receive(&hi2c1, addrSlave, data, n_data , 100);
	return status;
}
//======================================================================
// Receive n_data bytes - located at regAddr - from i2c slave
//======================================================================
int i2c1_ReadRegBuffer(uint16_t addrSlave, uint8_t  regAddr,  uint8_t *data, int n_data)
{
    int status;
    uint8_t RegAddr;
    RegAddr=regAddr;
    do{
        status=HAL_I2C_Master_Transmit(&hi2c1, addrSlave, &RegAddr, 1, 100);
        if( status )
            break;
        status =HAL_I2C_Master_Receive(&hi2c1, addrSlave, data, n_data, n_data*100);
    }while(0);
    return status;
}

//======================================================================
// Write n_data bytes - have to be written at regAddr - to i2c slave
//======================================================================
int i2c1_WriteRegBuffer(uint16_t addrSlave, uint8_t  regAddr,  uint8_t *data, int n_data)
{
    int status;
    uint8_t RegAddr[0x10];
    RegAddr[0]=regAddr;
    memcpy(RegAddr+1, data, n_data);
    status=HAL_I2C_Master_Transmit(&hi2c1, addrSlave, RegAddr, n_data+1, 100);
    return status;
}

//======================================================================
// Write 1 byte at regAddr Slave - Interrupt Method
//======================================================================
void i2c1_WriteRegByte_IT(uint16_t addrSlave, uint8_t  regAddr,  uint8_t data)
{

	uint8_t datas_to_send[2];

	datas_to_send[0]=regAddr;
	datas_to_send[1]=data;

	while(HAL_I2C_Master_Transmit_IT(&hi2c1, addrSlave, datas_to_send, 2)!= HAL_OK){}
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY){}
}
//======================================================================
// Read 1 byte from regAddr Slave - Interrupt Method
//======================================================================
void i2c1_ReadRegBuffer_IT(uint16_t addrSlave, uint8_t  regAddr,  uint8_t* datas, int len)
{
	while(HAL_I2C_Master_Transmit_IT(&hi2c1, addrSlave, &regAddr, 1)!= HAL_OK){}
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY){}

	while(HAL_I2C_Master_Receive_IT(&hi2c1, addrSlave, datas, len)!= HAL_OK){}
	while( HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY ){}
}

//======================================================================
// Write 1 byte to regAddr (16 bits) Slave
//======================================================================
int i2c1_WriteReg16Byte(uint16_t addrSlave, uint16_t  regAddr,  uint8_t data)
{
	int status;
	uint8_t buffer[3];
	buffer[0]=regAddr>>8;
	buffer[1]=regAddr&0xFF;
	buffer[2]=data;

	status = HAL_I2C_Master_Transmit(&hi2c1, addrSlave, buffer, 3 , 100);
	return status;
}
//======================================================================
// Write 16 bits word to regAddr (16 bits) Slave
//======================================================================
int i2c1_WriteReg16Word16(uint16_t addrSlave, uint16_t  regAddr,  uint16_t data)
{
	int status;
	uint8_t buffer[4];
	buffer[0]=regAddr>>8;
	buffer[1]=regAddr&0xFF;
    buffer[2]=data>>8;
    buffer[3]=data&0xFF;

	status = HAL_I2C_Master_Transmit(&hi2c1, addrSlave, buffer, 4 , 100);
	return status;
}
//======================================================================
// Write 32 bits word to regAddr (16 bits) Slave
//======================================================================
int i2c1_WriteReg16Word32(uint16_t addrSlave, uint16_t  regAddr,  uint32_t data)
{
	int status;
	uint8_t buffer[4];
	buffer[0]=regAddr>>8;
	buffer[1]=regAddr&0xFF;
    buffer[2]=data>>24;
    buffer[3]=(data>>16)&0xFF;
    buffer[4]=(data>>8)&0xFF;;
    buffer[5]=data&0xFF;

	status = HAL_I2C_Master_Transmit(&hi2c1, addrSlave, buffer, 6 , 100);
	return status;
}
//======================================================================
// Read 1 byte from regAddr (16 bits) Slave
//======================================================================
int i2c1_ReadReg16Byte(uint16_t addrSlave, uint16_t  regAddr, uint8_t *data)
{
    int status;
    uint8_t buffer[2];

    buffer[0]=regAddr>>8;
    buffer[1]=regAddr&0xFF;

    status=HAL_I2C_Master_Transmit(&hi2c1, addrSlave, buffer, 2, 100);
     if(!status ) {
        status =HAL_I2C_Master_Receive(&hi2c1, addrSlave, buffer, 1, 100);
        if( !status ){
            *data=buffer[0];
        }
     }

    return status;
}
//======================================================================
// Read 16 bits word from regAddr (16 bits) Slave
//======================================================================
int i2c1_ReadReg16Word16(uint16_t addrSlave, uint16_t  regAddr, uint16_t *data)
{
    int status;
    uint8_t buffer[2];

    buffer[0]=regAddr>>8;
    buffer[1]=regAddr&0xFF;

    status=HAL_I2C_Master_Transmit(&hi2c1, addrSlave, buffer, 2, 100);
     if(!status ) {
        status =HAL_I2C_Master_Receive(&hi2c1, addrSlave, buffer, 2, 100);
        if( !status ){
        	 *data=((uint16_t)buffer[0]<<8)|(uint16_t)buffer[1];
        }
     }

    return status;
}
//======================================================================
// Read 32 bits word from regAddr (16 bits) Slave
//======================================================================
int i2c1_ReadReg16Word32(uint16_t addrSlave, uint16_t  regAddr, uint32_t *data)
{
    int status;
    uint8_t buffer[4];

    buffer[0]=regAddr>>8;
    buffer[1]=regAddr&0xFF;

    status=HAL_I2C_Master_Transmit(&hi2c1, addrSlave, buffer, 2, 100);
     if(!status ) {
        status =HAL_I2C_Master_Receive(&hi2c1, addrSlave, buffer, 4, 100);
        if( !status ){
           	 *data=((uint32_t)buffer[0]<<24)|((uint32_t)buffer[1]<<16)|((uint32_t)buffer[2]<<8)|((uint32_t)buffer[3]);
        }
     }

    return status;
}

//======================================================================
// Read n_data bytes from regAddr (16 bits) Slave
//======================================================================
int i2c1_ReadReg16Buffer(uint16_t addrSlave, uint16_t  regAddr,  uint8_t *data, int n_data)
{
    int status;
    uint8_t buffer[2];

    buffer[0]=regAddr>>8;
    buffer[1]=regAddr&0xFF;

    status=HAL_I2C_Master_Transmit(&hi2c1, addrSlave, buffer, 2, 100);
    if( !status ){
        status=HAL_I2C_Master_Receive(&hi2c1, addrSlave, data, n_data, n_data*100);
    }

    return status;
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ END OF FILE ^^^^^^^^^^^^^^^^^^^^^^^^
