#include <armv7a.h>
.globl page_fault

page_fault:
	.func page_fault

	sub lr, lr, #4
	srsdb #ARMV7A_CPSR_SYS!
	cpsid if, #ARMV7A_CPSR_SYS
	push {r0-r4, r12, lr}
	and r4, sp, #4
	sub sp, sp, r4
	bl dmb
	bl pageFaultHandler
	bl dmb
	add sp, sp, r4
	pop {r0-r4, r12, lr}
	rfeia sp!
	.endfunc

