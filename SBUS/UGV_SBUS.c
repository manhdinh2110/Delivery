#include "UGV_SBUS.h"

/**
 * @defgroup Module Pin define
 * @{
 */
		/** 
		* @brief   SBUS Pin define 
		*/
	
/**
 * @}
 */

const uint8_t _sbusLostFrame = 0x04;
const uint8_t _sbusFailSafe = 0x08;
/*
Re-maps a number from one range to another. That is, a value of fromLow would get mapped to toLow, a value of fromHigh to toHigh, values in-between to values in-between, etc.
*/
long map(long x, long in_min, long in_max, long out_min, long out_max) 
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

bool SBUS_read(mybuffer *_sbuff, uint16_t* channels, int16_t* scalechannels, bool* failsafe, bool* lostFrame)
{
	// parse the SBUS packet
	if(_sbuff->mbuff_rx[0]==0x0F && _sbuff->mbuff_rx[24]==0x00)
	{ 
		if (channels) 
		{
			// 16 channels of 11 bit data
			channels[0]  = (uint16_t) ((_sbuff->mbuff_rx[1]     |_sbuff->mbuff_rx[2] <<8)& 0x07FF);
			channels[1]  = (uint16_t) ((_sbuff->mbuff_rx[2]>>3  |_sbuff->mbuff_rx[3] <<5)& 0x07FF);
			channels[2]  = (uint16_t) ((_sbuff->mbuff_rx[3]>>6  |_sbuff->mbuff_rx[4] <<2 |_sbuff->mbuff_rx[5]<<10)& 0x07FF);
			channels[3]  = (uint16_t) ((_sbuff->mbuff_rx[5]>>1  |_sbuff->mbuff_rx[6] <<7)& 0x07FF);
			channels[4]  = (uint16_t) ((_sbuff->mbuff_rx[6]>>4  |_sbuff->mbuff_rx[7] <<4)& 0x07FF);
			channels[5]  = (uint16_t) ((_sbuff->mbuff_rx[7]>>7  |_sbuff->mbuff_rx[8] <<1 |_sbuff->mbuff_rx[9]<<9)& 0x07FF);
			channels[6]  = (uint16_t) ((_sbuff->mbuff_rx[9]>>2  |_sbuff->mbuff_rx[10]<<6)& 0x07FF);
			channels[7]  = (uint16_t) ((_sbuff->mbuff_rx[10]>>5 |_sbuff->mbuff_rx[11]<<3)& 0x07FF);
			channels[8]  = (uint16_t) ((_sbuff->mbuff_rx[12]    |_sbuff->mbuff_rx[13]<<8)& 0x07FF);
			channels[9]  = (uint16_t) ((_sbuff->mbuff_rx[13]>>3 |_sbuff->mbuff_rx[14]<<5)& 0x07FF);
			channels[10] = (uint16_t) ((_sbuff->mbuff_rx[14]>>6 |_sbuff->mbuff_rx[15]<<2 |_sbuff->mbuff_rx[16]<<10)& 0x07FF);
			channels[11] = (uint16_t) ((_sbuff->mbuff_rx[16]>>1 |_sbuff->mbuff_rx[17]<<7)& 0x07FF);
			channels[12] = (uint16_t) ((_sbuff->mbuff_rx[17]>>4 |_sbuff->mbuff_rx[18]<<4)& 0x07FF);
			channels[13] = (uint16_t) ((_sbuff->mbuff_rx[18]>>7 |_sbuff->mbuff_rx[19]<<1 |_sbuff->mbuff_rx[20]<<9)& 0x07FF);
			channels[14] = (uint16_t) ((_sbuff->mbuff_rx[20]>>2 |_sbuff->mbuff_rx[21]<<6)& 0x07FF);
			channels[15] = (uint16_t) ((_sbuff->mbuff_rx[21]>>5 |_sbuff->mbuff_rx[22]<<3)& 0x07FF);
		  	scalechannels[0]=map(channels[0],314,1690,-20,20);	// scale -10 to 10 trai phai
			scalechannels[1]=map(channels[1],306,1693,-100,100);	// scale -100 to 100	len xuong
	    	scalechannels[2]=map(channels[2],306,1693,-115,115);	// scale -50 to 50	CAN GA
			scalechannels[3]=map(channels[3],302,1694,-100,100);	// scale -100 to 100
			scalechannels[4]=map(channels[4],306,1694,0,100);		// scale 0 to 100	 gear
			scalechannels[5]=map(channels[5],306,1694,-50,50);		// scale 0 to 100
			scalechannels[8]=map(channels[8],306,1694,0,100);		// scale 0 to 100
			scalechannels[6]=map(channels[6],306,1694,0,100);		// scale 0 to 100
			scalechannels[7]=map(channels[7],306,1694,0,100);		// scale 0 to 100
			scalechannels[8]=map(channels[8],292,1692,0,100);		// scale 0 to 100

		}
		if (lostFrame) 
		{
    	// count lost frames
    	if (_sbuff->mbuff_rx[23] & _sbusLostFrame)
				{
					*lostFrame = true;
				} 
			else 
				{
					*lostFrame = false;
				}
		}
		if (failsafe) 
			{
    	// failsafe state
    	if (_sbuff->mbuff_rx[23] & _sbusFailSafe) 
				{
      		*failsafe = true;
				}
    	else
				{
      		*failsafe = false;
				}
		}
			// return true on receiving a full packet
			return true;
  } 
	else 
	{
		// return false if a full packet is not received
//		GPIO_ResetBits(GPIOD,GPIO_Pin_15);
		return false;
	}
}

void clearScaleSbus(int16_t* _scalechannels,uint16_t* _channels)
	{
		for(uint8_t i=0;i<16;i++) 
			{
				_scalechannels[i]=0;
				_channels[i]=0;
			}
	}









