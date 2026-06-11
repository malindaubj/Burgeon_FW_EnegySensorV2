#include "adc1.h"
#include "uart1.h"

uint16_t TempAnalogData[ADCInterruptSampCnt];
uint8_t NewFlag=0;
uint8_t PeckDetectStat=PeckDetectwait;
uint16_t PulsePeck=0;
uint16_t CurrentPeck=0;
uint32_t numSample = 0;           //num of transmitted data
uint16_t ADCEnergySample[ADCEnergySampleSize];
uint16_t ADCEnergySamplePoint=0;
uint16_t HighSampleADC[HighSampleBufSize];
uint8_t HighSmaplePoint;
uint8_t SampleShiftPoint;
uint8_t NewPlseEnergyFlag=0;
uint16_t RowPulseEnergy=0;
uint16_t EnergyTherhold=Therhold;
uint8_t PickSampleCnt=HighSampleBufSize;
uint8_t LowSampleCnt=0;
uint8_t NewDataFlag=0;
uint8_t ContiueBuffRead=0;

void ADC1_Initialize (void)
{
    // ASAM enabled; ADDMABM disabled; ADSIDL disabled; DONE disabled; SIMSAM Sequential; FORM Absolute decimal result, unsigned, right-justified; SAMP disabled; SSRC Internal counter ends sampling and starts conversion; AD12B 12-bit; ADON enabled; SSRCG disabled; 
    AD1CON1 = 0x84E4;
    // CSCNA disabled; VCFG0 AVDD; VCFG1 AVSS; ALTS disabled; BUFM disabled; SMPI Generates interrupt after completion of every sample/conversion operation; CHPS 1 Channel; 
    AD1CON2 = 0x001E;
    // SAMC 16; ADRC FOSC/2; ADCS 19; 
    AD1CON3 = 0x1009;
    // CH0SA AN0; CH0SB AN0; CH0NB AVSS; CH0NA AVSS; 
    AD1CHS0 = 0x00;
    // CSS25 disabled; CSS24 disabled; CSS31 disabled; CSS30 disabled; 
    AD1CSSH = 0x00;
    // CSS2 disabled; CSS1 disabled; CSS0 disabled; CSS5 disabled; CSS4 disabled; CSS3 disabled; 
    AD1CSSL = 0x00;
    // DMABL Allocates 1 word of buffer to each analog input; ADDMAEN disabled; 
    AD1CON4 = 0x00;
    // CH123SA disabled; CH123SB CH1=OA2/AN0,CH2=AN1,CH3=AN2; CH123NA disabled; CH123NB CH1=VREF-,CH2=VREF-,CH3=VREF-; 
    AD1CHS123 = 0x00;
    
    //Assign Default Callbacks
    //ADC1_SetInterruptHandler(&ADC1_CallBack);
   
    // Enabling ADC1 interrupt.
    IEC0bits.AD1IE = 1;
    IPC3bits.AD1IP = 7;
    T2CON   =  0x2018;                                                           //set up th timer 1 with 1.66us count
    PR2     =  0x5E00;                                                           // the counter overflow value is 1000 
    PR3     =  0xB2D0;       
    T2CONbits.TON=1;
}

inline static void INTERRUPT_GlobalEnable(void)
{
    __builtin_enable_interrupts();
}

void __attribute__ ( ( __interrupt__ , no_auto_psv, weak ) ) _AD1Interrupt ( void )	
{
    if(IFS0bits.AD1IF)
    {
        if (AD1CON2bits.BUFS)                                                   //check that first half is filled
        {
            TempAnalogData[0] = ADC1BUF0;                                       //load the ADC data in the buffer
            TempAnalogData[1] = ADC1BUF1;
            TempAnalogData[2] = ADC1BUF2;
            TempAnalogData[3] = ADC1BUF3;
            TempAnalogData[4] = ADC1BUF4;
            TempAnalogData[5] = ADC1BUF5;
            TempAnalogData[6] = ADC1BUF6;
            TempAnalogData[7] = ADC1BUF7;
        }
    
        else                                          //check that second half is filled
        {
            TempAnalogData[0] = ADC1BUF8;               //load the ADC data in the buffer
            TempAnalogData[1] = ADC1BUF9;
            TempAnalogData[2] = ADC1BUFA;
            TempAnalogData[3] = ADC1BUFB;
            TempAnalogData[4] = ADC1BUFC;
            TempAnalogData[5] = ADC1BUFD;
            TempAnalogData[6] = ADC1BUFE;
            TempAnalogData[7] = ADC1BUFF;
        }
        NewDataFlag=1;
        IFS0bits.AD1IF = false;
    }
}

void ADCSample()
{
    uint8_t i,SuccessSampleCnt;
    uint32_t TempnumSample,EnergyTot;
    uint32_t Tempcal;
    
    if(NewDataFlag)
    {
        for(i=0;i<ADCInterruptSampCnt;i++)
        {           
            if(ContiueBuffRead==0)
            {
                switch(PeckDetectStat)
                {      
                    case PeckDetectStart:                                       //start detect the pulse            
                        if(TempAnalogData[i] > EnergyTherhold)                  //check the pulse is over the threshold
                        {                        
                            if(ADCEnergySamplePoint<ADCEnergySampleSize)
                            {
                               ADCEnergySample[ADCEnergySamplePoint++]=TempAnalogData[i];
                            }
                            
                            else
                            {
                                ADCEnergySamplePoint=0;
                                PeckDetectStat      =   PeckDetectwait;                                
                            }
                           LowSampleCnt=0;
            
                        }
                  
                        else
                        {
                            LowSampleCnt++;
                            if(LowSampleCnt>LowSampleCntTherhold)
                            {                        
                                numSample++;
                            
                                while(ADCEnergySamplePoint>0)
                                {            
                                    for(HighSmaplePoint=0;HighSmaplePoint<PickSampleCnt;HighSmaplePoint++)
                                    {
                                        if(HighSampleADC[HighSmaplePoint]<ADCEnergySample[ADCEnergySamplePoint-1])
                                        {
                                            for(SampleShiftPoint=(PickSampleCnt-1);SampleShiftPoint>HighSmaplePoint;SampleShiftPoint--)
                                            {
                                                HighSampleADC[SampleShiftPoint]=HighSampleADC[SampleShiftPoint-1];
                                            }
                                
                                            HighSampleADC[HighSmaplePoint]=ADCEnergySample[ADCEnergySamplePoint-1];                            
                                            break;
                                        }                                    
                                    }
                                    ADCEnergySamplePoint--;
                                }
                            
                                EnergyTot=0;
                                SuccessSampleCnt=0;
                                
                                for(HighSmaplePoint=0;HighSmaplePoint<PickSampleCnt;HighSmaplePoint++)
                                {
                                    if(HighSampleADC[HighSmaplePoint]!=0)
                                    {
                                        EnergyTot+=HighSampleADC[HighSmaplePoint];
                                        SuccessSampleCnt++;
                                    }
                                }
                                
                                Tempcal=(uint32_t)SuccessSampleCnt;
                                TempnumSample=EnergyTot/Tempcal;  
                                RowPulseEnergy=(uint16_t)TempnumSample;	
                                NewPlseEnergyFlag=1;
                                PeckDetectStat      =   PeckDetectwait;                           //goto the peck detect wait
                                LowSampleCnt=0;
                            
                                for(HighSmaplePoint=0;HighSmaplePoint<HighSampleBufSize;HighSmaplePoint++)
                                {
                                    HighSampleADC[HighSmaplePoint]=0;
                                }
                                ADCEnergySamplePoint=0;
                            }   
                        }
                  
                        break;
                  
                    case PeckDetectwait:                                                      //wait for the pulse
            
                        if(TempAnalogData[i] > EnergyTherhold)                                //check the pulse is over the thershold
                        {
                            PeckDetectStat      =  PeckDetectStart;                           //goto the peck detect start
                            ADCEnergySample[ADCEnergySamplePoint++]=TempAnalogData[i];
                        }
            
                        break;
                  
                }
                
            } 
            
            else if(ContiueBuffRead==ReadProcess)
            {
                if(ADCEnergySamplePoint<ADCEnergySampleSize)
                {
                    ADCEnergySample[ADCEnergySamplePoint++]=TempAnalogData[i];
                }     
                
                else
                {
                    ContiueBuffRead=ReadFinish;
					ADCEnergySamplePoint=0;
                }
            }
            
        }
        NewDataFlag=0;
        
    }
}


uint8_t ReadContinueSamp(uint16_t *EnergyValue,uint16_t *TempIntex)
{
    *EnergyValue=0;
    *TempIntex=0;
	if(ContiueBuffRead==ReadFinish)
	{
	
		if(ADCEnergySamplePoint>=(ADCEnergySampleSize+1))
		{
			ContiueBuffRead=0;
			ADCEnergySamplePoint=0;		
		}
        
        *TempIntex=ADCEnergySamplePoint;
		*EnergyValue    =   ADCEnergySample[ADCEnergySamplePoint++];	
        return  ReadFinish; 
	}
	
	return ContiueBuffRead;
}

void ContiueBuffReadTerm()
{
    if(ContiueBuffRead)
    {
        ADCEnergySamplePoint=0;
        ContiueBuffRead=0;
    }
}

void SetContinueRead()
{
    ADCEnergySamplePoint=0;
    ContiueBuffRead=ReadProcess;
}

uint8_t NewEnergyRead()
{
	return	NewPlseEnergyFlag;
}

void EnergyRead(uint16_t *TempRowData, uint32_t *TempIndex)
{
	*TempRowData=RowPulseEnergy;
	*TempIndex=numSample;
    NewPlseEnergyFlag=0;
}

uint16_t ReadEnergyTherhold()
{
	return	EnergyTherhold;
}

uint8_t SetEnergyTherhold(uint16_t TempData)
{
    if(TempData<ADCmax)
    {
        EnergyTherhold=TempData;
        return 1;
    }
    
    return 0;
}

uint16_t ReadPickSampleCnt()
{
	return	PickSampleCnt;
}

uint8_t SetPickSampleCnt(uint16_t TempData)
{
    if(TempData<HighSampleBufSize)
    {
        PickSampleCnt=TempData;
        return 1;
    }
    return 0;
}