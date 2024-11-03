/********************************************************************************
 *               		                                                            *
 *  Project    		UGV_ARM				                                          			*
 *	File      		UGV_CAN.h     	                                              *
 * 	Author	    	Le Phuc Cong Thanh		                                        *
 *  Created on: 	Feb 26, 2021	                                                *
 *  Version 			V1.0	                                                       	*
 *                                                                   		        *
 ********************************************************************************/
#ifndef UGV_CAN_H
#define UGV_CAN_H
#include "stm32f4xx.h"

/******************************************************************************
 *                                                                            *
 *  												DEFINE  	                                        *
 *  														                                              *
 ******************************************************************************/

#define label_veloc 0x001

/******************************************************************************
 *                                                                            *
 *  												TYPEDEFINE                                        *
 *  														                                              *
 ******************************************************************************/

typedef union{
	float mf[2];	
	double md;
	uint8_t mb[8];
} mCANdata;
 
typedef enum{
	sts_new_data,
	sts_no_flag,
}mDatastatus;


/******************************************************************************
 *                                                                            *
 *  												FUNCTION	                                        *
 *  														                                              *
 ******************************************************************************/
 

extern mDatastatus mCANstatus;

void CAN1_Configure(void);
void CAN1_Send(int _IDstd, mCANdata _data, uint8_t _length);
#endif
