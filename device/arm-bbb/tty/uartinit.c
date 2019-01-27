#include <xinu.h>

int32 uartinit(struct	uart_csreg *uptr) {

/* Set baud rate */
	uptr->lcr = UART_LCR_DLAB;
	uptr->dlm = UART_DLM;
	uptr->dll = UART_DLL;

	uptr->lcr = UART_LCR_8N1;	/* 8 bit char, No Parity, 1 Stop*/
	uptr->fcr = 0x00;		/* Disable FIFO for now		*/

  /* Enable interrupts on the device: reset the transmit and	*/
	/*   receive FIFOS, and set the interrupt trigger level		*/

	uptr->fcr = UART_FCR_EFIFO | UART_FCR_RRESET |
			UART_FCR_TRESET | UART_FCR_TRIG2;

	/* UART must be in 16x mode (TI AM335X specific) */

	uptr->mdr1 = UART_MDR1_16X;

	return (OK);

}
