#include "main.h"
#include "stm32f1xx_it.h"
   
extern CAN_HandleTypeDef   CanHandle;
extern TIM_HandleTypeDef    TimHandle;
extern TIM_HandleTypeDef    TimHandle_period;

extern I2C_HandleTypeDef hi2c1;

//===================================================================
// Cortex-M3 Processor Exceptions Handlers
//===================================================================

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

//==============================================================================
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f1xx.s).                                               */
//==============================================================================

void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c1);
}

void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c1);
}

void USB_LP_CAN1_RX0_IRQHandler(void) {
    HAL_CAN_IRQHandler(&CanHandle);
}

void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle_period);
}

