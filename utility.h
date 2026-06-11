#ifndef _UTILITY_H
#define _UTILITY_H

#include <stdbool.h>
#include <stdint.h>

#define Version "1.21,01262021"

void HexToChar(uint16_t data,uint8_t *Buf);
uint8_t CharCopy(char *Sourcedata,uint8_t *Buf,uint8_t Maxlen);
uint8_t CharToHex(uint16_t *DecValue, uint8_t *Hexdata);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */