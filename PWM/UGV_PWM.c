#include "stm32f4xx.h"
#include "UGV_ENC.h"
#include "UGV_PWM.h"

/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   PWM Pin define 
		*/
		#define		PWM_CLK_Cmd		  								 		RCC_AHB1PeriphClockCmd
		#define		PWM_CLK															RCC_AHB1Periph_GPIOE
		#define 	PWM_CH1_PORT 					 						 	GPIOE
		#define		PWM_CH1_PIN													GPIO_Pin_9
		#define 	PWM_CH1_SOURCE									 		GPIO_PinSource9
		#define 	PWM_CH1N_PORT 					 						GPIOE
		#define		PWM_CH1N_PIN												GPIO_Pin_8
		#define 	PWM_CH1N_SOURCE									 		GPIO_PinSource8
		#define 	PWM_CH2_PORT 					 						 	GPIOE
		#define		PWM_CH2_PIN													GPIO_Pin_11
		#define 	PWM_CH2_SOURCE									 		GPIO_PinSource11
		#define 	PWM_CH2N_PORT 					 						GPIOE
		#define		PWM_CH2N_PIN												GPIO_Pin_10
		#define 	PWM_CH2N_SOURCE									 		GPIO_PinSource10
		
		#define		PWM_TIM_AF													GPIO_AF_TIM1
		#define		PWM_TIM															TIM1
		#define 	PWM_TIM_CLK													RCC_APB2Periph_TIM1
		#define		PWM_TIM_CLK_Cmd											RCC_APB2PeriphClockCmd
		#define		PWM_TIM_Prescaler										0
		#define		PWM_TIM_Period											8399
		#define		PWM_TIM_ClockDivision								0
		#define		PWM_TIM_RepetitionCounter						0
		#define		PWM_TIM_Deadtime										11
/**
 * @}
 */


MOTOR Motor_1 ={0,0,0,0};
MOTOR Motor_2 ={0,0,0,0};
CAR Car;
PID PID_1={1.0,0.3,0.2,0,0,0,0,0};
PID PID_2={1.0,0.3,0.2,0,0,0,0,0};
void Motor_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	/* Enable GPIO clock */
	PWM_CLK_Cmd(PWM_CLK, ENABLE);
	/* TIM1 clock enable */
  PWM_TIM_CLK_Cmd(PWM_TIM_CLK, ENABLE);
	
	/* GPIOE Configuration: Channel 2 as alternate function push-pull */ 
  GPIO_InitStructure.GPIO_Pin = PWM_CH1N_PIN | PWM_CH1_PIN | PWM_CH2N_PIN | PWM_CH2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
  GPIO_PinAFConfig(PWM_CH1_PORT , PWM_CH1_SOURCE , PWM_TIM_AF);
	GPIO_PinAFConfig(PWM_CH1N_PORT, PWM_CH1N_SOURCE, PWM_TIM_AF);
  GPIO_PinAFConfig(PWM_CH2_PORT , PWM_CH2_SOURCE , PWM_TIM_AF);
	GPIO_PinAFConfig(PWM_CH2N_PORT, PWM_CH2N_SOURCE, PWM_TIM_AF);
	
	/* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = PWM_TIM_Prescaler;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = PWM_TIM_Period;
  TIM_TimeBaseStructure.TIM_ClockDivision = PWM_TIM_ClockDivision;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = PWM_TIM_RepetitionCounter;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	/* Channel 1 and 2 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0; /* TIM1->CCR1;*/
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 0; /*TIM1->CCR2;*/
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	  /* Automatic Output enable, Break, dead time and lock configuration*/
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
  TIM_BDTRInitStructure.TIM_DeadTime = PWM_TIM_Deadtime;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;

  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	
	/* TIM1 counter enable */
  TIM_Cmd(PWM_TIM, ENABLE);
	
	/* Main Output Enable */
  TIM_CtrlPWMOutputs(PWM_TIM, ENABLE);
}

void Motor_Control(void)
	{
//		Motor_1.Output = (Motor_Control_Speed(&PID_1, Motor_1.speed, Car.v_left) + 8400)/2;
//		Motor_2.Output = (Motor_Control_Speed(&PID_2, Motor_2.speed, Car.v_right) + 8400)/2;

//		if (Motor_1.Output > max)
//			Motor_1.Output = max;
//		else if (Motor_1.Output < min)
//			Motor_1.Output = min;

//		if (Motor_2.Output > max)
//			Motor_2.Output = max ;
//		else if (Motor_2.Output < min)
//			Motor_2.Output = min ;
//		
//		TIM1->CCR1 = (uint32_t)Motor_1.Output;
//		TIM1->CCR2 = (uint32_t)Motor_2.Output;
				TIM1->CCR1 = 5000;
		TIM1->CCR2 = 4000;
	}	


int32_t Motor_Control_Speed(PID* pid, double current,double setpoint)
	{
		pid->Error =setpoint-current;
		pid->P_part = pid->Kp*pid->Error;
		pid->I_part = 0.5*pid->Ki*T_Sample*(pid->Error + pid->pre_Error);
		pid->D_part = pid->Kd/T_Sample*( pid->Error - 2*pid->pre_Error+pid->pre_pre_Error);
		pid->Output = pid->pre_Output + pid->P_part + pid->I_part + pid->D_part ;
		pid->pre_pre_Error = pid->pre_Error;
		pid->pre_Error = pid->Error;
		if (pid->Output>duty_max)
			pid->Output=duty_max;	
		if (pid->Output<duty_min)
		pid->Output=duty_min;	
		pid->pre_Output = pid->Output;
		return pid->Output;
	}
	