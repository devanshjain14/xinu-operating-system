/* xsh_mmu.c - xsh_mmu*/

#include <xinu.h>
#include <string.h>
#include <stdio.h>

void function1(){
	int *num = (int*)0x11400000;
	*num = 300;
	printf("[process ID=%d]Value at address 0x%x is %d\n",currpid,num,*num);
}

shellcmd xsh_mmu(int32 args,char *argv[]) {
	int *num = (int*)0x11400000;
	//int *num1 = (int*)0x11C00000; // Accessing this will cause segmentation issue
	*num = 100;
	//*num1 = 200;
	printf("[process ID=%d]Value at address 0x%x is %d\n",currpid,num,*num);
	resume(create(function1,1024,20,"Parent",0));
	sleep(1);
	printf("[process ID=%d]Value at address 0x%x is %d\n",currpid,num,*num);
	return 0;
}
