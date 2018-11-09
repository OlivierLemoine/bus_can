#include "vl6180x_lib.h"

#define BSP_BP_PORT GPIOC
#define BSP_BP_PIN  GPIO_PIN_13

char Buff[10]="---.--";
float V=0000.1;
int Alpha =(int)(0.85*(1<<16));    /* range distance running average cofs */

/***************  DISPLAY PUBLIC *********************/
const char *DISP_NextString;
/***************  DISPLAY PRIVATE *********************/
static char DISP_CurString[10];
static int DISP_Loop=0;

void DISP_ExecLoopBody(void){
    if (DISP_NextString != NULL) {
        strncpy(DISP_CurString, DISP_NextString, sizeof(DISP_CurString) - 1);
        DISP_CurString[sizeof(DISP_CurString) - 1] = 0;
        DISP_NextString = NULL;
    }
    XNUCLEO6180XA1_DisplayString(DISP_CurString, DigitDisplay_ms);
    DISP_Loop++;
}
//============================================================
void DISP_Task(const void *arg) {
    do {
        DISP_ExecLoopBody();
    } while (1);
}
//============================================================

void SetDisplayString(const char *msg) {
    DISP_NextString=msg;
}
//============================================================
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin==GPIO_PIN_0)
	{
		XNUCLEO6180XA1_UserIntHandler();
	}
}
//============================================================

void HAL_SYSTICK_Callback(void){
    g_TickCnt++;
}
//============================================================
void XNUCLEO6180XA1_WaitMilliSec(int n){
    WaitMilliSec(n);
}
//============================================================
void WaitMilliSec(int ms){
    uint32_t start, now;
    int dif;
    start=g_TickCnt;
    do{
        now=g_TickCnt;
        dif= now -start;
    }
    while(dif<ms);
}
//============================================================
int BSP_GetPushButton(void){
    GPIO_PinState state ;
    state = HAL_GPIO_ReadPin(BSP_BP_PORT, BSP_BP_PIN);
    return state;
}
//============================================================
volatile int IntrFired=0;
/* VL6180x shield user interrupt handler */
void XNUCLEO6180XA1_UserIntHandler(void){
    IntrFired ++;
}
//=================================================================================
int VL6180x_I2CWrite(VL6180xDev_t addr, uint8_t  *buff, uint8_t len){
    int status;
    status = i2c1_WriteBuffer(addr,  buff, len);
    return status;
}
//=================================================================================
int VL6180x_I2CRead(VL6180xDev_t addr, uint8_t  *buff, uint8_t len){
    int status;
    status = i2c1_ReadBuffer(addr,  buff, len);
    return status;
}
//=================================================================================
void AbortErr( const char * msg ){
    State.mode=  WaitForReset;
}
//=================================================================================
/**
 * ALS mode idle run loops
 */
void AlsState(void)
{
    int status;
    status = VL6180x_AlsPollMeasurement(theVL6180xDev, &Als);
    if (status) {
       // SetDisplayString("Er 4");
    } else {
        if (Als.lux > 9999) {
        	term_printf("L----\n\r" );
        }
        else if (Als.lux > 999) {
        	term_printf("LUM:%d.%d\n\r", (int) Als.lux / 1000, (int) (Als.lux % 1000) / 10); /* show LX.YY  X k Lux 2 digit*/
        	sprintf(buffer, "L%d.%02d", (int) Als.lux / 1000, (int) (Als.lux % 1000) / 10); /* show LX.YY  X k Lux 2 digit*/
        } else {
        	term_printf( "lum:%d\n\r", (int) Als.lux);
        	sprintf(buffer, "l%3d", (int) Als.lux);
        }

        SetDisplayString( buffer );
    }
}
//=================================================================================

void InitAlsMode(void){
    //anything after prepare and prior to go into AlsState
    int time = 100;
    VL6180x_AlsSetIntegrationPeriod(theVL6180xDev, time);
}
//=================================================================================
/**
 * Manage UI and state for scale change
 *
 * @param scaling the next scaling factor
 */
void DoScalingSwap(int scaling){
    if( State.AutoScale){
        if( State.FilterEn ){
        					SetDisplayString("Sf A");
        					term_printf("Sf A");
        					}

		else 				{
							SetDisplayString("Sc A");
							term_printf("Sc A");
							}
    }
    else{
        if( State.FilterEn )
        {
			term_printf("Sf %d\n\r", (int)scaling);
        	sprintf(buffer, "Sf %d", (int)scaling);
        }
        	else
        {
        	term_printf("Sc %d\n\r", (int)scaling);
    		sprintf(buffer, "Sc %d", (int)scaling);
        }
        SetDisplayString(buffer);

    }
    State.mode = ScaleSwap;
    TimeStarted=g_TickCnt;
}
//=================================================================================
/**
 * When button is already pressed it Wait for user to release it
 * if button remain pressed for given time it return true
 * These is used to detect mode switch by long press on blue Push Button
 *
 * As soon as time is elapsed -rb- is displayed  to let user know order
 * the  request to switch mode is taken into account
 *
 * @return True if button remain pressed more than specified time
 */
int PusbButton_WaitUnPress(void){
    TimeStarted = g_TickCnt;
    while( !BSP_GetPushButton() ){ ; //
        DISP_ExecLoopBody();
        if( g_TickCnt - TimeStarted> PressBPSwicthTime){
        	term_printf(" rb \n\r");
        }
    }
    return  g_TickCnt - TimeStarted>PressBPSwicthTime;

}
//=================================================================================
void AlarmShowMode(const char *msg)
{
   // SetDisplayString( msg);
    TimeStarted=g_TickCnt;
    do {
       // DISP_ExecLoopBody();
    } while (g_TickCnt - TimeStarted < AlrmDispTime);
}
//=================================================================================
void AlarmLowThreshUseCase(void){
    AlarmShowMode("A-Lo");

    /* make sure from now on all register in group are not fetched by device */
    VL6180x_SetGroupParamHold(theVL6180xDev, 1);

    /* get interrupt whenever we go below 200mm */
    VL6180x_RangeSetThresholds(theVL6180xDev, 200, 0, 0 );
    /* set range interrupt reporting low threshold*/
    VL6180x_RangeConfigInterrupt(theVL6180xDev, CONFIG_GPIO_INTERRUPT_LEVEL_LOW);

    /* leave device peak up all new register in group */
    VL6180x_SetGroupParamHold(theVL6180xDev, 0);

    /* clear any interrupt that should ensure a new edge get generated even if we missed it */
    VL6180x_RangeClearInterrupt(theVL6180xDev);
}
//=================================================================================
void AlarmHighThreshUseCase(void){
    AlarmShowMode("A-hi");
    /* make sure from now on all register in group are not fetched by device */
    VL6180x_SetGroupParamHold(theVL6180xDev, 1);

    /* get interrupt whenever  higher than 200mm (low threshold don't care) */
    VL6180x_RangeSetThresholds(theVL6180xDev, 0, 200, 0 );

    /* set range interrupt reporting high threshold*/
    VL6180x_RangeConfigInterrupt(theVL6180xDev, CONFIG_GPIO_INTERRUPT_LEVEL_HIGH);

    /* leave device peak up all new register in group */
    VL6180x_SetGroupParamHold(theVL6180xDev, 0);

    /* clear any interrupt that should ensure a new edge get generated even if we missed it */
    VL6180x_RangeClearInterrupt(theVL6180xDev);
}
//=================================================================================
void AlarmWindowThreshUseCase(void){

    AlarmShowMode("A-0o");

    /* make sure from now on all register in group are not fetched by device */
    VL6180x_SetGroupParamHold(theVL6180xDev, 1);

    /* get interrupt whenever  out of  100mm  250mm  range */
    VL6180x_RangeSetThresholds(theVL6180xDev, 100, 200, 0 );

    /* set range interrupt reporting out of window  */
    VL6180x_RangeConfigInterrupt(theVL6180xDev, CONFIG_GPIO_INTERRUPT_OUT_OF_WINDOW);

    /* leave device peak up all new register in group */
    VL6180x_SetGroupParamHold(theVL6180xDev, 0);

    /* clear any interrupt that should ensure a new edge get generated even if we missed it */
    VL6180x_RangeClearInterrupt(theVL6180xDev);
}
//=================================================================================
void AlarmUpdateUseCase(void)
{
    State.CurAlrm =(State.CurAlrm%3);

    switch ( State.CurAlrm) {
    case 0: /* low thresh */
        AlarmLowThreshUseCase();
        break;
    case 1: /* high thresh */
        AlarmHighThreshUseCase();;
        break;
    case 2: /* out of window */
        AlarmWindowThreshUseCase();
    }
    VL6180x_RangeClearInterrupt(theVL6180xDev); /* clear any active interrupt it will ensure we get a new active edge is raised */
}
//=================================================================================

void AlarmStop(void){
    VL6180x_RangeSetSystemMode(theVL6180xDev, MODE_CONTINUOUS|MODE_START_STOP);
    /* Wait some time for last potential measure to stop ?
     * TODO can we poll check something to avoid that delay? */
    HAL_Delay(100);
    /* Clear any left pending interrupt
     * these is not mandatory or a left uncleared status can mess-up next intr mode change and status check  without a prior intr clear */
    VL6180x_ClearAllInterrupt(theVL6180xDev);

    /* Anover way to stop is to switch and trigger a single shot measure (in a safe way)
     * set interrupt report mode new sample ready
     * clear interrupt
     * kick of a measure
     * poll for measure ready
     * all that can take up to arround 2x max convergence time typically set to 50ms  */

}

//=================================================================================
/**
 * Initiate alarm (interrupt mode on distance threshold)
 */
void AlarmInit(void){
    State.mode = AlrmRun;
    TimeStarted=g_TickCnt;
    uint16_t InterMeasPeriod=50; /* 10 ms is the minimal */
    /* We assume device is stopped  */

    VL6180x_Prepare(theVL6180xDev);
    /* Increase convergence time to the max (this is because proximity config of API is used) */
    VL6180x_RangeSetMaxConvergenceTime(theVL6180xDev, 63);
    /* set max upscale so we can work up to some  50cm */
    VL6180x_UpscaleSetScaling(theVL6180xDev, 3);

    /* set inter measurement period (that is in fact inter measure time)
     * note that when low refresh rate  is need time like 100ms is best to keep power low  */
    VL6180x_RangeSetInterMeasPeriod(theVL6180xDev, InterMeasPeriod);
    /* if fast reaction is required then set a time of 0 (will set minimal possible) */
    /* VL6180x_RangeSetInterMeasPeriod(theVL6180xDev, 0); */

    /* setup gpio1 pin to range interrupt output with high polarity (rising edge) */
    VL6180x_SetupGPIO1(theVL6180xDev, GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT, INTR_POL_HIGH);
    /* set threshold for current used case and update the display */
    AlarmUpdateUseCase();
    /* enable interrupt at CPU level */
    //XNUCLEO6180XA1_EnableInterrupt();
    /*Clear any pending device interrupt even if already active it should force a new edge so we can pick up*/
    VL6180x_ClearAllInterrupt(theVL6180xDev);

    /* start continuous mode */
    VL6180x_RangeSetSystemMode(theVL6180xDev, MODE_START_STOP|MODE_CONTINUOUS);
    /* from now vl6180x is running on it's own and will interrupt us when condition is met
     * the interrupt set a flag peek-up in AlarmState run loop*/
}
//=================================================================================
/**
 * Alarm mode idle run
 *
 * We only here look at the volatile interrupt flags set that from isr
 * the isr manage to clear interrupt at cpu level
 * we Here clear and re-arm/clear interrupt at the the device level and do the UI and display
 */
void AlarmState(void){
    IntrStatus_t IntStatus;
    int status;

    if (IntrFired != 0) {
    /* Interrupt did fired Get interrupt  causes */
        status = VL6180x_RangeGetInterruptStatus(theVL6180xDev, &IntStatus.val);
        if (status) {
            AbortErr("Al 1");
            goto done;
        }
        switch( IntStatus.status.Range ) {
        case RES_INT_STAT_GPIO_LOW_LEVEL_THRESHOLD :
          term_printf("L---");
            break;
        case RES_INT_STAT_GPIO_HIGH_LEVEL_THRESHOLD :
        	term_printf("H---");
            break;
        case RES_INT_STAT_GPIO_OUT_OF_WINDOW :
            term_printf("O---");
            break;
        case RES_INT_STAT_GPIO_NEW_SAMPLE_READY:
        	term_printf("n---");
            break;
        }
        VL6180x_RangeClearInterrupt(theVL6180xDev); /* clear it */
        IntrFired = 0;
        TimeStarted=g_TickCnt;
        State.AlrmFired = 1;
    }
    else{
        int flush=0;
        //sanity check we are not in a state where i/o is active without an edge
        if( g_TickCnt-TimeStarted> 5000 ){
            if( flush )
                VL6180x_RangeClearInterrupt(theVL6180xDev); /* clear it */
            TimeStarted=g_TickCnt;
        }
    }
    if( State.AlrmFired ){
        /* After an interrupt fire keep the display message for some minimal time
         * over wise it could not be visible at all */
        if( g_TickCnt-TimeStarted > AlarmKeepDispTime )
            State.AlrmFired = 0;
    }
    else{
        switch( State.CurAlrm ){
            case 0 :
            	term_printf("alarm = Low\n\r");
                break;
            case 1 :
               	term_printf("alarm = High\n\r");
                break;
            case 2:
             	term_printf("alarm = Window\n\r");
                break;
        }
    }
    /* keep On refreshing display at every idle loop */
    DISP_ExecLoopBody();

    if( !BSP_GetPushButton() ){
        // when button get presses wait it get release (keep doing display)
        status = PusbButton_WaitUnPress();
        if( status ){
            // BP stay pressed very long time switch back to range/als
            AlarmStop();
            State.mode=FromSwitch;
        }
        else{
            //BP short pressed switch use case
            State.CurAlrm=(State.CurAlrm+1)%3;
            AlarmUpdateUseCase();
        }
    }

done:
    ;
}

//=================================================================================
void GoToAlaramState(void) {
    AlarmInit();
}
//=================================================================================
/**
 * Ranging mode idle loop
 */
void RangeState(void) {
    int status;
    uint16_t hlimit;
    uint8_t scaling;

    scaling = VL6180x_UpscaleGetScaling(theVL6180xDev);
    status = VL6180x_RangePollMeasurement(theVL6180xDev, &Range); /* these invoke dipslay for  polling */
    if (status) {
        AbortErr("Er r");
        return;
    }

    hlimit = VL6180x_GetUpperLimit(theVL6180xDev);
    if (Range.range_mm >= (hlimit * AutoThreshHigh) / 100 && scaling < 3 && State.AutoScale) {
        VL6180x_UpscaleSetScaling(theVL6180xDev, scaling + 1);
    }
    if (Range.range_mm < (hlimit * AutoThreshLow) / 100 && scaling > 1 && State.AutoScale) {
        VL6180x_UpscaleSetScaling(theVL6180xDev, scaling - 1);
    }

    if (Range.errorStatus) {
        /* no valid ranging*/
        if (State.OutofRAnge) {
#if VL6180x_HAVE_DMAX_RANGING
            if (g_TickCnt - TimeStarted >= ErrRangeDispTime &&  g_TickCnt - TimeStarted <  ErrRangeDispTime + DMaxDispTime ){
                    term_printf("d%d", (int)Range.DMax);
                    //SetDisplayString(buffer);
            }
            else

#endif
            if(g_TickCnt - TimeStarted < ErrRangeDispTime  )
            {

                term_printf("rE%d\n\r", (int) Range.errorStatus);
               // SetDisplayString(buffer);
            }
            else{
                State.OutofRAnge=0; /* back to out of range display */
                TimeStarted=g_TickCnt;
            }
        }
        else {
            int FilterEn;
#if VL6180x_WRAP_AROUND_FILTER_SUPPORT
            FilterEn = VL6180x_FilterGetState(theVL6180xDev);
            if (FilterEn && VL6180x_RangeIsFilteredMeasurement(&Range) ){
                SetDisplayString("F---");
            }
            else
                SetDisplayString("r---");
#else
            SetDisplayString("r---");
#endif
            if( g_TickCnt - TimeStarted > OutORangeDispfTime ) {
                State.OutofRAnge = 1;
                TimeStarted = g_TickCnt;
            }
        }
    }
    else {
        State.OutofRAnge = 0;
        TimeStarted = g_TickCnt;
        range = (range * Alpha + Range.range_mm * ((1 << 16) - Alpha)) >> 16;
    	term_printf("range:%d\n\r", (int) range);
    	sprintf(buffer,"r%d",(int)range);
        if (State.AutoScale) {
            if (scaling == 1) {
                buffer[0] = '_';
            }
            else
                if (scaling == 2)
                    buffer[0] = '=';
                else
                    buffer[0] = '~';
        }

        SetDisplayString(buffer);
    }

#define max_scale 3
    if (!BSP_GetPushButton()) {
        TimeStarted = g_TickCnt;
        status = PusbButton_WaitUnPress();
        if (status) {
            GoToAlaramState();
            return;
        }
        State.ScaleSwapCnt++;
        if (State.ScaleSwapCnt % (max_scale + 1) == max_scale) {
            State.AutoScale = 1;
            scaling = max_scale;
        }
        else {
#if ALLOW_DISABLE_WAF_FROM_BLUE_BUTTON
            /* togle filtering every time we roll over all scaling(pass by autoscale) */
            if (State.AutoScale)
                State.FilterEn = !State.FilterEn;
#endif
            State.AutoScale = 0;
            scaling = State.InitScale + (State.ScaleSwapCnt % max_scale);
            if (scaling > max_scale)
                scaling = scaling - (max_scale);
        }

        status = VL6180x_UpscaleSetScaling(theVL6180xDev, scaling);
        if (status<0) {
            AbortErr("ErUp");
            State.mode = InitErr;
        }
        else {
            /* do not check status may fail when filter support not active */
            VL6180x_FilterSetState(theVL6180xDev, State.FilterEn);
            DoScalingSwap(scaling);
        }
    }
}



