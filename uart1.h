#ifndef _UART1_H
#define _UART1_H

#include <stdbool.h>
#include <stdint.h>

#define RXBufSize 25
#define TXBufSize 25
#define RXStartChar ':'
#define RXEndChar '\n'
#define RXPreEndChar '\r'
#define TXCNTUStartChar ':'
#define TXNORMStartChar '>'
#define TXSeperator ','
#define RXSeperator ','
#define	MinPacketSize	3

#define		CMDCONCATANATE(CMD1,CMD2)	(CMD1<<8)|CMD2							//command char concatanate

#define	MO			(('M')<<8)|('O')											//Mode change
#define	VE			(('V')<<8)|('E')											//version command
#define	AE			(('A')<<8)|('E')											//energy read
#define	ND			(('N')<<8)|('D')											//Read new data flag											
#define	ET			(('E')<<8)|('T')											//Energy Threshold 
#define	PS			(('P')<<8)|('S')											//Pulse count for peack read
#define	CR			(('C')<<8)|('R')											//continues read
#define	HP			(('H')<<8)|('P')											//Check the communication

void UART1_Initialize(void);
void InterfaceProcess();


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

