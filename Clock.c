#include <stdint.h>
//#include "xc.h"
#include "clock.h"

void CLOCK_Initialize(void)
{
    // FRCDIV FRC/2; PLLPRE 3; DOZE 1:8; PLLPOST 1:2; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3101;
    // TUN Center frequency; 
    OSCTUN = 0x00;
    // ROON disabled; ROSEL FOSC; RODIV 0; ROSSLP disabled; 
    REFOCON = 0x00;
    // PLLDIV 58; 
    PLLFBD = 0x3A;
    // AD1MD enabled; PWMMD enabled; T3MD enabled; T4MD enabled; T1MD enabled; U2MD enabled; T2MD enabled; U1MD enabled; QEI1MD enabled; SPI2MD enabled; SPI1MD enabled; T5MD enabled; I2C1MD enabled; 
    PMD1 = 0x00;
    // IC4MD enabled; IC3MD enabled; OC1MD enabled; IC2MD enabled; OC2MD enabled; IC1MD enabled; OC3MD enabled; OC4MD enabled; 
    PMD2 = 0x00;
    // CMPMD enabled; CRCMD enabled; I2C2MD enabled; 
    PMD3 = 0x00;
    // CTMUMD enabled; REFOMD enabled; 
    PMD4 = 0x00;
    // PWM2MD enabled; PWM1MD enabled; PWM3MD enabled; 
    PMD6 = 0x00;
    // PTGMD enabled; DMA0MD enabled; 
    PMD7 = 0x00;
    // CF no clock failure; NOSC PRIPLL; CLKLOCK unlocked; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) (0x03));
    __builtin_write_OSCCONL((uint8_t) (0x01));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
}
