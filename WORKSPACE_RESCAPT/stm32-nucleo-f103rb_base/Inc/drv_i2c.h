/*
 * drv_i2c.h
 *
 *  Created on: 19 avr. 2016
 *      Author: kerhoas
 */

#ifndef INC_DRV_I2C_H_
#define INC_DRV_I2C_H_

#include "main.h"

void i2c1_Init();

// Transmit n_data bytes to i2c slave
int i2c1_WriteBuffer(uint16_t addrSlave, uint8_t *data, int n_data);

// Receive n_data bytes from i2c slave
int i2c1_ReadBuffer(uint16_t addrSlave, uint8_t *data, int n_data);

// Receive n_data bytes - located at regAddr - from i2c slave
int i2c1_ReadRegBuffer(uint16_t addrSlave, uint8_t  regAddr,  uint8_t *data, int n_data);

// Write n_data bytes - have to be written at regAddr - to i2c slave
int i2c1_WriteRegBuffer(uint16_t addrSlave, uint8_t  regAddr,  uint8_t *data, int n_data);

// Write 1 byte at regAddr Slave - Interrupt Method
void i2c1_WriteRegByte_IT(uint16_t addrSlave, uint8_t  regAddr,  uint8_t data);

// Read 1 byte from regAddr Slave - Interrupt Method
void i2c1_ReadRegBuffer_IT(uint16_t addrSlave, uint8_t  regAddr,  uint8_t* datas, int len);

// Write 1 byte to regAddr (16 bits) Slave
int i2c1_WriteReg16Byte(uint16_t addrSlave, uint16_t  regAddr,  uint8_t data);

// Write 16 bits word to regAddr (16 bits) Slave
int i2c1_WriteReg16Word16(uint16_t addrSlave, uint16_t  regAddr,  uint16_t data);

// Write 32 bits word to regAddr (16 bits) Slave
int i2c1_WriteReg16Word32(uint16_t addrSlave, uint16_t  regAddr,  uint32_t data);

// Read 1 byte from regAddr (16 bits) Slave
int i2c1_ReadReg16Byte(uint16_t addrSlave, uint16_t  regAddr, uint8_t *data);

// Read 16 bits word from regAddr (16 bits) Slave
int i2c1_ReadReg16Word16(uint16_t addrSlave, uint16_t  regAddr, uint16_t *data);

// Read 32 bits word from regAddr (16 bits) Slave
int i2c1_ReadReg16Word32(uint16_t addrSlave, uint16_t  regAddr, uint32_t *data);

// Read n_data bytes from regAddr (16 bits) Slave
int i2c1_ReadReg16Buffer(uint16_t addrSlave, uint16_t  regAddr,  uint8_t *data, int n_data);


#endif /* INC_DRV_I2C_H_ */
