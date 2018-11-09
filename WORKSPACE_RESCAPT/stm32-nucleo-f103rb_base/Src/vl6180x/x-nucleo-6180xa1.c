
#include "x-nucleo-6180xa1.h"
#include "main.h"
#include "drv_i2c.h"

#define V2_IRQ_PIN          GPIO_PIN_0
#define V2_IRQ EXTI0_IRQn

uint16_t _V2PadVal; // gpio SR value caching to avoid reading on each new bit set

static int _err=0;

static void _V2_DisplayOff(void);
static void _V2_SetChipEn( int No, int state );
static int _V2_GetSwicth(void);
static  void _V2_Set7Segment( int Leds, int digit );

//====================================================================
// @brief Default  do nothing interrupt callback
//====================================================================
#pragma weak XNUCLEO6180XA1_UserIntHandler
void  XNUCLEO6180XA1_UserIntHandler(void)
{
}
//====================================================================
// Init XNUCLEO6180XA1 Expander
//====================================================================
void XNUCLEO6180XA1_Init(I2C_HandleTypeDef *hi2c1) {

    uint8_t ExpanderID[2];
    uint16_t PadDir;

    i2c1_ReadRegBuffer(EXPANDER_I2C_ADDRESS,0, ExpanderID,2);

    PadDir=~V2_DISP_SEL;	    // expander config
    i2c1_WriteRegBuffer(EXPANDER_I2C_ADDRESS, GPDR,  (uint8_t*)&PadDir, 2);
    _V2_DisplayOff();

}
//====================================================================
int XNUCLEO6180XA1_EXTI_CallBackHandle(uint16_t GPIO_Pin)
{
        XNUCLEO6180XA1_UserIntHandler();
        return 0;
}

//====================================================================
/**
 *
 * @param No    0= top , 1= Left, 2=Bottom 3=Right
 * @param state
 */
static void _V2_SetChipEn( int No, int state ){

	int mask = (No==3) ? V2_CHIPEN_R : ((No==2) ? V2_CHIPEN_B : ((No==1) ? V2_CHIPEN_L : V2_CHIPEN));
    if( state)
        _V2PadVal|=mask ;
    else
        _V2PadVal&=~mask;

    i2c1_WriteRegBuffer(EXPANDER_I2C_ADDRESS, GPSR, (uint8_t*)&_V2PadVal, 2); // Set Pin State Register

}
//====================================================================
void XNUCLEO6180XA1_Reset(int state)
{
        _V2_SetChipEn(0, state);
}
//====================================================================
int XNUCLEO6180XA1_ResetId(int state, int id)
{
        _V2_SetChipEn(id, state);
        return 0;
}
//====================================================================
// Get Switch Button (red one) to choose between 'ALS' and 'range'
//====================================================================
int XNUCLEO6180XA1_GetSwitch(void){
    GPIO_PinState state ;
    state= _V2_GetSwicth() ? GPIO_PIN_SET : GPIO_PIN_RESET ;
    return state;
}
//====================================================================
int _V2_GetSwicth(){
    int status;
    uint16_t Value;
    status = i2c1_ReadRegBuffer(EXPANDER_I2C_ADDRESS,GPMR, (uint8_t*)&Value,2);
    if(status ==0 ){
        Value&=V2_DISP_SEL;
    }
    else{
        _err++;
        Value=0;
    }
    return Value;
}

//====================================================================
// Display string on 7 segments
//====================================================================
void XNUCLEO6180XA1_DisplayString(const char *str, int SegDelayMs){
    int i;
    const char *pc;

    for( i=0, pc=str; i<4 && *pc!=0 ; i++, pc++){
        _V2_Set7Segment( ascii_to_display_lut[(uint8_t)*pc], i);
        if( *(pc+1)== '.'){
            pc++;
        }
    }
    XNUCLEO6180XA1_WaitMilliSec(SegDelayMs);
    _V2_DisplayOff();
 }
//====================================================================
// Update 7 segment display
//====================================================================
static  void _V2_Set7Segment( int Leds, int digit ){
    _V2PadVal |= 0x7F; /* clear 7 seg bits */
    _V2PadVal |= V2_D1|V2_D2|V2_D3|V2_D4; /* all segment off */
    _V2PadVal &= ~(V2_D1<<digit);         /* digit on */
    _V2PadVal &= ~(Leds&0x7F);

    i2c1_WriteRegBuffer(EXPANDER_I2C_ADDRESS, GPSR, (uint8_t*)&_V2PadVal, 2);
}

//====================================================================
// Clear 7 segments
//====================================================================
static void _V2_DisplayOff() {
    _V2PadVal |= (V2_D1|V2_D2|V2_D3| V2_D4); /* segment en off */
    i2c1_WriteRegBuffer(EXPANDER_I2C_ADDRESS, GPSR, (uint8_t*)&_V2PadVal, 2);
}
//====================================================================

