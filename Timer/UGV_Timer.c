#include "stm32f4xx.h"
#include "string.h"
#include "UGV_Timer.h"
#include "UGV_UART_JOYSTICK.h"
#include "UGV_ENC.h"
#include "UGV_PWM.h"
#include "UGV_CAN.h"
#include "UGV_UART_SONAR.h"

/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   Timer Pin define 
		*/
		#define		TIMER_MOTOR				 				TIM5
		#define		TIMER_MOTOR_CLK						RCC_APB1Periph_TIM5
		#define 	TIMER_MOTOT_CLK_Cmd				RCC_APB1PeriphClockCmd
		#define		TIMER_MOTOR_Prescaler			8399
		#define		TIMER_MOTOR_Period				99
		
//		#define		TIMER_SAMPLE							TIM6
//		#define		TIMER_SAMPLE_CLK					RCC_APB1Periph_TIM6
//		#define 	TIMER_SAMPLE_CLK_Cmd			RCC_APB1PeriphClockCmd
//		#define		TIMER_SAMPLE_Prescaler		8399
//		#define		TIMER_SAMPLE_Period				99
		
		#define		TIMER_CONTROL							TIM7
		#define		TIMER_CONTROL_CLK					RCC_APB1Periph_TIM7
		#define 	TIMER_CONTROL_CLK_Cmd			RCC_APB1PeriphClockCmd
		#define		TIMER_CONTROL_Prescaler		8399
		#define		TIMER_CONTROL_Period			499
/**
 * @}
 */
 

void Timer_Motor(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	  NVIC_InitTypeDef NVIC_InitStruct;

    TIMER_MOTOT_CLK_Cmd(TIMER_MOTOR_CLK, ENABLE);

    TIM_TimeBaseInitStruct.TIM_Prescaler =  TIMER_MOTOR_Prescaler;
    TIM_TimeBaseInitStruct.TIM_Period = TIMER_MOTOR_Period;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMER_MOTOR, &TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIMER_MOTOR, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIMER_MOTOR, ENABLE);
		NVIC_SetPriorityGrouping(3);	
    NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIMER_MOTOR, TIM_IT_Update))
    {		
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
			Get_ENC_speed(&Motor_1,&Motor_2);

			Motor_Control();

			TIM_ClearITPendingBit(TIMER_MOTOR, TIM_IT_Update);
    }
}

void Timer_Control(void)
	{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	  NVIC_InitTypeDef NVIC_InitStruct;

    TIMER_CONTROL_CLK_Cmd(TIMER_CONTROL_CLK, ENABLE);

    TIM_TimeBaseInitStruct.TIM_Prescaler = TIMER_CONTROL_Prescaler ;
    TIM_TimeBaseInitStruct.TIM_Period = TIMER_CONTROL_Period;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMER_CONTROL, &TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIMER_CONTROL, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIMER_CONTROL, ENABLE);
		
		NVIC_SetPriorityGrouping(3);	
    NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	}

void TIM7_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIMER_CONTROL, TIM_IT_Update))
    {
			Mode_stop();
			GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
			Manual_mode();
			if (mCANstatus == sts_new_data)
			{
				GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
			}
			else GPIO_ResetBits(GPIOD, GPIO_Pin_15);
	//		CAN_TR(label_veloc, &mydata_write, 8);

      TIM_ClearITPendingBit(TIMER_CONTROL, TIM_IT_Update);	
		}
	}


