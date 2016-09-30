/* delay.h - DELAY */

/*
 * Delay units are in microseconds.
 */
#ifdef X86_GALILEO
#define	DELAY(n)                                	\
{                                               	\
	register int i = 0;				\
							\
	while(i < (n * 100)) i++;			\
}			

#else /* !X86_GALILEO */
#define	DELAY(n)                                	\
{                                               	\
	volatile long N = 2*n;				\
							\
	while(N > 0) {					\
		N--;					\
	}						\
}
#endif

/*
 * Delay units are in milli-seconds.
 */
#define	MDELAY(n)					\
{							\
	register long i;				\
							\
	for (i=n;i>0;i--) {				\
		DELAY(1000);				\
	}						\
}

