/*
 * vl6180x_appcfg.h
 *
 *  Created on: 5 avr. 2016
 *      Author: kerhoas
 */

#ifndef INC_VL6180X_APPCFG_H_
#define INC_VL6180X_APPCFG_H_


/**
 * @def VL6180x_SINGLE_DEVICE_DRIVER
 * @brief enable lightweight single vl6180x device driver
 *
 * value 1 =>  single device capable
 * Configure optimized APi for single device driver with static data and minimal use of ref pointer \n
 * limited to single device driver or application in non multi thread/core environment \n
 *
 * value 0 =>  multiple device capable user must review "device" structure and type in porting files
 * @ingroup Configuration
 */
#define VL6180x_SINGLE_DEVICE_DRIVER 1

/**
 * @def VL6180x_RANGE_STATUS_ERRSTRING
 * @brief when define include range status Error string and related
 *
 * The string table lookup require some space in read only area
 * @ingroup Configuration
 */
#define VL6180x_RANGE_STATUS_ERRSTRING  1

/**
 * @def VL6180X_SAFE_POLLING_ENTER
 *
 * @brief Ensure safe polling method when set
 *
 * Polling for a condition can be hazardous and result in infinite looping if any previous interrupt status
 * condition is not cleared. \n
 * Setting these flags enforce error clearing on start of polling method to avoid it.
 * the drawback are : \n
 * @li extra use-less i2c bus usage and traffic
 * @li potentially slower measure rate.
 * If application ensure interrupt get clear on mode or interrupt configuration change
 * then keep option disabled. \n
 * To be safe set these option to 1
 * @ingroup Configurationf
 */
#define VL6180X_SAFE_POLLING_ENTER  0


/**
 * @brief Enable function start/end logging
 *
 * requires porting  @a #LOG_FUNCTION_START @a #LOG_FUNCTION_END @a #LOG_FUNCTION_END_FMT
 * @ingroup Configuration
 */
#define VL6180X_LOG_ENABLE  0


#endif /* INC_VL6180X_APPCFG_H_ */
