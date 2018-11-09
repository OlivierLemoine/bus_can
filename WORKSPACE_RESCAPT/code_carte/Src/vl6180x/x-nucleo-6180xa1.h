/*
 * x-nucleo-6180xa1.h
 *
 *  Created on: 5 avr. 2016
 *      Author: kerhoas
 */

#ifndef SRC_VL6180X_X_NUCLEO_6180XA1_H_
#define SRC_VL6180X_X_NUCLEO_6180XA1_H_

#include "types.h"
#include "main.h"



#define SWITCH_VAL_RANGING  0



/* function below must be provided  */
void XNUCLEO6180XA1_WaitMilliSec(int SegDelayMs);
/* optional list */
void XNUCLEO6180XA1_UserIntHandler(void);

/* provided */
void XNUCLEO6180XA1_Init();
void XNUCLEO6180XA1_DisplayString(const char *str, int SegDelayMs);

void XNUCLEO6180XA1_ClearInterrupt(void);

int XNUCLEO6180XA1_GetSwitch(void);
void XNUCLEO6180XA1_Reset(int state);


/**
 * @brief  tio be called by HAL EXTI handler dispatecher
 * @warning to be called only aftrer @a XNUCLEO6180XA1_I2C1_Init() get called
 * @param pin
 * @return  non 0 if the GPIO_pin is the GPIO pin for active VL6180x GPIO int line
 */
int XNUCLEO6180XA1_EXTI_CallBackHandle(uint16_t GPIO_Pin);


/*all function below only valid for v2 */
#define XNUCLEO6180XA1_ID_TOP   0
#define XNUCLEO6180XA1_ID_BOT   1
#define XNUCLEO6180XA1_ID_LEFT  2
#define XNUCLEO6180XA1_ID_RIGHT 3
int XNUCLEO6180XA1_ResetId(int state, int id);

#define GPMR    0x10 		// Monitor Pin State Register
#define GPSR    0x12 		// Set Pin State Register
#define GPDR    0x14 		// Set Pin Direction Register

#define V2_D1       (1<<7)
// second byte or word MSB
#define V2_D2       (1<<8)
#define V2_D3       (1<<9)
#define V2_D4       (1<<10)
#define V2_DISP_SEL (1<<11)
#define V2_CHIPEN   (1<<12)
#define V2_CHIPEN_B (1<<13)
#define V2_CHIPEN_L (1<<14)
#define V2_CHIPEN_R (1<<15)


//====================================================================
/**
 * Display digit enable pad/pin LUT
 * D1 is left most digit  (ms nibble) on the display
 * D3 is right most digit (ls nibble) on the display
 */
#define PA  0
#define PB  1
#define PC  2
#define MAKE_PAD(port, pin)  (((port)<<4)|pin)


#undef MAKE_PAD
#undef PA
#undef PB
#undef PC

#define pad_write(pad, state)   HAL_GPIO_WritePin( (void*)((char*)GPIOA + (pad>>4)*((char*)GPIOB-(char*)GPIOA)), 1<<(pad&0x0F), state);

/*
   --s0--
   s    s
   5    1
   --s6--
   s    s
   4    2
   --s3-- s7

 */

#define DP  (1<<7)
#define NOT_7_NO_DP( ... ) (uint8_t) ~( __VA_ARGS__ + DP )
#define S0 (1<<0)
#define S1 (1<<1)
#define S2 (1<<2)
#define S3 (1<<3)
#define S4 (1<<4)
#define S5 (1<<5)
#define S6 (1<<6)

/* refer to http://www.twyman.org.uk/Fonts/ */
static const uint8_t ascii_to_display_lut[256]={
      [' ']=           0,
      ['-']=           S6,
      ['_']=           S3,
      ['=']=           S3+S6,
      ['~']=           S0+S3+S6, /* 3 h bar */
      ['^']=           S0, /* use as top bar */

      ['?']=           NOT_7_NO_DP(S5+S3+S2),
      ['*']=           NOT_7_NO_DP(),
      ['[']=           S0+S3+S4+S5,
      [']']=           S0+S3+S2+S1,
      ['@']=           S0+S3,

      ['0']=           NOT_7_NO_DP(S6),
      ['1']=           S1+S2,
      ['2']=           S0+S1+S6+S4+S3,
      ['3']=           NOT_7_NO_DP(S4+S5),
      ['4']=           S5+S1+S6+S2,
      ['5']=           NOT_7_NO_DP(S1+S4),
      ['6']=           NOT_7_NO_DP(S1),
      ['7']=           S0+S1+S2,
      ['8']=           NOT_7_NO_DP(0),
      ['9']=           NOT_7_NO_DP(S4),

      ['a']=           S2+ S3+ S4+ S6 ,
      ['b']=           NOT_7_NO_DP(S0+S1),
      ['c']=           S6+S4+S3,
      ['d']=           NOT_7_NO_DP(S0+S5),
      ['e']=           NOT_7_NO_DP(S2),
      ['f']=           S6+S5+S4+S0, /* same as F */
      ['g']=           NOT_7_NO_DP(S4), /* same as 9 */
      ['h']=           S6+S5+S4+S2,
      ['i']=           S4,
      ['j']=           S1+S2+S3+S4,
      ['k']=           S6+S5+S4+S2, /* a h */
      ['l']=           S3+S4,
      ['m']=           S0+S4+S2, /* same as  */
      ['n']=           S2+S4+S6,
      ['o']=           S6+S4+S3+S2,
      ['p']=           NOT_7_NO_DP(S3+S2), // same as P
      ['q']=           S0+S1+S2+S5+S6,
      ['r']=           S4+S6,
      ['s']=           NOT_7_NO_DP(S1+S4),
      ['t']=           NOT_7_NO_DP(S0+S1+S2),
      ['u']=           S4+S3+S2+S5+S1, // U
      ['v']=           S4+S3+S2, // is u but u use U
      ['w']=           S1+S3+S5,
      ['x']=           NOT_7_NO_DP(S0+S3), // similar to H
      ['y']=           NOT_7_NO_DP(S0+S4),
      ['z']=           S0+S1+S6+S4+S3, // same as 2

      ['A']=           NOT_7_NO_DP(S3),
      ['B']=           NOT_7_NO_DP(S0+S1), /* as b  */
      ['C']=           S0+S3+S4+S5, // same as [
      ['E']=           NOT_7_NO_DP(S1+S2),
      ['F']=           S6+S5+S4+S0,
      ['G']=           NOT_7_NO_DP(S4), /* same as 9 */
      ['H']=           NOT_7_NO_DP(S0+S3),
      ['I']=           S1+S2,
      ['J']=           S1+S2+S3+S4,
      ['K']=           NOT_7_NO_DP(S0+S3), /* same as H */
      ['L']=           S3+S4+S5,
      ['M']=           S0+S4+S2, /* same as  m*/
      ['N']=           S2+S4+S6, /* same as n*/
      ['O']=           NOT_7_NO_DP(S6),
      ['P']=           S0+S1+S2+S5+S6, // sames as 'q'
      ['Q']=           NOT_7_NO_DP(S3+S2),
      ['R']=           S4+S6,
      ['S']=           NOT_7_NO_DP(S1+S4), /* sasme as 5 */
      ['T']=           NOT_7_NO_DP(S0+S1+S2), /* sasme as t */
      ['U']=           NOT_7_NO_DP(S6+S0),
      ['V']=           S4+S3+S2, // is u but u use U
      ['W']=           S1+S3+S5,
      ['X']=           NOT_7_NO_DP(S0+S3), // similar to H
      ['Y']=           NOT_7_NO_DP(S0+S4),
      ['Z']=           S0+S1+S6+S4+S3, // same as 2


};

#undef S0
#undef S1
#undef S2
#undef S3
#undef S4
#undef S5
#undef S6
#undef DP

#endif /* SRC_VL6180X_X_NUCLEO_6180XA1_H_ */
