/* ttyhandler.c - ttyhandler 
 * PL011 version
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandler - Handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
void ttyhandler(uint32 xnum) {
	struct	dentry	*devptr;	/* Address of device control blk*/
	struct	ttycblk	*typtr;		/* Pointer to ttytab entry	*/	
	struct	uart_csreg *csrptr;	/* Address of UART's CSR	*/
	//uint32	iir = 0;		/* Interrupt identification	*/
	//uint32	lsr = 0;		/* Line status			*/
  uint32 mis;

	/* Get CSR address of the device (assume console for now) */

	devptr = (struct dentry *) &devtab[CONSOLE];

	csrptr = (struct uart_csreg *) devptr->dvcsr;

	/* Obtain a pointer to the tty control block */

	typtr = &ttytab[ devptr->dvminor ];

  /* Check the Masked Interrupt Status register to determine which
   * interrupts have occurred, then handle them.  */
  mis = csrptr->mis;

  resched_cntl(DEFER_START);

  if (mis & PL011_MIS_TXMIS)
    {
      /* Transmit interrupt is asserted.  If FIFOs are enabled, this
       * happens when the amount of data in the transmit FIFO is less than
       * or equal to the programmed trigger level.  If FIFOs are disabled,
       * this happens if the Tx holding register is empty.  */
      
      /* Increment number of transmit interrupts received on this UART.
       * */
      typtr->oirq++;
      
      /* Explicitly clear the transmit interrupt.  This is necessary
       * because there may not be enough bytes in the output buffer to
       * fill the FIFO greater than the transmit interrupt trigger level.
       * If FIFOs are disabled, this applies if there are 0 bytes to
       * transmit and therefore nothing to fill the Tx holding register
       * with.  */
      csrptr->icr = PL011_ICR_TXIC;
      
      /* If there are bytes pending in the output buffer, write them to
       * the UART until either there are no bytes remaining or there is no
       * space remaining in the transmit FIFO.  (If FIFOs are disabled,
       * the Tx holding register acts like a FIFO of size 1, so the code
       * still works.)  Otherwise, the UART is now idle, so set the
       * "oidle" flag, which will allow the next call to uartWrite() to
       * start transmitting again by writing a byte directly to the
       * hardware.  */
      ttyhandle_out(typtr, csrptr);
      
    }
  if (mis & PL011_MIS_RXMIS)  {
    /* Receive interrupt is asserted.  If FIFOs are enabled, this
     * happens when the amount of data in the receive FIFO is greater
     * than or equal to the programmed trigger level.  If FIFOs are
     * disabled, this happens when the Rx holding register was filled
     * with one byte.  */

    /* Increment number of receive interrupts received on this UART.  */
    typtr->iirq++;

    while (!(csrptr->fr & PL011_FR_RXFE)) {
      ttyhandle_in(typtr, csrptr);
    }
  }
  
  resched_cntl(DEFER_STOP);


}
