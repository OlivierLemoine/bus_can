#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "systemClock.h"
#include "can_bus_types.h"
#include "drv_can.h"
#include "drv_uart.h"
#include "drv_spi.h"
#include "drv_i2c.h"

#include "config.h"
#include "tickTimer.h"

#include "anemo.h"
#include "mpu9250.h"


#include "vl6180x_api.h"
#include "vl6180x_lib.h"
#include "x-nucleo-6180xa1.h"
#include "types.h"


















/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
