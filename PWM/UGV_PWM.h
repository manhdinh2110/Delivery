/********************************************************************************
 *               		                                                            *
 *  Project    		UGV_ARM							                                          *
 *	File      		UGV_PWM.h     	                                              *
 * 	Author	    	Le Phuc Cong Thanh		                                        *
 *  Created on: 	Feb 26, 2021	                                                *
 *  Version 			V1.0	                                                       	*
 *                                                                   		        *
 ********************************************************************************/
#ifndef UGV_PWM_H
#define UGV_PWM_H

#include "stdint.h"
#include "math.h"
#include "stdlib.h"

/******************************************************************************
 *                                                                            *
 *  												DEFINE  	                                        *
 *  														                                              *
 ******************************************************************************/

#define T_Sample 		0.01
#define PI 					3.141592654
#define duty_max 		8399  //8399
#define duty_min 		-8400  //-8399
#define max 				7980
#define min 				420
#define max_pulse	 	4000

/******************************************************************************
 *                                                                            *
 *  												TYPEDEFINE                                        *
 *  														                                              *
 ******************************************************************************/

typedef struct
	{
		double Kp;
		double Ki;
		double Kd; 
		double P_part;
		double I_part;
		double D_part;
		double Error;
		double pre_Error;
		double pre_pre_Error;
		double Output;
		double pre_Output; 
//			double Kp;
//			double Ki;
//			double Kd; 
//			double Error;
//			double pre_Error;
//			double delta_Error;
//			double Sum_Error;
//			double Output; 
	} PID;

typedef struct
	{
		int32_t pulse;
		int32_t delta_pulse;
		double speed;
		int32_t Output;
	}	MOTOR;

typedef struct
	{
		double v_car;
		double v_offset;
		double v_left;
		double v_right;
	} CAR;



/******************************************************************************
 *                                                                            *
 *  												FUNCTION	                                        *
 *  														                                              *
 ******************************************************************************/

extern PID PID_1, PID_2, PID_IMU;
extern CAR Car;
extern MOTOR Motor_1, Motor_2;


void Motor_Config(void);
void Motor_Control(void);
int32_t Motor_Control_Speed(PID* pid, double current,double setpoint);
	
#endif 
