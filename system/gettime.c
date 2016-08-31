/* gettime.c - gettime */

#include <xinu.h>

static uint date_time = 0;

uint get_datetime(void)
{
    return (date_time + clktime);
}

uint set_datetime(uint dt)
{
    date_time = dt - clktime;
    return (date_time + clktime);
}
/*------------------------------------------------------------------------
 *  gettime  -  Get xinu time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
status	gettime(
	  uint32  *timvar		/* Location to store the result	*/
	)
{
	uint32	now;			/* Current time (UCT)		*/
	int32	retval;			/* Return value from call	*/

	/* Get current time in UCT representation (GMT) */

	//retval = getutime(&now);
	//if (retval == SYSERR) {
	//	return SYSERR;
	//}

	/* Adjust to xinu time and store result */

	*timvar = get_datetime();
	return OK;
}
