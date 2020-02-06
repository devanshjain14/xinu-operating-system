/* ttyhandle_out.c - ttyhandle_out 
 * PL011 version
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandle_out - handle an output on a tty device by sending more
 *		    characters to the device FIFO (interrupts disabled)
 *------------------------------------------------------------------------
 */
void	ttyhandle_out(
	 struct	ttycblk	*typtr,		/* ptr to ttytab entry		*/
	 struct	uart_csreg *csrptr	/* address of UART's CSRs	*/
	)
{
	
	int32	ochars;			/* number of output chars sent	*/
					/*   to the UART		*/
	int32	avail;			/* available chars in output buf*/
	int32	uspace;			/* space left in onboard UART	*/
					/*   output FIFO		*/
  int32   imsc = 0;

	/* If output is currently held, simply ignore the call */

	if (typtr->tyoheld) {
		return;
	}

	/* If echo and output queues empty, turn off interrupts */

	if ( (typtr->tyehead == typtr->tyetail) &&
	     (semcount(typtr->tyosem) >= TY_OBUFLEN) ) {
    imsc = csrptr->imsc;
		csrptr->imsc = (imsc & ~PL011_IMSC_TXIM);
		return;
	}
	
	/* Initialize uspace to the available space in the Tx FIFO */

  //	uspace = UART_FIFO_SIZE - csrptr->txfifo_lvl;  XXX no way to read level
  //  in the PL011

  uspace = 1;

	/* While onboard FIFO is not full and the echo queue is	*/
	/* nonempty, xmit chars from the echo queue		*/

	while ( (uspace>0) &&  typtr->tyehead != typtr->tyetail) {
		csrptr->dr = *typtr->tyehead++;
		if (typtr->tyehead >= &typtr->tyebuff[TY_EBUFLEN]) {
			typtr->tyehead = typtr->tyebuff;
		}
		uspace--;
	}

	/* While onboard FIFO is not full and the output queue	*/
	/* is nonempty,	xmit chars from the output queue	*/

	ochars = 0;
	avail = TY_OBUFLEN - semcount(typtr->tyosem);
	while ( (uspace>0) &&  (avail > 0) ) {
		csrptr->dr = *typtr->tyohead++;
		if (typtr->tyohead >= &typtr->tyobuff[TY_OBUFLEN]) {
			typtr->tyohead = typtr->tyobuff;
		}
		avail--;
		uspace--;
		ochars++;
	}
	if (ochars > 0) {
		signaln(typtr->tyosem, ochars);
	}

	if ( (typtr->tyehead == typtr->tyetail) &&
	     (semcount(typtr->tyosem) >= TY_OBUFLEN) ) {
    imsc = csrptr->imsc;
		csrptr->imsc = (imsc & ~PL011_IMSC_TXIM);
	}
	return;
}
