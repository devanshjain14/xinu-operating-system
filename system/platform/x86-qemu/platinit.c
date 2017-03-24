/* platinit.c - platinit */
#include <xinu.h>

/*------------------------------------------------------------------
 * platinit - platform specific initialization for X86_QEMU
 *------------------------------------------------------------------
 */
void	platinit()
{
	/* Initialize the PCI bus */
	pci_init();
}
