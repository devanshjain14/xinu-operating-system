/*  main.c  - main */

#include <xinu.h>

process	main(void)
{

	int cdev = CONSOLE;
	
	/* Run the Xinu shell */

	kprintf("\n...creating a shell\n");
	
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, cdev));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, cdev));
	}
	return OK;
    
}
