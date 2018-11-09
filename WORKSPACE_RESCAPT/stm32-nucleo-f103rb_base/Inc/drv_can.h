/*
 * can_bus.h
 *
 *  Created on: 10 sept. 2015
 *      Author: kerhoas
 */

#ifndef INC_DRV_CAN_H_
#define INC_DRV_CAN_H_

#include "main.h"
#include "can_bus_types.h"

//********************************************************************



//********************************************************************

typedef enum {
    IRQ_RX,
    IRQ_TX,
    IRQ_ERROR,
    IRQ_OVERRUN,
    IRQ_WAKEUP,
    IRQ_PASSIVE,
    IRQ_ARB,
    IRQ_BUS,
    IRQ_READY
} CanIrqType;


typedef enum {
    MODE_RESET,
    MODE_NORMAL,
    MODE_SILENT,
    MODE_TEST_GLOBAL,
    MODE_TEST_LOCAL,
    MODE_TEST_SILENT
} CanMode;

typedef struct can_s can_t;

void          can_Init(void);
int           can_SetFreq(int hz);

void          can_IrqInit(void);
void          can_IrqFree(void);
void          can_IrqSet(void (*fptr)(void));

int           can_Write(CAN_Message);
int           can_Read(CAN_Message *msg);
int           can_Mode(CanMode mode);
int 		  can_Filter_list(uint32_t id1_id2, uint32_t id3_id4, CANFormat format /*=CANAny*/, int32_t handle /*=0*/ );
int 		  can_Filter_disable();
void          can_Reset(void);
unsigned char can_Rderror(void);
unsigned char can_Tderror(void);


#endif /* INC_DRV_CAN_H_ */
