/********************************************************************************
 *               		                                                            *
 *  Project    		UGV_ARM																												*
 *	File      		UGV_ENC.h     	                                              *
 * 	Author	    	Le Phuc Cong Thanh		                                        *
 *  Created on: 	Feb 26, 2021	                                                *
 *  Version 			V1.0	                                                       	*
 *                                                                   		        *
 ********************************************************************************/
#ifndef UGV_ENC_H
#define UGV_ENC_H

#include "stdint.h"
#include "math.h"
#include "stdlib.h"
#include "UGV_PWM.h"
#include "UGV_CAN.h"

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



/******************************************************************************
 *                                                                            *
 *  												FUNCTION	                                        *
 *  														                                              *
 ******************************************************************************/

extern MOTOR Motor_1, Motor_2;
extern mCANdata mydata_write;
void ENC_Config(void);
void Get_ENC_speed(MOTOR* motor1,MOTOR* motor2);

#endif 
