#include "stm32f4xx.h"
#include <string.h>
#include "utils.h"
#include "UGV_CAN.h"
#include "UGV_UART_JOYSTICK.h"
#include "UGV_ENC.h"
#include "UGV_UART_SONAR.h"

CanTxMsg CAN1_TxMessage;
CanRxMsg CAN1_RxMessage;
mCANdata mydata_read;
mDatastatus mCANstatus;

/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   CAN Pin define 
		*/
		#define		CAN_TRANS_x												CAN1
		#define		CAN_TRANS_CLK											RCC_APB1Periph_CAN1
		#define		CAN_TRANS_GPIO_CLK								RCC_AHB1Periph_GPIOA
		#define		CAN_TRANS_CLK_Cmd									RCC_APB1PeriphClockCmd
		#define		CAN_TRANS_CLK_GPIO_Cmd						RCC_AHB1PeriphClockCmd
		#define		CAN_TRANS_GPIO_PORT								GPIOA
		#define		CAN_TRANS_AF_PORT									GPIO_AF_CAN1
		#define		CAN_TRANS_RX_SOURCE								GPIO_PinSource11
		#define		CAN_TRANS_TX_SOURCE								GPIO_PinSource12
		#define		CAN_TRANS_Rx_Pin 									GPIO_Pin_11 //PB8
		#define 	CAN_TRANS_Tx_Pin 									GPIO_Pin_12 //PB9
		#define 	CANx_TRANS_Prescaler							2 // 	1 Mbit/s
/**
 * @}
 */

void CAN1_Configure(void)
{ 
  CAN_InitTypeDef         CAN_InitStructure;
  CAN_FilterInitTypeDef   CAN_FilterInitStructure;	
  GPIO_InitTypeDef        GPIO_InitStructure;
  
  uint16_t NVIC_Priority_Number;
  
  /* Enable GPIO clock */
  CAN_TRANS_CLK_Cmd(CAN_TRANS_CLK, ENABLE);
  CAN_TRANS_CLK_GPIO_Cmd(CAN_TRANS_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(CAN_TRANS_GPIO_PORT, CAN_TRANS_RX_SOURCE, CAN_TRANS_AF_PORT);
  GPIO_PinAFConfig(CAN_TRANS_GPIO_PORT, CAN_TRANS_TX_SOURCE, CAN_TRANS_AF_PORT);

  /* Configure CAN 1 RX and TX pins */	
  GPIO_InitStructure.GPIO_Pin = CAN_TRANS_Rx_Pin | CAN_TRANS_Tx_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_TRANS_GPIO_PORT, &GPIO_InitStructure);

  CAN_DeInit(CAN_TRANS_x);

  CAN_InitStructure.CAN_TTCM = DISABLE;		// time-triggered communication mode = DISABLED
  CAN_InitStructure.CAN_ABOM = DISABLE;		// automatic bus-off management mode = DISABLED
  CAN_InitStructure.CAN_AWUM = DISABLE;		// automatic wake-up mode = DISABLED
  CAN_InitStructure.CAN_NART = DISABLE; 	// non-automatic retransmission mode = ENABLE (To prevent endless spam)
  CAN_InitStructure.CAN_RFLM = DISABLE; 	// receive FIFO locked mode = DISABLED
  CAN_InitStructure.CAN_TXFP = DISABLE; 	// transmit FIFO priority = DISABLED
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
  CAN_InitStructure.CAN_Prescaler = CANx_TRANS_Prescaler;

  CAN_Init(CAN_TRANS_x, &CAN_InitStructure);	 

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x001<<5;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x100<<5; 
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x113<<5;  
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x111<<5;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN_TRANS_x,CAN_IT_FMP0, ENABLE);
  //FIFO ? full Interrupt
  CAN_ITConfig(CAN_TRANS_x, CAN_IT_FF0, ENABLE);  
  NVIC_Priority_Number = NVIC_EncodePriority(0,1,0);
  NVIC_SetPriority(CAN1_RX0_IRQn,NVIC_Priority_Number);
	
  NVIC_EnableIRQ(CAN1_RX0_IRQn); 		
}

void CAN1_RX0_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_14);
		mCANstatus = sts_new_data;
		CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RxMessage);
		if (mode_manual == 0 && mode_stop ==0)
		{
			switch(CAN1_RxMessage.StdId)
			{
				case 0x111:
				{
//					memcpy(mydata_read.mb, &CAN1_RxMessage.Data, 8);
					mydata_read.mb[3] = CAN1_RxMessage.Data[0];
					mydata_read.mb[2] = CAN1_RxMessage.Data[1];
					mydata_read.mb[1] = CAN1_RxMessage.Data[2];
					mydata_read.mb[0] = CAN1_RxMessage.Data[3];	
					mydata_read.mb[7] = CAN1_RxMessage.Data[4];
					mydata_read.mb[6] = CAN1_RxMessage.Data[5];
					mydata_read.mb[5] = CAN1_RxMessage.Data[6];
					mydata_read.mb[4] = CAN1_RxMessage.Data[7];	
					Car.v_left = mydata_read.mf[0] * 30 /PI;
					Car.v_right = mydata_read.mf[1] * 30 /PI;
					
					break;
				}
			}
		}
	}
	CAN1_Send(212, mydata_write, 8);
}
void CAN1_Send(int _IDstd, mCANdata _data, uint8_t _length)
{
		uint8_t status;
	CAN1_TxMessage.StdId = _IDstd;
	CAN1_TxMessage.RTR = CAN_RTR_DATA;
	CAN1_TxMessage.IDE = CAN_ID_STD;
	CAN1_TxMessage.DLC = _length;
	for(int i=0; i< _length ; i++)
		{
			CAN1_TxMessage.Data[i] = _data.mb[i];	
		}
	
	
	uint8_t mailbox = CAN_Transmit(CAN1, &CAN1_TxMessage);

	// Checks whether the message was sent to a mailbox	
	if (mailbox != CAN_TxStatus_NoMailBox) {
		
	}
//			int32_t i = 0;
//  while((status != CANTXOK) && (i != 0xFFFF))
//  {
//    status = CAN_TransmitStatus(CAN1,mailbox);
//    i++;
//  }
	

}


//void CAN_TR(int _IDstd, mCANdata* _data, uint8_t _length)
//	{
//		uint32_t uwCounter = 0;
//		uint8_t TransmitMailbox = 0;
//		uint8_t i = 0;
//		/* transmit */
//		CAN1_TxMessage.StdId =  _IDstd;
//		CAN1_TxMessage.RTR = CAN_RTR_DATA;
//		CAN1_TxMessage.IDE = CAN_ID_STD;
//		CAN1_TxMessage.DLC = _length;
//		for(i=0; i< _length ; i++)
//		{
//			CAN1_TxMessage.Data[i] = _data->mb[i];	
//		}

//		TransmitMailbox = CAN_Transmit(CAN1, &CAN1_TxMessage);
//		while((CAN_TransmitStatus(CAN1, TransmitMailbox)  !=  CANTXOK) && (uwCounter  !=  0xFFFF))
//		{
//			uwCounter++;
//		}
//		uwCounter = 0;
//		while((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (uwCounter  !=  0xFFFF))
//		{
//			uwCounter++;
//		}
//	}

//void CAN_TransmitAndWait(CAN_TypeDef* CANx, CanTxMsg* TxMessage)
//{
//	uint8_t TransmitMailbox = CAN_Transmit(CANx, TxMessage);
//	
//	uint32_t uwCounter = 0;
//	while((CAN_TransmitStatus(CANx, TransmitMailbox) != CANTXOK) && (uwCounter != 0xFFFF))
//		uwCounter++;
//}
