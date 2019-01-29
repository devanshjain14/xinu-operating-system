/* ttykickout.c - ttykickout 
 * PL011 version
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttykickout  -  "Kick" the hardware for a tty device, causing it to
 *		     generate an output interrupt (interrupts disabled)
 *------------------------------------------------------------------------
 */
void	ttykickout(
	 struct uart_csreg *csrptr	/* Address of UART's CSRs	*/
	)
{
	/* Force the UART hardware generate an output interrupt */

	csrptr->imsc = csrptr->imsc | PL011_IMSC_TXIM;
  csrptr->cr = csrptr->cr | PL011_CR_TXE;
	return;
}
