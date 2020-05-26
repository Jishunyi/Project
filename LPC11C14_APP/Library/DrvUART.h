#ifndef __UART_H 
#define __UART_H

#include "DrvGPIO.h"

#define IER_RBR         (0x01<<0)
#define IER_THRE        (0x01<<1)
#define IER_RLS         (0x01<<2)

#define IIR_PEND        0x01
#define IIR_RLS         0x03
#define IIR_RDA         0x02
#define IIR_CTI         0x06
#define IIR_THRE        0x01

#define LSR_RDR         (0x01<<0)
#define LSR_OE          (0x01<<1)
#define LSR_PE          (0x01<<2)
#define LSR_FE          (0x01<<3)
#define LSR_BI          (0x01<<4)
#define LSR_THRE        (0x01<<5)
#define LSR_TEMT        (0x01<<6)
#define LSR_RXFE        (0x01<<7)

#define DATALEN_5				(0x00)
#define DATALEN_6				(0x01)
#define DATALEN_7				(0x02)
#define DATALEN_8				(0x03)

#define STOPBIT_1				(0x00)
#define STOPBIT_2				(0x01)

typedef enum{
	E_RS232 = 0,
	E_RS485
} T_UART_TYPE;

void UARTOpen(T_UART_TYPE pType, uint32 pBaud, uint32 pDataLen/*DATALEN_8*/, uint32 pStopBit/*STOPBIT_1*/, uint32 pVerify/*VERIFY_NONE*/);
void UARTClose(void);

void UART_IRQHandler(void);
uint8 UARTReceive(void);
void UARTSend(uint8 *BufferPtr, uint32 Length);	

void Callback_UartReceived(uint8 pData);

#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
