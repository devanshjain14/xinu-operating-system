/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

extern uint32 nsaddr;

process	main(void)
{
#ifdef ETHER0
	char x_ipaddr[] = "192.168.1.101";
	char x_router[] = "192.168.1.255";

	/* Start the network */
	netstart(x_ipaddr,x_router);

	nsaddr = 0x800a0c10;
#endif

	kprintf("\n...creating a shell\n");
	recvclr();
#ifdef ARM_QEMU
	resume(create(shell, 8192, 50, "shell", 1, SERIAL0));
#endif
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
#ifdef ARM_QEMU
		resume(create(shell, 4096, 20, "shell", 1, SERIAL0));
#endif
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
