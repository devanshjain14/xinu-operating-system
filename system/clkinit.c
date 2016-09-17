/* clkinit.c - clkinit for Qemu versatilepb/arm1176 */

#include <xinu.h>
#include <platform.h>
uint32	clktime;		/* Seconds since boot			*/
uint32	ctr1000 = 0;		/* Milliseconds since boot		*/
qid16	sleepq;			/* Queue of sleeping processes		*/
uint32	preempt;		/* Preemption counter			*/
volatile ulong clkticks;
/*------------------------------------------------------------------------
 * clkinit  -  Initialize the clock and sleep queue at startup
 *------------------------------------------------------------------------
 */
void	clkinit(void)
{
	
	kprintf("Time base %dHz, Clock ticks at %dHz\r\n",
        platform.clkfreq, CLKTICKS_PER_SEC);
	interruptVector[IRQ_TIMER] = clkhandler;
	enable_irq(IRQ_TIMER);
	clkupdate(platform.clkfreq / CLKTICKS_PER_SEC);
	sleepq = newqueue();	/* Allocate a queue to hold the delta	*/
				/*   list of sleeping processes		*/

	preempt = QUANTUM;	/* Set the preemption time		*/

	clktime = 0;		/* Start counting seconds		*/

	return;
}


