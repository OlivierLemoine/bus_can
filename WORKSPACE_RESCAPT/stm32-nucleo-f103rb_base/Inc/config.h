/*
 * config.h
 *
 *  Created on: 19 avr. 2016
 *      Author: kerhoas
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_


//=====================================================
//		I2C
//=====================================================
#define I2C1_SPEED			60000

//=====================================================
//		X-NUCLEO-6180XA1 board
//=====================================================
#define EXPANDER_I2C_ADDRESS    (0x42<<1)
#define VL6180x_I2C_ADDRESS   0x52    // what we use as "API device

//=====================================================
// 		IMU MPU9250
//=====================================================
#define    MPU9250_ADDRESS	(0x68<<1)
#define    AK8963_ADDRESS   (0x0C<<1)

#define    TE				10			// Period in ms

#define 	USE_MAGNETOMETER	0

//=====================================================
//		UART2 ST-LINK
//=====================================================
#define UART_BAUDRATE	115200

//=====================================================
//		CAN BUS
//=====================================================
#define CAN_BAUDRATE	500000

#endif /* INC_CONFIG_H_ */
