/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _ADC1_H
#define _ADC1_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h> 

#define Therhold                100                                                //threshold value for ADC raw value
#define LowSampleCntTherhold    50
#define ADCInterruptSampCnt     8                                                 //sample count for single interrupt

#define ADCmax                  4095
#define PeckDetectStart         0
#define PeckDetectwait          1
#define SampleBufferSize        110
#define HighSampleBufSize       20
#define MedianLoc           (HighSampleBufSize>>1)
#define ADCEnergySampleSize           8000
#define ReadProcess             1
#define ReadFinish              2



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    void ADC1_Initialize (void);
    void ADCSample();
    uint8_t ReadContinueSamp(uint16_t *EnergyValue,uint16_t *TempIntex);
    void ContiueBuffReadTerm();
    void SetContinueRead();
    uint8_t NewEnergyRead();
    void EnergyRead(uint16_t *TempRowData, uint32_t *TempIndex);
    uint16_t ReadEnergyTherhold();
    uint8_t SetEnergyTherhold(uint16_t TempData);
    uint16_t ReadPickSampleCnt();
    uint8_t SetPickSampleCnt(uint16_t TempData);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

