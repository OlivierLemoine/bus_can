#include "main.h"
//====================================================================
#define VL6180X 0
#define MPU9250 0
#define MPL115A_ANEMO 1
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

    txMsg.id = 0x55;
    txMsg.data[0] = 1;
    txMsg.data[1] = 2;
    txMsg.len = 2;
    txMsg.format = CANStandard;
    txMsg.type = CANData;

    can_Write(txMsg);

    // Décommenter pour utiliser ce Timer ; permet de déclencher une interruption toutes les N ms
    // Le programme d'interruption est dans tickTimer.c
    tickTimer_Init(10); // period in ms

    while (1)
    {

#if VL6180X
        VL6180x_Step();
        

#endif

#if MPU9250
    

#endif
    }
    return 0;
}

//====================================================================
//			CAN CALLBACK RECEPT
//====================================================================

void can_callback(void)
{

    new_switch_state = (new_switch_state == 0) ? 1 : 0;

    /*
    CAN_Message msg_rcv;
    int i = 0;

    can_Read(&msg_rcv);
    txMsg.id = 0x55; // Identifiant du message à envoyer

    for (i = 0; i < 8; i++)
    {
        txMsg.data[i] = msg_rcv.data[i] + 1;
    }
    txMsg.len = 8; // Nombre d'octets à envoyer
    txMsg.format = CANStandard;
    txMsg.type = CANData;

    can_Write(txMsg);
    */
}
//====================================================================
//			TIMER CALLBACK PERIOD
//====================================================================

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //term_printf("from timer interrupt\n\r");
    // mpu9250_Step();

    
    displayPressure();
    int anemo = anemo_GetCount();
    displayAnemo(anemo);
    anemo_ResetCount();
    // convertion Hz to Km/h
    

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
        RangeState();
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

void sendOverCan(char *data,int dateSize,int id)
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
        if (Als.lux > 9999)
        {
            term_printf("L----\n\r");
        }
        else if (Als.lux > 999)
        {
            term_printf("LUM:%d.%d\n\r", (int)Als.lux / 1000, (int)(Als.lux % 1000) / 10); /* show LX.YY  X k Lux 2 digit*/
            sprintf(buffer, "L%d.%02d", (int)Als.lux / 1000, (int)(Als.lux % 1000) / 10);  /* show LX.YY  X k Lux 2 digit*/
        }
        else
        {
            term_printf("lum:%d\n\r", (int)Als.lux);
            sprintf(buffer, "l%3d", (int)Als.lux);
        }

        sendOverCan(Als.lux,sizeof(Als.lux),0x55);
    }
}
/*
void displayLENGTH()
{
    int status;
    status = VL6180x_AlsPollMeasurement(theVL6180xDev, &Als);
    if (status)
    {
        // SetDisplayString("Er 4");
    }
    else
    {
        if (Als.lux > 9999)
        {
            term_printf("L----\n\r");
        }
        else if (Als.lux > 999)
        {
            term_printf("LUM:%d.%d\n\r", (int)Als.lux / 1000, (int)(Als.lux % 1000) / 10); // show LX.YY  X k Lux 2 digit
            sprintf(buffer, "L%d.%02d", (int)Als.lux / 1000, (int)(Als.lux % 1000) / 10);  // show LX.YY  X k Lux 2 digit 
        }
        else
        {
            term_printf("lum:%d\n\r", (int)Als.lux);
            sprintf(buffer, "l%3d", (int)Als.lux);
        }

        sendOverCan(buffer);
    }
}
*/

void displayAnemo(int countAnemo)
{
    //float r = (float)countAnemo / 0.01 ; // secondes-1
    // aproximation 10Hz = 10 Km/h ( voir le graph pour etre plus précis)

    

   // sendOverCan((int)r,sizeof(r),0x55); // 0x55 à changer
}

void displayPressure()
{
    uint8_t coef = 0;
    uint8_t t1 = 0;
    uint8_t t2 = 0;
    coef = spi1_Transfer(0x24);
    spi1_Transfer(0x00);
    //HAL_Delay(4);
    spi1_Transfer(0x24);
    //HAL_Delay(4);
    t1 = spi1_Transfer(0x80);
    t2 = spi1_Transfer(0x82);

    //term_printf("%d",t1);
    //term_printf("%d",t2);
   // sendOverCan((int)r,sizeof(r),0x55); // 0x55 à changer
}
