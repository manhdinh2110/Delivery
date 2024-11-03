#include "stm32f4xx.h"
#include "utils.h"
#include "UGV_CAN.h"
#include "UGV_UART_JOYSTICK.h"
#include "UGV_Timer.h"
#include "UGV_PWM.h"
#include "UGV_ENC.h"
#include "UGV_SBUS.h"
#include "UGV_UART_SONAR.h"
#include "stdlib.h"

void LED_Config(void);
double convert(uint8_t *string);

double convert(uint8_t *string)
{
	return atof((char*)string);
}

int main(void)
{
	LED_Config();
	ENC_Config();
	Motor_Config();

//	SONAR2_USART2_DMA_Rx_Config();
//	SONAR1_USART4_DMA_Rx_Config();
	
	
	Timer_Motor();
	Timer_Control();		

//	CAN1_Configure();
//	
//	mCANstatus = sts_no_flag;

	while(1)
	{
	}
}

void LED_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}
