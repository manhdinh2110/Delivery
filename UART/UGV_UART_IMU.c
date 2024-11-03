#include "stm32f4xx.h"
#include <string.h>
#include "UGV_UART_IMU.h"
#include "utils.h"

/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   IMU Pin define 
		*/
		
		/* Definition for USARTx resources ******************************************/
		#define		USARTx_IMU_TX_CLK_INIT		  								 	RCC_AHB1PeriphClockCmd
		#define 	USARTx_IMU_TX_GPIO_CLK												RCC_AHB1Periph_GPIOC
		#define 	USARTx_IMU_TX_GPIO_PORT 					 						GPIOC
		#define		USARTx_IMU_TX_PIN															GPIO_Pin_6
		#define 	USARTx_IMU_TX_SOURCE											 		GPIO_PinSource6
		
		#define		USARTx_IMU_RX_CLK_INIT		  	 								RCC_AHB1PeriphClockCmd
		#define 	USARTx_IMU_RX_GPIO_CLK												RCC_AHB1Periph_GPIOC
		#define 	USARTx_IMU_RX_GPIO_PORT 				  						GPIOC
		#define		USARTx_IMU_RX_PIN															GPIO_Pin_7
		#define 	USARTx_IMU_RX_SOURCE					 								GPIO_PinSource7
		
		#define		USARTx_IMU																		USART6
		#define 	USARTx_IMU_CLK																RCC_APB2Periph_USART6
		#define		USARTx_IMU_CLK_INIT		 												RCC_APB2PeriphClockCmd
		#define 	USARTx_IMU_BAUDRATE														115200
		#define		USARTx_IMU_AF																	GPIO_AF_USART6
		#define 	USARTx_IMU_IRQn																USART6_IRQn
		#define 	USARTx_IMU_IRQPreemptionPriority							0x00
		#define 	USARTx_IMU_IRQSubPriority											0x02
		
		#define		USARTx_IMU_DMA																DMA2
		#define 	USARTx_IMU_DMAx_CLK                  					RCC_AHB1Periph_DMA2
		#define   USARTx_IMU_DMA_Channel												DMA_Channel_5
		#define   USARTx_IMU_DMA_Stream													DMA2_Stream1
		#define		USARTx_IMU_DMA_CLK_Cmd		  									RCC_AHB1PeriphClockCmd
		#define		USARTx_IMU_DMA_Stream_IRQn										DMA2_Stream1_IRQn
		#define 	USARTx_IMU_DMA_IRQPreemptionPriority					0x00
		#define 	USARTx_IMU_DMA_IRQSubPriority									0x01
		
		#define		USARTx_IMU_DMA_Stream_IRQHandler							DMA2_Stream1_IRQHandler
		#define		USARTx_IMU_IRQHandler													USART6_IRQHandler
		
		#define 	USARTx_IMU_BUFFER_LENGTH 											100
		
	/**
 * @}
 */
uint8_t 	rxbuff_USART6[BUFF_SIZE_USART6_Rx];
IMU_Frame IMU_FrameReceive;
uint8_t strRoll[5]={0},strPitch[5]={0},strYaw[5]={0};
uint8_t strAccelX[5]={0},strAccelY[5]={0},strAccelZ[5]={0};
uint8_t strGyroX[5]={0},strGyroY[5]={0},strGyroZ[5]={0};
IMU_t IMU;
uint8_t Send_count;


void USART6_Rx_DMA(void)
	{
 	GPIO_InitTypeDef 	GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;  
	DMA_InitTypeDef   DMA_InitStructure;
	
	/* Enable GPIO clock */
	USARTx_IMU_TX_CLK_INIT(USARTx_IMU_TX_GPIO_CLK, ENABLE);
	USARTx_IMU_RX_CLK_INIT(USARTx_IMU_RX_GPIO_CLK, ENABLE);
		
	/* Enable USART clock */
	USARTx_IMU_CLK_INIT(USARTx_IMU_CLK, ENABLE);
		
	/* Enable DMA clock */
	USARTx_IMU_DMA_CLK_Cmd(USARTx_IMU_DMAx_CLK, ENABLE);
		
	/* Connect USART6 pins to AF2 */  
  GPIO_PinAFConfig(USARTx_IMU_TX_GPIO_PORT, USARTx_IMU_TX_SOURCE, USARTx_IMU_AF);
  GPIO_PinAFConfig(USARTx_IMU_RX_GPIO_PORT, USARTx_IMU_RX_SOURCE, USARTx_IMU_AF); 
		
	/* GPIO Configuration for USART6 Tx */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin   = USARTx_IMU_TX_PIN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USARTx_IMU_TX_GPIO_PORT, &GPIO_InitStructure);
	
	/* GPIO Configuration for USART6 Rx */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin   = USARTx_IMU_RX_PIN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USARTx_IMU_RX_GPIO_PORT, &GPIO_InitStructure);
	

  /* USARTx configured as follow:
		- BaudRate = 100000 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = USARTx_IMU_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx_IMU, &USART_InitStructure);
  
	/* Enable USART */
  USART_Cmd(USARTx_IMU, ENABLE);

	/* Enable USART6 DMA */
  USART_DMACmd(USARTx_IMU, USART_DMAReq_Rx, ENABLE);

	/* DMA2 Stream2 Channel5 for USART6 Rx configuration */	
  DMA_InitStructure.DMA_Channel = USARTx_IMU_DMA_Channel;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rxbuff_USART6;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = USARTx_IMU_BUFFER_LENGTH;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;//
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  
	DMA_Init(USARTx_IMU_DMA_Stream, &DMA_InitStructure);
  DMA_Cmd(USARTx_IMU_DMA_Stream, ENABLE);
	}
void DMA2_Stream1_IRQHandler(void)
{
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
	
		DMA_Cmd(DMA2_Stream1, ENABLE);
}

void Get_IMU (void)
 {
	 String_Split(rxbuff_USART6,'$',1,IMU_FrameReceive.RQx);
	 if(IMU_FrameReceive.RQx[0]=='R' && IMU_FrameReceive.RQx[1]=='Q')
	 {
			 String_Split(IMU_FrameReceive.RQx,',',1,strRoll);
			 String_Split(IMU_FrameReceive.RQx,',',2,strPitch);
			 String_Split(IMU_FrameReceive.RQx,',',3,strYaw);
			 
			 IMU.Roll = (Str2Double(strRoll) - 4300)*0.1;
			 IMU.Pitch = (Str2Double(strPitch) - 4300)*0.1;
			 IMU.Yaw = -(Str2Double(strYaw) - 4300)*0.1;
			 
			 String_Split(IMU_FrameReceive.RQx,',',4,strAccelX);
			 String_Split(IMU_FrameReceive.RQx,',',5,strAccelY);
			 String_Split(IMU_FrameReceive.RQx,',',6,strAccelZ);
			 
			 IMU.AccelX = (Str2Double(strAccelX)/20000);
			 IMU.AccelY = (Str2Double(strAccelY)/20000);
			 IMU.AccelZ = (Str2Double(strAccelZ)*9.8065/20000);
			 
			 String_Split(IMU_FrameReceive.RQx,',',7,strGyroX);
			 String_Split(IMU_FrameReceive.RQx,',',8,strGyroY);
			 String_Split(IMU_FrameReceive.RQx,',',9,strGyroZ); 
			 
			 IMU.GyroX = (Str2Double(strGyroX)*PI/180);
			 IMU.GyroY = (Str2Double(strGyroY)*PI/180);
			 IMU.GyroZ = (Str2Double(strGyroZ)*PI/180);
	 }
	}