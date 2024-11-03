/********************************************************************************
 *               		                                                            *
 *  Project    		UGV_ARM				                                          			*
 *	File      		UGV_UART_IMU.h     	                                   				*
 * 	Author	    	Le Phuc Cong Thanh		                                        *
 *  Created on: 	Feb 26, 2021	                                                *
 *  Version 			V1.0	                                                       	*
 *  Descrption: 	Use USART6 to read IMU RTxQ                                   *
 *               		                                                            *
 ********************************************************************************/

 
#ifndef UGV_UART_IMU_H
#define UGV_UART_IMU_H

#include "stdint.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include <string.h>
/******************************************************************************
 *                                                                            *
 *  												DEFINE  	                                        *
 *  														                                              *
 ******************************************************************************/
#define BUFF_SIZE_USART6_Rx			100
#define G	10000
#define	g	9.80665
#define PI	3.141592654

/******************************************************************************
 *                                                                            *
 *  												TYPEDEFINE                                        *
 *  														                                              *
 ******************************************************************************/
 	typedef struct
{
	uint8_t RQx[60];
} IMU_Frame;

typedef struct
{
	float Roll;
	float	Pitch;
	float Yaw;
	
	float AccelX;
	float AccelY;
	float AccelZ;
	float GyroX;
	float GyroY;
	float GyroZ;
}IMU_t;
/******************************************************************************
 *                                                                            *
 *  												FUNCTION	                                        *
 *  														                                              *
 ******************************************************************************/
extern IMU_t IMU;
void USART6_Rx_DMA(void);
void DMA2_Stream2_IRQHandler(void);
void Get_IMU (void);
#endif 
