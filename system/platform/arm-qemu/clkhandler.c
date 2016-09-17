/* clkhandler.c - clkhandler */

#include <xinu.h>
#include <platform.h>
void wakeup(void);
/**
 * @ingroup timer
 *
 * Interrupt handler function for the timer interrupt.  This schedules a new
 * timer interrupt to occur at some point in the future, then updates ::clktime
 * and ::clkticks, then wakes sleeping threads if there are any, otherwise
 * reschedules the processor.
 */
interrupt clkhandler(void)
{
    clkupdate(platform.clkfreq / CLKTICKS_PER_SEC);

    /* Another clock tick passes. */
    clkticks++;
	 
    /* Update global second counter. */
    if (CLKTICKS_PER_SEC == clkticks)
    {
        clktime++;
        clkticks = 0;
    }

    /* If sleepq is not empty, decrement first key.   */
    /* If key reaches zero, call wakeup.              */
    if (nonempty(sleepq) && (--firstkey(sleepq) <= 0))
    {
        wakeup();
    }
    else
    {
        resched();
    }
}

