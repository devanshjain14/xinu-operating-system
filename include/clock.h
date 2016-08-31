/* clock.h */

extern	uint32	clktime;	/* current time in secs since boot	*/

extern	qid16	sleepq;		/* queue for sleeping processes		*/
extern	int32	slnonempty;	/* nonzero if sleepq is nonempty	*/
extern	int32	*sltop;		/* ptr to key in first item on sleepq	*/
extern	uint32	preempt;	/* preemption counter			*/
extern volatile ulong clkticks;


void clkinit(void);

void clkupdate(ulong cycles);

ulong clkcount(void);

interrupt clkhandler(void);
void udelay(ulong);
void mdelay(ulong);


#define CLKTICKS_PER_SEC  1000
