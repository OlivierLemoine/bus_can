/*
 * can_bus.c
 *
 *  Created on: 10 sept. 2015
 *      Author: kerhoas
 */

#include "drv_can.h"

extern void (*rxCompleteCallback) (void);
CAN_HandleTypeDef    CanHandle;
CanRxMsgTypeDef     canRxMsg;
CanTxMsgTypeDef     canTxMsg;

extern void can_callback(void);

//===========================================================================
// INIT CAN
//===========================================================================
void can_Init()
{
	CanHandle.Instance = CAN1;
	CanHandle.pTxMsg = &canTxMsg;
	CanHandle.pRxMsg = &canRxMsg;

	CanHandle.Init.TTCM = DISABLE;
	CanHandle.Init.ABOM = DISABLE;
	CanHandle.Init.AWUM = DISABLE;
	CanHandle.Init.NART = DISABLE;
	CanHandle.Init.RFLM = DISABLE;
	CanHandle.Init.TXFP = DISABLE;
	CanHandle.Init.Mode = CAN_MODE_NORMAL;

	// 125kbps bit rate (default)
	// APB1 peripheral clock = 36000000Hz
	CanHandle.Init.Prescaler = 18;      // number of time quanta = 36000000/18/125000 = 16
	CanHandle.Init.SJW = CAN_SJW_1TQ;
	CanHandle.Init.BS1 = CAN_BS1_11TQ;  // sample point at (1 + 11) / 16 * 100 = 75%
	CanHandle.Init.BS2 = CAN_BS2_4TQ;

	HAL_CAN_Init(&CanHandle);
	can_IrqSet(can_callback);
	
//	can_Filter(0, 0, CANAny, 0);
}

//===========================================================================
int can_SetFreq(int hz) {

    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    // APB1 peripheral clock = 36000000Hz
    switch(hz) {
    case 1000000:
        // 1000kbps bit rate
        CanHandle.Init.Prescaler = 3;//3;      // number of time quanta = 36000000/3/1000000 = 12
        CanHandle.Init.SJW = CAN_SJW_1TQ;
        CanHandle.Init.BS1 = CAN_BS1_8TQ;  // sample point at: (1 + 8) / 12 * 100 = 75%
        CanHandle.Init.BS2 = CAN_BS2_3TQ;
        break;
    case 500000:
        // 500kbps bit rate
        CanHandle.Init.Prescaler = 6;      // number of time quanta = 36000000/6/500000 = 12
        CanHandle.Init.SJW = CAN_SJW_1TQ;
        CanHandle.Init.BS1 = CAN_BS1_8TQ;  // sample point at: (1 + 8) / 12 * 100 = 75%
        CanHandle.Init.BS2 = CAN_BS2_3TQ;
        break;
    case 250000:
        // 250kbps
        CanHandle.Init.Prescaler = 9;      // number of time quanta = 36000000/9/250000 = 16
        CanHandle.Init.SJW = CAN_SJW_1TQ;
        CanHandle.Init.BS1 = CAN_BS1_11TQ; // sample point at: (1 + 11) / 16 * 100 = 75%
        CanHandle.Init.BS2 = CAN_BS2_4TQ;
        break;
    case 125000:
        // 125kbps
        CanHandle.Init.Prescaler = 18;     // number of time quanta = 36000000/18/125000 = 16
        CanHandle.Init.SJW = CAN_SJW_1TQ;
        CanHandle.Init.BS1 = CAN_BS1_11TQ; // sample point at: (1 + 11) / 16 * 100 = 75%
        CanHandle.Init.BS2 = CAN_BS2_4TQ;
        break;
    default:
        // 125kbps (default)
        CanHandle.Init.Prescaler = 18;     // number of time quanta = 36000000/18/125000 = 16
        CanHandle.Init.SJW = CAN_SJW_1TQ;
        CanHandle.Init.BS1 = CAN_BS1_11TQ; // sample point at: (1 + 11) / 16 * 100 = 75%
        CanHandle.Init.BS2 = CAN_BS2_4TQ;
    }

    HAL_CAN_Init(&CanHandle);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

    return 1;
}
//===========================================================================

void can_IrqInit(void)
{
	HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0);
}

//===========================================================================
void can_IrqFree()
{
    rxCompleteCallback = NULL;
}
//===========================================================================
void can_IrqSet(void (*fptr) (void))
{
    rxCompleteCallback = fptr;
}
//===========================================================================
int can_Write(CAN_Message msg)
{
    int i = 0;

    if(msg.format == CANStandard) {
        CanHandle.pTxMsg->StdId = msg.id;
        CanHandle.pTxMsg->ExtId = 0x00;
    }
    else {
        CanHandle.pTxMsg->StdId = 0x00;
        CanHandle.pTxMsg->ExtId = msg.id;
    }

    CanHandle.pTxMsg->RTR = msg.type == CANData ? CAN_RTR_DATA : CAN_RTR_REMOTE;
    CanHandle.pTxMsg->IDE = msg.format == CANStandard ? CAN_ID_STD : CAN_ID_EXT;
    CanHandle.pTxMsg->DLC = msg.len;

    for(i = 0; i < msg.len; i++)
        CanHandle.pTxMsg->Data[i] = msg.data[i];

    if(HAL_CAN_Transmit(&CanHandle, 0) != HAL_OK)
							{
								 return 0;
							 }

    else
        return 1;
}
//===========================================================================
int can_Read(CAN_Message* msg)
{
	int i=0;

    msg->id = CanHandle.pRxMsg->IDE == CAN_ID_STD ? CanHandle.pRxMsg->StdId : CanHandle.pRxMsg->ExtId;
    msg->type = CanHandle.pRxMsg->RTR == CAN_RTR_DATA ? CANData : CANRemote;
    msg->format = CanHandle.pRxMsg->IDE == CAN_ID_STD ? CANStandard : CANExtended;
    msg->len = CanHandle.pRxMsg->DLC;
    for(i = 0; i < msg->len; i++)
        msg->data[i] = CanHandle.pRxMsg->Data[i];

    return msg->len;
}
//===========================================================================
int can_Mode(CanMode mode)
{
    switch(mode) {
    case MODE_RESET:
        return HAL_ERROR;

    case MODE_NORMAL:
        CanHandle.Init.Mode = CAN_MODE_NORMAL;
        break;

    case MODE_SILENT:
        CanHandle.Init.Mode = CAN_MODE_SILENT;
        break;

    case MODE_TEST_GLOBAL:
        CanHandle.Init.Mode = CAN_MODE_LOOPBACK;
        break;

    case MODE_TEST_LOCAL:
        CanHandle.Init.Mode = CAN_MODE_LOOPBACK;
        break;

    case MODE_TEST_SILENT:
        CanHandle.Init.Mode = CAN_MODE_SILENT_LOOPBACK;
        break;
    }

    return HAL_CAN_Init(&CanHandle);
}
//===========================================================================
int can_Filter_list(uint32_t id1_id2, uint32_t id3_id4, CANFormat format /*=CANAny*/, int32_t handle /*=0*/ )
{
    CAN_FilterConfTypeDef   sFilterConfig;

    sFilterConfig.FilterNumber = handle;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;  // CAN_FILTERMODE_IDMASK
    
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT; // CAN_FILTERSCALE_32BIT
    sFilterConfig.FilterIdHigh = (((id1_id2) >> 16) & 0xFFFF);
    sFilterConfig.FilterIdLow = ((id1_id2) & 0xFFFF);
    sFilterConfig.FilterMaskIdHigh = (((id3_id4) >> 16) & 0xFFFF);
    sFilterConfig.FilterMaskIdLow = ((id3_id4) & 0xFFFF);
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 14;
    HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig);

    return 1;
}

//===========================================================================
int can_Filter_disable()
{
    CAN_FilterConfTypeDef   sFilterConfig;

    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;

    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0;
    sFilterConfig.FilterIdLow = 0;
    sFilterConfig.FilterMaskIdHigh = 0;
    sFilterConfig.FilterMaskIdLow = 0;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 14;
    HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig);

    return 1;
}


//===========================================================================
void can_Reset()
{
    __HAL_CAN_RESET_HANDLE_STATE(&CanHandle);
}
//===========================================================================
unsigned char can_Rderror()
{
    return HAL_CAN_GetError(&CanHandle);
}
//===========================================================================
unsigned char can_Tderror()
{
    return HAL_CAN_GetError(&CanHandle);
}
//===========================================================================

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle)
{
    if (rxCompleteCallback != NULL)
        rxCompleteCallback();

    if (CanHandle->State == HAL_CAN_STATE_BUSY_TX)
        CanHandle->State = HAL_CAN_STATE_BUSY_TX_RX;
    else {
        CanHandle->State = HAL_CAN_STATE_BUSY_RX;

        /* Set CAN error code to none */
        CanHandle->ErrorCode = HAL_CAN_ERROR_NONE;

        /* Enable Error warning Interrupt */
        __HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_EWG);

        /* Enable Error passive Interrupt */
        __HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_EPV);

        /* Enable Bus-off Interrupt */
        __HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_BOF);

        /* Enable Last error code Interrupt */
        __HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_LEC);

        /* Enable Error Interrupt */
        __HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_ERR);
    }

    // Enable FIFO 0 message pending Interrupt
    __HAL_CAN_ENABLE_IT(CanHandle, CAN_IT_FMP0);
}
