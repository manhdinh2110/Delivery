/******************************************************************************
 *                                                                            *
 *  @Project    Dev_Base				                                              *
 *	@File       Define.h		                                                  *
 * 	@Author	    Nguyen Minh Chuong                                               *
 *  @Version 		V1.1	                                                       	*
 *                                                                            *
 ******************************************************************************/
#ifndef __DEFINE_H
#define __DEFINE_H
/******************************************************************************
 *                                                                            *
 *  												INCLUDE		                                        *
 *  														                                              *
 ******************************************************************************/
#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
/******************************************************************************
 *                                                                            *
 *  												TYPEDEF		                                        *
 *  														                                              *
 ******************************************************************************/
 
 /* Struct */

 typedef struct
 {
	 uint32_t timpos;
	 uint32_t timneg;
	 uint32_t interval;
	 double   time_ms;
 }CAPTURE_t;
 /* Enum */
 typedef enum
 {
    BIT0 = (uint8_t)0,
    BIT1 = (uint8_t)1
 }BIT;
 
 /* Data Type */
 #define NULL    0
 
 #define FALSE   0
 #define TRUE    1
 typedef float real32_t;
 typedef double real64_t;
 
/******************************************************************************
 *                                                                            *
 *  												DEFINE		                                        *
 *  														                                              *
 ******************************************************************************/


//#define _IMU_I2C_			1

#endif /* _DEFINE_H */
