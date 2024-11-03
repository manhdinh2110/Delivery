/********************************************************************************
 *               		                                                            *
 *  Project    		UGV_ARM				                                          			*
 *	File      		UGV_UART.h     	                                   						*
 * 	Author	    	Le Phuc Cong Thanh		                                        *
 *  Created on: 	Feb 26, 2021	                                                *
 *  Version 			V1.0	                                                       	*
 *  Descrption: 	Use USART3 to read Joystick                                   *
 *               		                                                            *
 ********************************************************************************/

 
#ifndef UGV_UART1_H
#define UGV_UART1_H

#include "stdint.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "UGV_UART_JOYSTICK.h"
#include <string.h>
/******************************************************************************
 *                                                                            *
 *  												DEFINE  	                                        *
 *  														                                              *
 ******************************************************************************/

/******************************************************************************
 *                                                                            *
 *  												TYPEDEFINE                                        *
 *  														                                              *
 ******************************************************************************/
typedef struct 
	{
		 uint16_t counter;
		 bool flag;
		 uint8_t mbuff_rx[15];
		 uint8_t mbuff_tx[15];
	} mybuffer_sonar;	

typedef struct
	{
		double range_left;
		double range_right;
		double range;
	}	range_sonar;
/******************************************************************************
 *                                                                            *
 *  												FUNCTION	                                        *
 *  														                                              *
 ******************************************************************************/
extern range_sonar range_SONAR;
extern mybuffer_sonar uart_buffer_u2,uart_buffer_u4;
extern int mode_stop;
void SONAR1_USART4_DMA_Rx_Config (void);
void SONAR2_USART2_DMA_Rx_Config (void);
void Mode_stop(void);
double Get_range_from_sonar(mybuffer_sonar *buffer);

	
#endif 
