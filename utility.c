#include <xc.h>
#include "utility.h"

void HexToChar(uint16_t data,uint8_t *Buf)
{
   uint8_t UserTXBuf;
   uint16_t temp;
   uint8_t j= 0;   
 
   for(j=0;j<4;j++)
   {
      temp        =     data  &  0xF000;                                        //get the hex number
      temp        >>=   12;
      UserTXBuf   =     (uint8_t)temp;
      
      if(UserTXBuf   <  10)                                                     //if value is decimal
      {
         UserTXBuf += 48;                                                       //convert to char
      }
      
      else
      {
         UserTXBuf += 55;                                                       //convert to Hex char
      }
      
      data  <<=   4;                                                            //remove the lower 4 bit;
      //UART1_Write(UserTXBuf);                                                   //sent to UART
	  *Buf=UserTXBuf;
	  Buf++;
   }

}

uint8_t CharCopy(char *Sourcedata,uint8_t *Buf,uint8_t Maxlen)
{
   uint8_t j= 0;  
   
   while(*Sourcedata!='\0')
   {
       j++;       
       if(j>=Maxlen)
       {
           return 0;
       }
       *Buf=*Sourcedata;
       Buf++;
       Sourcedata++;       
   }
   
   return j;
}

uint8_t CharToHex(uint16_t *DecValue, uint8_t *Hexdata)
{
	uint8_t i = 0;
	uint16_t temp = 0;

	for(i = 0; i < 4; i++)
	{
		temp*=16;																//shift to upper 4 bit

		if(Hexdata[i] >= '0' && Hexdata[i]<= '9')									//check that it is digit range
		{
			temp += Hexdata[i] - '0';											//convert to decimal
		}

		else if (Hexdata[i] >= 'A' && Hexdata[i] <= 'F')							//Letter range
		{
			temp += (Hexdata[i] - 55);											//convert to decimal
		}

		else
		{
			return 0;															//Error return
		}
	}

	*DecValue	=	temp;
	return 1;																	//indicate that it was success
}