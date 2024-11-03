#include "stm32f4xx.h"
#include "UGV_ENC.h"
#include "UGV_PWM.h"
#include "UGV_CAN.h"
#include "utils.h"

mCANdata mydata_write;

/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   ENCODER Pin define 
		*/
		#define		ENC1_CLK_Cmd		  								 		RCC_AHB1PeriphClockCmd
		#define		ENC1_CLK															RCC_AHB1Periph_GPIOB
		#define		ENC1_PORT															GPIOB
		
		#define 	ENC1_CH1_PORT 					 						 	GPIOB
		#define		ENC1_CH1_PIN													GPIO_Pin_6
		#define 	ENC1_CH1_SOURCE									 			GPIO_PinSource6
		#define		ENC1_TIM_Channel1											TIM_Channel_1
		#define		ENC1_TIM_ICFilter1										15
		
		#define 	ENC1_CH2_PORT 					 						 	GPIOB
		#define		ENC1_CH2_PIN													GPIO_Pin_7
		#define 	ENC1_CH2_SOURCE									 			GPIO_PinSource7
		#define		ENC1_TIM_Channel2											TIM_Channel_2
		#define		ENC1_TIM_ICFilter2										15
		
		#define		ENC1_TIM_AF														GPIO_AF_TIM4
		#define		ENC1_TIM															TIM4
		#define 	ENC1_TIM_CLK													RCC_APB1Periph_TIM4
		#define		ENC1_TIM_CLK_Cmd											RCC_APB1PeriphClockCmd
		#define		ENC1_TIM_Prescaler										0
		#define		ENC1_TIM_Period												0xffff
		#define		ENC1_TIM_ClockDivision								TIM_CKD_DIV1
		
		#define		ENC1_TIM_SetCounter										0
		
		#define		ENC2_CLK_Cmd		  								 		RCC_AHB1PeriphClockCmd
		#define		ENC2_CLK															RCC_AHB1Periph_GPIOB
		#define		ENC2_PORT															GPIOB
		
		#define 	ENC2_CH1_PORT 					 						 	GPIOB
		#define		ENC2_CH1_PIN													GPIO_Pin_4
		#define 	ENC2_CH1_SOURCE									 			GPIO_PinSource4
		#define		ENC2_TIM_Channel1											TIM_Channel_1
		#define		ENC2_TIM_ICFilter1										15
		
		#define 	ENC2_CH2_PORT 					 						 	GPIOB
		#define		ENC2_CH2_PIN													GPIO_Pin_5
		#define 	ENC2_CH2_SOURCE									 			GPIO_PinSource5
		#define		ENC2_TIM_Channel2											TIM_Channel_2
		#define		ENC2_TIM_ICFilter2										15
				
		#define		ENC2_TIM_AF														GPIO_AF_TIM3
		#define		ENC2_TIM															TIM3
		#define 	ENC2_TIM_CLK													RCC_APB1Periph_TIM3
		#define		ENC2_TIM_CLK_Cmd											RCC_APB1PeriphClockCmd
		#define		ENC2_TIM_Prescaler										0
		#define		ENC2_TIM_Period												0xffff
		#define		ENC2_TIM_ClockDivision								TIM_CKD_DIV1
		
		#define		ENC2_TIM_SetCounter										0
		
		#define		ENC_RESOLUTION												400
		
/**
 * @}
 */


void ENC_Config(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;
		
		//ENC1 PB6,PB7, Timer4
		ENC1_CLK_Cmd(ENC1_CLK, ENABLE);
		ENC1_TIM_CLK_Cmd(ENC1_TIM_CLK, ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = ENC1_CH1_PIN| ENC1_CH2_PIN;
		GPIO_Init(ENC1_PORT, &GPIO_InitStructure);

		GPIO_PinAFConfig(ENC1_CH1_PORT, ENC1_CH1_SOURCE, ENC1_TIM_AF);
		GPIO_PinAFConfig(ENC1_CH2_PORT, ENC1_CH2_SOURCE, ENC1_TIM_AF);

		TIM_TimeBaseStructure.TIM_Prescaler = ENC1_TIM_Prescaler;
		TIM_TimeBaseStructure.TIM_ClockDivision = ENC1_TIM_ClockDivision; // = 0
		TIM_TimeBaseStructure.TIM_Period = ENC1_TIM_Period ;  // su dung toi da thanh ghi ARR
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(ENC1_TIM, &TIM_TimeBaseStructure);

		TIM_ICInitStructure.TIM_Channel = ENC1_TIM_Channel1;
		TIM_ICInitStructure.TIM_ICFilter = ENC1_TIM_ICFilter1; 
		TIM_ICInitStructure.TIM_Channel = ENC1_TIM_Channel2;
		TIM_ICInitStructure.TIM_ICFilter = ENC1_TIM_ICFilter2;
		TIM_ICInit(ENC1_TIM, &TIM_ICInitStructure);
		
		TIM_EncoderInterfaceConfig(ENC1_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

		TIM_SetCounter(ENC1_TIM, ENC1_TIM_SetCounter);
		
		TIM_Cmd(ENC1_TIM, ENABLE);
		
		//ENC2 PB4,PB5, Timer3
		ENC2_CLK_Cmd(ENC2_CLK, ENABLE);
		ENC2_TIM_CLK_Cmd(ENC2_TIM_CLK, ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = ENC2_CH1_PIN| ENC2_CH2_PIN;
		GPIO_Init(ENC2_PORT, &GPIO_InitStructure);

		GPIO_PinAFConfig(ENC2_CH1_PORT, ENC2_CH1_SOURCE, ENC2_TIM_AF);
		GPIO_PinAFConfig(ENC2_CH2_PORT, ENC2_CH2_SOURCE, ENC2_TIM_AF);

		TIM_TimeBaseStructure.TIM_Prescaler = ENC2_TIM_Prescaler;
		TIM_TimeBaseStructure.TIM_ClockDivision = ENC2_TIM_ClockDivision; // = 0
		TIM_TimeBaseStructure.TIM_Period = ENC2_TIM_Period ;  // su dung toi da thanh ghi ARR
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(ENC2_TIM, &TIM_TimeBaseStructure);

		TIM_ICInitStructure.TIM_Channel = ENC2_TIM_Channel1;
		TIM_ICInitStructure.TIM_ICFilter = ENC2_TIM_ICFilter1; 
		TIM_ICInitStructure.TIM_Channel = ENC2_TIM_Channel2;
		TIM_ICInitStructure.TIM_ICFilter = ENC2_TIM_ICFilter2;	
		TIM_ICInit(ENC2_TIM, &TIM_ICInitStructure);

		TIM_EncoderInterfaceConfig(ENC2_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
		
		TIM_SetCounter(ENC2_TIM, ENC2_TIM_SetCounter);
		
		TIM_Cmd(ENC2_TIM, ENABLE);
	}
	
	
void Get_ENC_speed(MOTOR* motor1,MOTOR* motor2)
	{
		motor1->pulse = TIM_GetCounter(ENC1_TIM);
		motor2->pulse = TIM_GetCounter(ENC2_TIM);
		
		//motor1
		motor1->delta_pulse =  ENC1_TIM_SetCounter - motor1->pulse;
		motor1->speed = 60*100*motor1->delta_pulse/(4*ENC_RESOLUTION*60); //convert pulse/T_sample to rpm

		TIM_SetCounter(ENC1_TIM, ENC1_TIM_SetCounter);
		
		//motor2
		motor2->delta_pulse = motor2->pulse - ENC2_TIM_SetCounter;
		motor2->speed = 60*100*motor2->delta_pulse/(4*ENC_RESOLUTION*60);

		TIM_SetCounter(ENC2_TIM, ENC2_TIM_SetCounter);
		
		mydata_write.mf[0] = (float)Motor_1.speed * PI / 30;
		mydata_write.mf[1] = (float)Motor_2.speed * PI / 30;
		//CAN1_Send(212, mydata_write, 8);

	}

	