/* platinit.c - platinit */

#include <xinu.h>

void clearLED();

/*------------------------------------------------------------------------
 * platinit - platform specific initialization for BeagleBone Black
 *------------------------------------------------------------------------
 */
void	platinit(void)
{
	struct	uart_csreg *uptr;	/* address of UART's CSRs	*/

	clearLED();

	/* Initialize the Interrupt Controller */

	initintc();

	/* Pad control for CONSOLE */

	am335x_padctl(UART0_PADRX_ADDR,
			AM335X_PADCTL_RXTX | UART0_PADRX_MODE);
	am335x_padctl(UART0_PADTX_ADDR,
			AM335X_PADCTL_TX | UART0_PADTX_MODE);

	/* Reset the UART device */

	uptr = (struct uart_csreg *)devtab[CONSOLE].dvcsr;
	uptr->sysc |= UART_SYSC_SOFTRESET;
	while((uptr->syss & UART_SYSS_RESETDONE) == 0);
}

void clearLED() {
#ifdef GPIO

  int i;

  for (i=GPIO_LED_USR0; i<=GPIO_LED_USR3; i=i<<1) {
    initGPIO(i);
    gpioLEDOff(i);
  }
#endif
}

void am335x_init(void)
{
  volatile uint32 *wspr = (volatile uint32 *) 0x44E35048;
  volatile uint32 *wwps = (volatile uint32 *) 0x44E35034;
  
  kprintf("Disable AM335x watchdog timer 1: ");
  *wspr = 0x0000AAAA;
  while (*wwps&0x00000010); // Delay while the first write completes
  *wspr = 0x00005555;
  while (*wwps&0x00000010); // Delay while the second write completes
  kprintf("Complete\n\r");

}
