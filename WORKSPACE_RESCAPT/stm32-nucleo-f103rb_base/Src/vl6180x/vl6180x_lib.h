/*
 * vl6180x_lib.h
 *
 *  Created on: 5 avr. 2016
 *      Author: kerhoas
 */

#ifndef SRC_VL6180X_VL6180X_LIB_H_
#define SRC_VL6180X_VL6180X_LIB_H_

#include "main.h"
#include "vl6180x_api.h"
#include "x-nucleo-6180xa1.h"

#define ScaleDispTime       800
#define AlrmDispTime        800

#define PressBPSwicthTime   1000 /* if user keep bp press more that this mean swicth mode else rooll over use c&se in mode */

#define AlarmKeepDispTime   250  /*  alarm message retain time after it fires */

#define ALLOW_DISABLE_WAF_FROM_BLUE_BUTTON 0 /* set to 1 to add 3 extra modes to the demo where WAF is disabled (to see impact of WAF) */

#define theVL6180xDev   0x52    // what we use as "API device

#define i2c_bus      (&hi2c1)
#define def_i2c_time_out 100


#define ErrRangeDispTime    0 /* Set to 800 ms to display error code when no target os detected */
#if ErrRangeDispTime == 0
#endif

#define DigitDisplay_ms     1 /* ms each digit is kept on */
#if VL6180x_HAVE_DMAX_RANGING
#define DMaxDispTime     0 /* Set to 1000 to display Dmax during 1 sec when no target is detected */
#else
#define DMaxDispTime     0
#endif

#define OutORangeDispfTime  800

enum runmode_t{
    RunRangePoll=0,
    RunAlsPoll,
    InitErr,
    ScaleSwap,
    WaitForReset,
    AlrmStart,
    AlrmRun,
    FromSwitch,
};

char buffer[10];

struct state_t {
    int OutofRAnge:1;
    int AutoScale:1;
    int FilterEn:1;
    uint8_t mode;
    int8_t ScaleSwapCnt;
    uint8_t InitScale;

    uint8_t CurAlrm;
    uint8_t AlrmFired; /* just used to keep display at least min time */
}State;

uint32_t TimeStarted;       /* various display and mode delay starting time */
VL6180x_AlsData_t   Als;    /* ALS measurement */
VL6180x_RangeData_t Range;  /* Range measurmeent  */


uint16_t range;             /* range average distance */

#define AutoThreshHigh  80  /*auto scale high thresh => AutoThreshHigh *  max_raneg => scale ++  */
#define AutoThreshLow   33  /*auto scale low thresh  => AutoThreshHigh *  max_raneg => scale ++  */

volatile uint32_t g_TickCnt;

void HAL_SYSTICK_Callback(void);
void WaitMilliSec(int ms);
int BSP_GetPushButton(void);
void XNUCLEO6180XA1_UserIntHandler(void);
int VL6180x_I2CWrite(VL6180xDev_t addr, uint8_t  *buff, uint8_t len);
int VL6180x_I2CRead(VL6180xDev_t addr, uint8_t  *buff, uint8_t len);
void AbortErr( const char * msg );
void AlsState(void);
void InitAlsMode(void);
void DoScalingSwap(int scaling);
int PusbButton_WaitUnPress(void);
void AlarmShowMode(const char *msg);
void AlarmLowThreshUseCase(void);
void AlarmHighThreshUseCase(void);
void AlarmWindowThreshUseCase(void);
void AlarmUpdateUseCase(void);
void AlarmStop(void);
void AlarmInit(void);
void AlarmState(void);
void GoToAlaramState(void);
void RangeState(void);

#endif /* SRC_VL6180X_VL6180X_LIB_H_ */
