#include "stm32f4xx.h"
#include "utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "UGV_UART_SONAR.h"
#include "UGV_UART_JOYSTICK.h"
#include "UGV_PWM.h"
/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   SONAR Pin define 
		*/
		
		/* Definition for USARTx resources ******************************************/
/*DEFINE FOR SONAR 1*/
		#define		USARTx_SONAR1_TX_CLK_INIT		  								 	RCC_AHB1PeriphClockCmd
		#define 	USARTx_SONAR1_TX_GPIO_CLK												RCC_AHB1Periph_GPIOA
		#define 	USARTx_SONAR1_TX_GPIO_PORT 					 						GPIOA
		#define		USARTx_SONAR1_TX_PIN														GPIO_Pin_0
		#define 	USARTx_SONAR1_TX_SOURCE											 		GPIO_PinSource0
		
		#define		USARTx_SONAR1_RX_CLK_INIT		  	 								RCC_AHB1PeriphClockCmd
		#define 	USARTx_SONAR1_RX_GPIO_CLK												RCC_AHB1Periph_GPIOA
		#define 	USARTx_SONAR1_RX_GPIO_PORT 				  						GPIOA
		#define		USARTx_SONAR1_RX_PIN														GPIO_Pin_1
		#define 	USARTx_SONAR1_RX_SOURCE					 								GPIO_PinSource1
		
		#define		USARTx_SONAR1																		UART4
		#define 	USARTx_SONAR1_CLK																RCC_APB1Periph_UART4
		#define		USARTx_SONAR1_CLK_INIT		 											RCC_APB1PeriphClockCmd
		#define 	USARTx_SONAR1_BAUDRATE													9600
		#define		USARTx_SONAR1_AF																GPIO_AF_UART4
		#define 	USARTx_SONAR1_IRQn															UART4_IRQn
//		#define 	USARTx_SONAR1K_IRQPreemptionPriority						0x00
//		#define 	USARTx_SONAR1K_IRQSubPriority										0x02
		
		#define		USARTx_SONAR1_DMA																DMA1
		#define 	USARTx_SONAR1_DMAx_CLK                  				RCC_AHB1Periph_DMA1
		#define   USARTx_SONAR1_DMA_Channel												DMA_Channel_4
		#define   USARTx_SONAR1_DMA_Stream												DMA1_Stream2 // RX only 
		#define		USARTx_SONAR1_DMA_CLK_Cmd		  									RCC_AHB1PeriphClockCmd
		#define		USARTx_SONAR1_DMA_Stream_IRQn										DMA1_Stream2_IRQn
//		#define 	USARTx_SONAR1K_DMA_IRQPreemptionPriority				0x00
//		#define 	USARTx_SONAR1K_DMA_IRQSubPriority								0x01
		
		#define		USARTx_SONAR1_DMA_Stream_IRQHandler						DMA1_Stream2_IRQHandler
		#define		USARTx_SONAR1_IRQHandler												UART4_IRQHandler
		
		#define 	USARTx_SONAR1_BUFFER_LENGTH 										25
			
/*DEFINE FOR SONAR 2*/
		#define		USARTx_SONAR2_TX_CLK_INIT		  								 	RCC_AHB1PeriphClockCmd
		#define 	USARTx_SONAR2_TX_GPIO_CLK												RCC_AHB1Periph_GPIOA
		#define 	USARTx_SONAR2_TX_GPIO_PORT 					 						GPIOA
		#define		USARTx_SONAR2_TX_PIN														GPIO_Pin_2
		#define 	USARTx_SONAR2_TX_SOURCE											 		GPIO_PinSource2
		
		#define		USARTx_SONAR2_RX_CLK_INIT		  	 								RCC_AHB1PeriphClockCmd
		#define 	USARTx_SONAR2_RX_GPIO_CLK												RCC_AHB1Periph_GPIOA
		#define 	USARTx_SONAR2_RX_GPIO_PORT 				  						GPIOA
		#define		USARTx_SONAR2_RX_PIN														GPIO_Pin_3
		#define 	USARTx_SONAR2_RX_SOURCE					 								GPIO_PinSource3
		
		#define		USARTx_SONAR2																		USART2
		#define 	USARTx_SONAR2_CLK																RCC_APB1Periph_USART2
		#define		USARTx_SONAR2_CLK_INIT		 											RCC_APB1PeriphClockCmd
		#define 	USARTx_SONAR2_BAUDRATE													9600
		#define		USARTx_SONAR2_AF																GPIO_AF_USART2
		#define 	USARTx_SONAR2_IRQn															USART2_IRQn
//		#define 	USARTx_SONAR2K_IRQPreemptionPriority						0x00
//		#define 	USARTx_SONAR2K_IRQSubPriority										0x02
		
		#define		USARTx_SONAR2_DMA																DMA1
		#define 	USARTx_SONAR2_DMAx_CLK                  				RCC_AHB1Periph_DMA1
		#define   USARTx_SONAR2_DMA_Channel												DMA_Channel_4
		#define   USARTx_SONAR2_DMA_Stream												DMA1_Stream5 // RX only 
		#define		USARTx_SONAR2_DMA_CLK_Cmd		  									RCC_AHB1PeriphClockCmd
		#define		USARTx_SONAR2_DMA_Stream_IRQn										DMA1_Stream5_IRQn
//		#define 	USARTx_SONAR2K_DMA_IRQPreemptionPriority				0x00
//		#define 	USARTx_SONAR2K_DMA_IRQSubPriority								0x01
		
		#define		USARTx_SONAR2_DMA_Stream_IRQHandler							DMA1_Stream5_IRQHandler
		#define		USARTx_SONAR2_IRQHandler												USART2_IRQHandler
		
		#define 	USARTx_SONAR2_BUFFER_LENGTH 										25			
			
			
			
			
			
/**
 * @}
 */
mybuffer_sonar uart_buffer_u2,uart_buffer_u4;
range_sonar range_SONAR;
int mode_stop;
double range ,range1;
void SONAR1_USART4_DMA_Rx_Config(void)
{
			GPIO_InitTypeDef 	GPIO_InitStructure; 
			USART_InitTypeDef USART_InitStructure;  
			DMA_InitTypeDef   DMA_InitStructure;
			
			/* Enable GPIO clock */
			USARTx_SONAR1_TX_CLK_INIT(USARTx_SONAR1_TX_GPIO_CLK, ENABLE);
			/* Enable UART clock */
			USARTx_SONAR1_CLK_INIT(USARTx_SONAR1_CLK, ENABLE);
		/* Enable DMA clock */
		USARTx_SONAR1_DMA_CLK_Cmd(USARTx_SONAR1_DMAx_CLK, ENABLE);
		
		/* GPIO Configuration for UART Rx */
		USARTx_SONAR1_RX_CLK_INIT(USARTx_SONAR1_RX_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Pin   = USARTx_SONAR1_RX_PIN;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(USARTx_SONAR1_RX_GPIO_PORT, &GPIO_InitStructure);
		
		/* GPIO Configuration for UART Tx */
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Pin   = USARTx_SONAR1_TX_PIN;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(USARTx_SONAR1_TX_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(USARTx_SONAR1_TX_GPIO_PORT, USARTx_SONAR1_TX_SOURCE, USARTx_SONAR1_AF);
		GPIO_PinAFConfig(USARTx_SONAR1_RX_GPIO_PORT, USARTx_SONAR1_RX_SOURCE, USARTx_SONAR1_AF); 

		/* USARTx configured as follow:
			- BaudRate = 9600 baud  
			- Word Length = 8 Bits
			- One Stop Bit
			- No parity
			- Hardware flow control disabled (RTS and CTS signals)
			- Receive and transmit enabled
		*/
		USART_InitStructure.USART_BaudRate = USARTx_SONAR1_BAUDRATE;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USARTx_SONAR1, &USART_InitStructure);
		
		/* Enable USART */
		USART_Cmd(USARTx_SONAR1, ENABLE);
		
		/* Enable UART DMA */
		USART_DMACmd(USARTx_SONAR1, USART_DMAReq_Rx, ENABLE);
		
		/* DMA1 Stream2 Channel4 for UART4 Rx configuration */	
		DMA_InitStructure.DMA_Channel = USARTx_SONAR1_DMA_Channel;  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uart_buffer_u4.mbuff_rx;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = USARTx_SONAR1_BUFFER_LENGTH;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;//
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
		DMA_Init(USARTx_SONAR1_DMA_Stream, &DMA_InitStructure);
		DMA_Cmd(USARTx_SONAR1_DMA_Stream, ENABLE);

}

 void SONAR2_USART2_DMA_Rx_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;  
	DMA_InitTypeDef   DMA_InitStructure;
	
	/* Enable GPIO clock */
	USARTx_SONAR2_TX_CLK_INIT(USARTx_SONAR2_TX_GPIO_CLK, ENABLE);
	/* Enable UART clock */
	USARTx_SONAR2_CLK_INIT(USARTx_SONAR2_CLK, ENABLE);
	/* Enable DMA clock */
	USARTx_SONAR2_DMA_CLK_Cmd(USARTx_SONAR2_DMAx_CLK, ENABLE);
	
	/* GPIO Configuration for UART Rx */
	USARTx_SONAR2_RX_CLK_INIT(USARTx_SONAR2_RX_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin   = USARTx_SONAR2_RX_PIN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(USARTx_SONAR2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* GPIO Configuration for UART Tx */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin   = USARTx_SONAR2_TX_PIN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(USARTx_SONAR2_TX_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(USARTx_SONAR2_TX_GPIO_PORT, USARTx_SONAR2_TX_SOURCE, USARTx_SONAR2_AF);
  GPIO_PinAFConfig(USARTx_SONAR2_RX_GPIO_PORT, USARTx_SONAR2_RX_SOURCE, USARTx_SONAR2_AF); 

  /* USARTx configured as follow:
		- BaudRate = 9600 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = USARTx_SONAR2_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx_SONAR2, &USART_InitStructure);
  
	/* Enable USART */
  USART_Cmd(USARTx_SONAR2, ENABLE);
	
	/* Enable UART DMA */
	USART_DMACmd(USARTx_SONAR2, USART_DMAReq_Rx, ENABLE);
	
	/* DMA1 Stream2 Channel4 for UART4 Rx configuration */	
  DMA_InitStructure.DMA_Channel = USARTx_SONAR2_DMA_Channel;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uart_buffer_u2.mbuff_rx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = USARTx_SONAR2_BUFFER_LENGTH;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal;//
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  
	DMA_Init(USARTx_SONAR2_DMA_Stream, &DMA_InitStructure);
  DMA_Cmd(USARTx_SONAR2_DMA_Stream, ENABLE);

}

void Mode_stop(void){
		range_SONAR.range_left  = Get_range_from_sonar(&uart_buffer_u2);
		range_SONAR.range_right = Get_range_from_sonar(&uart_buffer_u4);
		if(range_SONAR.range_left > range_SONAR.range_right)
			range_SONAR.range = range_SONAR.range_right;
		else if(range_SONAR.range_left <= range_SONAR.range_right)
			range_SONAR.range = range_SONAR.range_left;
		
		if ( range_SONAR.range <= 100 ){
			mode_stop = 1;
			Car.v_left = 0;
			Car.v_right = 0;
		}
		else mode_stop = 0;
			
}
double Get_range_from_sonar(mybuffer_sonar *buffer1){
	uint8_t value[5];
	String_Split(buffer1->mbuff_rx,'R',1,value);
	return Str2Double(value);
}

