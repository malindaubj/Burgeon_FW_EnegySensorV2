/*
 * File:   uart1.c
 * Author: EE
 *
 * Created on October 29, 2020, 5:50 AM
 */


#include <xc.h>
#include "uart1.h"
#include "adc1.h"
#include "utility.h"
uint8_t TempTXBufPoint=0;
uint8_t TXBufPoint  =0;
uint8_t UserRXArray[RXBufSize];
uint8_t UserTXArray[TXBufSize];
uint8_t UserDataReady=0;
uint8_t RXBufPoint=0;
uint8_t ContineMode=1;

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1TXInterrupt ( void )
{
	IFS0bits.U1TXIF = 0;                                                        //tx interrupt flag 
	while(U1STAbits.UTXBF == 0)                                                 //tx buffer full
	{
		if(TempTXBufPoint>=TXBufPoint)
		{
			TXBufPoint		=	0;
            TempTXBufPoint  =   0;
			IEC0bits.U1TXIE =	0;
			break;
		}			
		U1TXREG	=	UserTXArray[TempTXBufPoint++];
	}
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{
	uint8_t TempRX;
  
    IFS0bits.U1RXIF = 0;                                                        //rx interrupt flag
	
    while((U1STAbits.URXDA == 1))                                               //rx buffer data available
    {
        TempRX	=	U1RXREG;                                                    //rx data load to buffer
		if(UserDataReady==0)
		{
			if(TempRX==RXStartChar)                                             //check that start charater is recived
			{
				RXBufPoint=0;                                                   //reset the reciver buffer
			}
			
			UserRXArray[RXBufPoint]	=	TempRX;                                 //load the data to array
			
			if(TempRX==RXEndChar)                                               //check that end char is recived
			{
				if((RXBufPoint>=(MinPacketSize-1))&&(UserRXArray[0]==RXStartChar))//check that minimum packet size an start char is ok
				{
#ifdef RXPreEndChar
					if(UserRXArray[RXBufPoint-1]==RXPreEndChar)                 //check that previous char is reached
					{
#endif
						UserDataReady=1;
						break;
#ifdef RXPreEndChar
					}
#endif
				}
				
				RXBufPoint=0;
			}
			
			else
			{
				RXBufPoint++;
				if(RXBufPoint>=RXBufSize)                                       //check that buffer pointer is overfolw
				{
					RXBufPoint=0;                                               //reset the pointer
				}
			}
		}
	}
	
}




void UART1_Initialize(void)
{
    IEC0bits.U1TXIE = 0;
    U1MODE = (0x8008 & ~(1<<15));                                               // disabling UARTEN bit
                                                                                // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U1STA = 0x00;
                                                                                // BaudRate = 115200; Frequency = 60000000 Hz; BRG 129; 
    U1BRG = 0x81;
    
	IEC0bits.U1RXIE = 1;                                                        //interrupt enabled
    
    U1MODEbits.UARTEN = 1;                                                      // enabling UART ON bit
    U1STAbits.UTXEN = 1;
}



void InterfaceProcess()
{
	uint16_t	TempData1,TempData3;
    uint8_t     Temp;
	uint32_t	TempData2;
    
	if((ContineMode)&&(NewEnergyRead()))                                        //check for continue mode and new data
	{		
		if(TXBufPoint==0)
		{                                                                       //load the data to txarray
			UserTXArray[0]=TXCNTUStartChar;
			TXBufPoint=1;
			EnergyRead(&TempData1,&TempData2);
			HexToChar(TempData1,&UserTXArray[TXBufPoint]);
			TXBufPoint+=4;
			UserTXArray[TXBufPoint++]=TXSeperator;			
			TempData1=(uint16_t)TempData2;
			TempData2>>=16;
			HexToChar((uint16_t)TempData2,&UserTXArray[TXBufPoint]);
			TXBufPoint+=4;
			HexToChar(TempData1,&UserTXArray[TXBufPoint]);			
			TXBufPoint+=4;
			
#ifdef RXPreEndChar	
			UserTXArray[TXBufPoint++]=RXPreEndChar;
#endif
			UserTXArray[TXBufPoint++]=RXEndChar;
			IEC0bits.U1TXIE = 1;                                                //fire tx interrupt
		}
		
	}   
	
	else if(UserDataReady)                                                      //rx command ready
	{
		if(TXBufPoint==0)
		{
			TempData1=CMDCONCATANATE(UserRXArray[1], UserRXArray[2]);           
			UserTXArray[0]=TXNORMStartChar;
			UserTXArray[1]=UserRXArray[1];
			UserTXArray[2]=UserRXArray[2];
			UserTXArray[3]=TXSeperator;
			TXBufPoint=4;
			switch(TempData1)
			{
				case MO:			
					if(UserRXArray[3]==RXSeperator)
					{                                                           //select or deselect the continuous mode 
						if(UserRXArray[4]=='1')
						{
                            Temp=ReadContinueSamp(&TempData1,&TempData3);       
                            if(Temp==0)                                         //continuous mode
                            {
                                ContineMode=1;		
                                UserTXArray[TXBufPoint++]='1';
                            }
                            
                            else
                            {
                                UserTXArray[3]='!';	
                            }
						}
					
						else if(UserRXArray[4]=='0')                            //command mode
						{	
						
							ContineMode=0;
							UserTXArray[TXBufPoint++]='0';
						}
					
						else
						{
							UserTXArray[3]='!';													
						}
					}

#ifdef RXPreEndChar				
					else if(UserRXArray[3]==RXPreEndChar)                       //request the mode status
#else	
					else if(UserRXArray[3]==RXEndChar)
#endif
					{
						//UserTXArray[3]=TXSeperator;
						if(ContineMode)
						{
							UserTXArray[TXBufPoint++]='1';						
						}
					
						else
						{
							UserTXArray[TXBufPoint++]='0';
						}
					}
				
					else
					{
						UserTXArray[3]='!';
					}
				
					break;
					
				case AE:                                                        //request Energy 
					EnergyRead(&TempData1,&TempData2);
					HexToChar(TempData1,&UserTXArray[TXBufPoint]);
					TXBufPoint+=4;
					break;
				
				case ND:                                                        //check new data received				
					if(NewEnergyRead())
					{
						UserTXArray[TXBufPoint++]='1';
					}
				
					else
					{
						UserTXArray[TXBufPoint++]='0';
					}
				
					break;
				
				case ET:                                                        //set the threshold value	
					if(UserRXArray[3]==RXSeperator)
					{
#ifdef RXPreEndChar				
						if(UserRXArray[8]!=RXPreEndChar)                        //energy value with 4 chars
#else
						if(UserRXArray[8]!=RXEndChar)
#endif
						{
							UserTXArray[3]='!';
						}
					
						else
						{
						
							if(CharToHex(&TempData1,&UserRXArray[4]))           //rcv data convert to decimal
							{
								if(SetEnergyTherhold(TempData1))                //set the threshold value with rcved data
                                {
                                    TempData1=ReadEnergyTherhold();             //read the current energy val
                                    HexToChar(TempData1,&UserTXArray[TXBufPoint]);  //pass to tx buffer
                                    TXBufPoint+=4;
                                }
                                
                                else
                                {
                                    UserTXArray[3]='!';                         //error
                                }
							}
							else
							{
								UserTXArray[3]='!';                             //error
							}
						
						}
					
					}
				
#ifdef RXPreEndChar				
					else if(UserRXArray[3]==RXPreEndChar)
#else
					else if(UserRXArray[3]==RXEndChar)
#endif
					{                                                           //request current threshold val
						TempData1=ReadEnergyTherhold();
						HexToChar(TempData1,&UserTXArray[TXBufPoint]);          //pass to tx buffer
						TXBufPoint+=4;		
					}
				
					else
					{
						UserTXArray[3]='!';                                     //error
					}
				
					break;				
							
				case PS:                                                        //set the num of samples for averaging
					if(UserRXArray[3]==RXSeperator)
					{
#ifdef RXPreEndChar				
						if(UserRXArray[8]!=RXPreEndChar)                    
#else
						if(UserRXArray[8]!=RXEndChar)
#endif
						{
							UserTXArray[3]='!';
						}
					
						else
						{
							if(CharToHex(&TempData1,&UserRXArray[4]))           //convert rcved count char to hex 
							{
								if(SetPickSampleCnt(TempData1))                 //set the sample count number
                                {
                                    TempData1=ReadPickSampleCnt();              //read the set sample count number
                                    HexToChar(TempData1,&UserTXArray[TXBufPoint]);   //load to tx buffer
                                    TXBufPoint+=4;
                                }
                                
                                else
                                {
                                    UserTXArray[3]='!';                                    
                                }
							}
						
							else
							{
								UserTXArray[3]='!';
							}
						
						}
					
					}
				
#ifdef RXPreEndChar				
					else if(UserRXArray[3]==RXPreEndChar)
#else
					else if(UserRXArray[3]==RXEndChar)
#endif
					{                                                           //request the sample count number
						TempData1=ReadPickSampleCnt();
						HexToChar(TempData1,&UserTXArray[TXBufPoint]);
						TXBufPoint+=4;		
					}
				
					else
					{
						UserTXArray[3]='!';
					}
				
					break;
                    
                case CR:                                                        //coutinuous mode options 
                    if((UserRXArray[3]==RXSeperator)&&(ContineMode==0))
                    {
                        if(UserRXArray[4]=='S')
                        {
                            SetContinueRead();
                            UserTXArray[TXBufPoint++]='S';                            
                        }
                        
                        else if(UserRXArray[4]=='R')
                        {
                            Temp=ReadContinueSamp(&TempData1,&TempData3);
                            if(Temp==ReadProcess)
                            {
                                UserTXArray[TXBufPoint++]='W';                                
                            }
                            
                            else if(Temp==ReadFinish)
                            {
                                HexToChar(TempData1,&UserTXArray[TXBufPoint]);
                                TXBufPoint+=4;
                                UserTXArray[TXBufPoint++]=TXSeperator;
                                HexToChar(TempData3,&UserTXArray[TXBufPoint]);
                                TXBufPoint+=4;
                            }
                            
                            else
                            {
                                UserTXArray[3]='!';                                
                            }                                                        
                        }
                        
                        else if(UserRXArray[4]=='T')
                        {
                            ContiueBuffReadTerm();
                            UserTXArray[TXBufPoint++]='T';
                        }
                        
                        else
                        {
                            UserTXArray[3]='!';
                        }                      
                        
                    }
                    
                    else
                    {
                        UserTXArray[3]='!';
                    }
                    
                    break;   
                    
                case VE:                                                        //request current firmware version
                    
#ifdef RXPreEndChar				
                    if(UserRXArray[3]==RXPreEndChar)
#else
					else if(UserRXArray[3]==RXEndChar)
#endif
					{
                        Temp=CharCopy((char *)Version,&UserTXArray[TXBufPoint],TXBufSize-7);
                        if(Temp==0)
                        {
                            UserTXArray[3]='!';
                        }
                        
                        else
                        {
                            TXBufPoint+=Temp;
                        }	
					}
				
					break;
				
				default:
					UserTXArray[3]='!';
					break;
			
			}		
		
#ifdef RXPreEndChar	
			UserTXArray[TXBufPoint++]=RXPreEndChar;
#endif

			UserTXArray[TXBufPoint++]=RXEndChar;
			IEC0bits.U1TXIE = 	1;                                              //tx interrupt enable
            RXBufPoint=0;
			UserDataReady	=	0;
		}
	}
}
