#include "DrvUART.h"
#include "logic.h"

void UARTOpen(T_UART_TYPE pType, uint32 pBaud, uint32 pDataLen /*DATALEN_8*/, uint32 pStopBit /*STOPBIT_1*/, uint32 pVerify /*VERIFY_NONE*/)
{
	uint32 Fdiv;
	uint32 regVal;

	NVIC_DisableIRQ(UART_IRQn);

	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16); //开启IOCON

	if (pType == E_RS485)
	{
		LPC_IOCON->PIO1_5 &= ~0x07; /*  UART RTS */
		LPC_IOCON->PIO1_5 |= 0x01;
	}

	LPC_IOCON->PIO1_6 &= ~0x07; /*  UART I/O config */
	LPC_IOCON->PIO1_6 |= 0x01;	/* UART RXD */
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01; /* UART TXD */

	/* Enable UART clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);

	LPC_SYSCON->UARTCLKDIV = 0x1; /* divided by 1 */

	regVal = LPC_SYSCON->UARTCLKDIV;
	Fdiv = ((uSystemAHBFrequency / regVal) / 16) / pBaud; /*baud rate */

	LPC_UART->LCR = 0x80 + pDataLen + (pStopBit << 2) + (pVerify << 3); /* DLAB = 0 */
	LPC_UART->DLM = Fdiv / 256;
	LPC_UART->DLL = Fdiv % 256;
	LPC_UART->LCR &= ~0x80;
	LPC_UART->FCR = 0x07; /* Enable and reset TX and RX FIFO. */

	if (pType == E_RS485)
	{
		LPC_UART->RS485CTRL = 0x30; // 配置485方向控制为RTS
	}

	/* Read to clear the line status. */
	regVal = LPC_UART->LSR;
	/* Ensure a clean start, no data in either TX or RX FIFO. */
	while ((LPC_UART->LSR & (LSR_THRE | LSR_TEMT)) != (LSR_THRE | LSR_TEMT))
		;
	while (LPC_UART->LSR & LSR_RDR)
		regVal = LPC_UART->RBR; /* Dump data from RX FIFO */

	LPC_UART->IER = IER_RBR /* | IER_THRE | IER_RLS*/; /* Enable UART Tx & Rx interrupt */

	/* Enable the UART Interrupt */
	NVIC_EnableIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 2);
}

void UARTClose(void)
{
	LPC_UART->IER = 0;
	NVIC_DisableIRQ(UART_IRQn);
}

uint8 UARTReceive(void)
{
	uint8 rcvData;
	while (!(LPC_UART->LSR & LSR_RDR))
		;					 // 查询数据是否接收完毕
	rcvData = LPC_UART->RBR; // 接收数据
	return (rcvData);
}

void UARTSend(uint8 *BufferPtr, uint32 Length)
{
	while (Length != 0)
	{
		while (!(LPC_UART->LSR & LSR_THRE))
			;
		LPC_UART->THR = *BufferPtr;
		BufferPtr++;
		Length--;
	}
	return;
}

void UART_IRQHandler(void)
{
	uint8 IIRValue, LSRValue;
	IIRValue = LPC_UART->IIR;
	IIRValue >>= 1;			 /* skip pending bit in IIR */
	IIRValue &= 0x07;		 /* check bit 1~3, interrupt identification */
	if (IIRValue == IIR_RLS) /* Receive Line Status */
	{
		LSRValue = LPC_UART->LSR; /* Receive Line Status */
		if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI))
		{
			LPC_UART->RBR; /* Dummy read on RX to clear interrupt, then bail out */
		}
		if (LSRValue & LSR_RDR) /* Receive Data Ready */
		{
			LPC_UART->RBR;
		}
	}
	else if (IIRValue == IIR_RDA) /* Receive Data Available */
	{
		/* Receive Data Available */
		Callback_UartReceived(LPC_UART->RBR); //接收的1byte
	}
	else if (IIRValue == IIR_CTI) /* Character timeout indicator */
	{
		/* Character Time-out indicator , Bit 9 as the CTI error */
	}
	else if (IIRValue == IIR_THRE) /* THRE, transmit holding register empty */
	{
		/* THRE interrupt */
		LSRValue = LPC_UART->LSR; /* Check status in the LSR to see if valid data in U0THR or not */
		if (LSRValue & LSR_THRE)
		{
			//UART待发送的数据全部发送完毕
		}
	}
}

//CAN接收到一个字节后立即执行Callback_UartReceived，可以在此函数中增加接收时间等逻辑判断
void Callback_UartReceived(uint8 pData)
{
	RecvRawDataFromController(E_CONTROLLER_PORT_UART, pData);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
