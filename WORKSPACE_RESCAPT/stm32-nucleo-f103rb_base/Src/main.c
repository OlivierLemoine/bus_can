#include "main.h"
//====================================================================
#define VL6180X 0
#define MPU9250 1
#define MPL115A_ANEMO 0
//====================================================================
//			CAN ACCEPTANCE FILTER
//====================================================================
#define USE_FILTER 1
// Can accept until 4 Standard IDs
#define ID_1 0x01
#define ID_2 0x02
#define ID_3 0x03
#define ID_4 0x04
//====================================================================
extern void systemClock_Config(void);

void (*rxCompleteCallback)(void);
void can_callback(void);

CAN_Message rxMsg;
CAN_Message txMsg;

long int counter = 0;

uint8_t *aTxBuffer[2];

extern float magCalibration[3];

void VL6180x_Init(void);
void VL6180x_Step(void);

int status;
int new_switch_state;
int switch_state = -1;

//====================================================================
// >>>>>>>>>>>>>>>>>>>>>>>>>> MAIN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//====================================================================
int main(void)
{
    HAL_Init();
    systemClock_Config();
    SysTick_Config(HAL_RCC_GetHCLKFreq() / 1000); //SysTick end of count event each 1ms
    uart2_Init();

#if VL6180X || MPU9250
    i2c1_Init();
#endif

    HAL_Delay(1000); // Wait

#if MPL115A_ANEMO
    spi1_Init();
    anemo_Timer1Init();
#endif

#if VL6180X
    VL6180x_Init();
#endif

#if MPU9250
    mpu9250_InitMPU9250();
    mpu9250_CalibrateMPU9250();
#if USE_MAGNETOMETER
    mpu9250_InitAK8963(magCalibration);
#endif
    uint8_t response = 0;
    response = mpu9250_WhoAmI();
    //term_printf("%d",response);
#endif

    can_Init();
    can_SetFreq(CAN_BAUDRATE); // CAN BAUDRATE : 500 MHz -- cf Inc/config.h
#if USE_FILTER
#if VL6180X
    can_Filter_list((ID_1 << 21) | (ID_1 << 5), (ID_1 << 21) | (ID_1 << 5), CANStandard, 0); // Accept until 4 Standard IDs
#elif MPU9250
    can_Filter_list((ID_2 << 21) | (ID_2 << 5), (ID_2 << 21) | (ID_2 << 5), CANStandard, 0); // Accept until 4 Standard IDs
#elif MPL115A_ANEMO
    can_Filter_list((ID_3 << 21) | (ID_3 << 5), (ID_3 << 21) | (ID_3 << 5), CANStandard, 0); // Accept until 4 Standard IDs
#endif
#else
    can_Filter_disable(); // Accept everybody
#endif
    can_IrqInit();
    can_IrqSet(&can_callback);

    // Décommenter pour utiliser ce Timer ; permet de déclencher une interruption toutes les N ms
    // Le programme d'interruption est dans tickTimer.c
    //tickTimer_Init(10); // period in ms

    while (1)
    {

#if VL6180X
        VL6180x_Step();

#endif

#if MPU9250
        mpu9250_Step();
#endif
    }
    return 0;
}

//====================================================================
//			CAN CALLBACK RECEPT
//====================================================================

void can_callback(void)
{
    #if VL6180X
    new_switch_state = (new_switch_state == 0) ? 1 : 0;
    #elif MPU9250
    
    #endif
    // CAN_Message msg_rcv;
    // int i = 0;

    // can_Read(&msg_rcv);
    // txMsg.id = 0x55; // Identifiant du message à envoyer

    // for (i = 0; i < 8; i++)
    // {
    //     txMsg.data[i] = msg_rcv.data[i] + 1;
    // }
    // txMsg.len = 8; // Nombre d'octets à envoyer
    // txMsg.format = CANStandard;
    // txMsg.type = CANData;

    // can_Write(txMsg);
}
//====================================================================
//			TIMER CALLBACK PERIOD
//====================================================================

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //term_printf("from timer interrupt\n\r");
    // mpu9250_Step();
}
//====================================================================

void VL6180x_Init(void)
{
    uint8_t id;
    State.mode = 1;

    XNUCLEO6180XA1_Init();
    HAL_Delay(500); // Wait
    // RESET
    XNUCLEO6180XA1_Reset(0);
    HAL_Delay(10);
    XNUCLEO6180XA1_Reset(1);
    HAL_Delay(1);

    HAL_Delay(10);
    VL6180x_WaitDeviceBooted(theVL6180xDev);
    id = VL6180x_Identification(theVL6180xDev);
    term_printf("id=%d, should be 180 (0xB4) \n\r", id);
    VL6180x_InitData(theVL6180xDev);

    State.InitScale = VL6180x_UpscaleGetScaling(theVL6180xDev);
    State.FilterEn = VL6180x_FilterGetState(theVL6180xDev);

    // Enable Dmax calculation only if value is displayed (to save computation power)
    VL6180x_DMaxSetState(theVL6180xDev, DMaxDispTime > 0);

    switch_state = -1; // force what read from switch to set new working mode
    State.mode = AlrmStart;
}
//====================================================================
void VL6180x_Step(void)
{
    DISP_ExecLoopBody();

    //  new_switch_state = XNUCLEO6180XA1_GetSwitch();
    if (new_switch_state != switch_state)
    {
        switch_state = new_switch_state;
        status = VL6180x_Prepare(theVL6180xDev);
        // Increase convergence time to the max (this is because proximity config of API is used)
        VL6180x_RangeSetMaxConvergenceTime(theVL6180xDev, 63);
        if (status)
        {
            AbortErr("ErIn");
        }
        else
        {
            if (switch_state == SWITCH_VAL_RANGING)
            {
                VL6180x_SetupGPIO1(theVL6180xDev, GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT, INTR_POL_HIGH);
                VL6180x_ClearAllInterrupt(theVL6180xDev);
                State.ScaleSwapCnt = 0;
                DoScalingSwap(State.InitScale);
            }
            else
            {
                State.mode = RunAlsPoll;
                InitAlsMode();
            }
        }
    }

    switch (State.mode)
    {
    case RunRangePoll:
        displayLENGTH();
        break;

    case RunAlsPoll:
        displayLUX();
        break;

    case InitErr:
        TimeStarted = g_TickCnt;
        State.mode = WaitForReset;
        break;

    case AlrmStart:
        GoToAlaramState();
        break;

    case AlrmRun:
        AlarmState();
        break;

    case FromSwitch:
        // force reading swicth as re-init selected mode
        switch_state = !XNUCLEO6180XA1_GetSwitch();
        break;

    case ScaleSwap:

        if (g_TickCnt - TimeStarted >= ScaleDispTime)
        {
            State.mode = RunRangePoll;
            TimeStarted = g_TickCnt; /* reset as used for --- to er display */
        }
        else
        {
            DISP_ExecLoopBody();
        }
        break;

    default:
    {
        DISP_ExecLoopBody();
        if (g_TickCnt - TimeStarted >= 5000)
        {
            NVIC_SystemReset();
        }
    }
    }
}
//====================================================================

void sendOverCan(char *data, int dateSize, int id)
{
    CAN_Message txMsg;
    txMsg.len = dateSize; // Nombre d'octets à envoyer
    txMsg.format = CANStandard;
    txMsg.type = CANData;
    txMsg.id = id;

    for (size_t i = 0; i < dateSize; i++)
    {
        txMsg.data[i] = data[i];
    }
    can_Write(txMsg);
}

void displayLUX()
{
    //char buffer[10];
    int status;
    status = VL6180x_AlsPollMeasurement(theVL6180xDev, &Als);
    if (status)
    {
        // SetDisplayString("Er 4");
    }
    else
    {
        sprintf(buffer, "Lux");
        char buff[4];
        int2char_ptr((int)Als.lux, buff);
        sendOverCan(buff, 4, 0x55);
    }
}

void int2char_ptr(int in_value, char *out_value)
{
    out_value[0] = (char)((in_value >> 24) & 0xFF);
    out_value[1] = (char)((in_value >> 16) & 0xFF);
    out_value[2] = (char)((in_value >> 8) & 0xFF);
    out_value[3] = (char)((in_value)&0xFF);
}

void displayLENGTH()
{
    int status;
    uint16_t hlimit;
    uint8_t scaling;

    scaling = VL6180x_UpscaleGetScaling(theVL6180xDev);
    status = VL6180x_RangePollMeasurement(theVL6180xDev, &Range); /* these invoke dipslay for  polling */
    if (status)
    {
        AbortErr("Er r");
        return;
    }

    hlimit = VL6180x_GetUpperLimit(theVL6180xDev);
    if (Range.range_mm >= (hlimit * AutoThreshHigh) / 100 && scaling < 3 && State.AutoScale)
    {
        VL6180x_UpscaleSetScaling(theVL6180xDev, scaling + 1);
    }
    if (Range.range_mm < (hlimit * AutoThreshLow) / 100 && scaling > 1 && State.AutoScale)
    {
        VL6180x_UpscaleSetScaling(theVL6180xDev, scaling - 1);
    }

    if (Range.errorStatus)
    {
        /* no valid ranging*/
        if (State.OutofRAnge)
        {
#if VL6180x_HAVE_DMAX_RANGING
            if (g_TickCnt - TimeStarted >= ErrRangeDispTime && g_TickCnt - TimeStarted < ErrRangeDispTime + DMaxDispTime)
            {
                term_printf("d%d", (int)Range.DMax);
                //SetDisplayString(buffer);
            }
            else

#endif
                if (g_TickCnt - TimeStarted < ErrRangeDispTime)
            {

                term_printf("rE%d\n\r", (int)Range.errorStatus);
                // SetDisplayString(buffer);
            }
            else
            {
                State.OutofRAnge = 0; /* back to out of range display */
                TimeStarted = g_TickCnt;
            }
        }
        else
        {
            int FilterEn;
#if VL6180x_WRAP_AROUND_FILTER_SUPPORT
            FilterEn = VL6180x_FilterGetState(theVL6180xDev);
            if (FilterEn && VL6180x_RangeIsFilteredMeasurement(&Range))
            {
                SetDisplayString("F---");
            }
            else
                SetDisplayString("r---");
#else
            SetDisplayString("r---");
#endif
            if (g_TickCnt - TimeStarted > OutORangeDispfTime)
            {
                State.OutofRAnge = 1;
                TimeStarted = g_TickCnt;
            }
        }
    }
    else
    {
        State.OutofRAnge = 0;
        TimeStarted = g_TickCnt;
        int Alpha =(int)(0.85*(1<<16));
        range = (range * Alpha + Range.range_mm * ((1 << 16) - Alpha)) >> 16;
        
        sprintf(buffer, "Ran");
        char buff[4];
        int2char_ptr((int)range, buff);
        sendOverCan(buff, 4, 0x56);

        if (State.AutoScale)
        {
            if (scaling == 1)
            {
                buffer[0] = '_';
            }
            else if (scaling == 2)
                buffer[0] = '=';
            else
                buffer[0] = '~';
        }

        SetDisplayString(buffer);
    }

#define max_scale 3
    if (!BSP_GetPushButton())
    {
        TimeStarted = g_TickCnt;
        status = PusbButton_WaitUnPress();
        if (status)
        {
            GoToAlaramState();
            return;
        }
        State.ScaleSwapCnt++;
        if (State.ScaleSwapCnt % (max_scale + 1) == max_scale)
        {
            State.AutoScale = 1;
            scaling = max_scale;
        }
        else
        {
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
        if (status < 0)
        {
            AbortErr("ErUp");
            State.mode = InitErr;
        }
        else
        {
            /* do not check status may fail when filter support not active */
            VL6180x_FilterSetState(theVL6180xDev, State.FilterEn);
            DoScalingSwap(scaling);
        }
    }
}
