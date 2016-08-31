#include <xinu.h>
#include <mmu.h>	

#define CACHE_DISABLED    0x12
#define SDRAM_START       0x80000000
#define SDRAM_END         0x8fffffff
#define CACHE_WRITEBACK   0x1e

uint32 __attribute__((aligned(16384))) page_table[NPROC][NUM_PAGE_TABLE_ENTRIES];

void initializeMMU(void) {
    
    int i,j,k=0;
    for (i = 0; i < 264; i++){ // 258
	for (j = 0; j < NPROC; j++){
        	page_table[j][i] = i << 20 | (3 << 10) | CACHE_DISABLED;
		}
	}
	
	for (j = 264; j <264+(2064-264)/NPROC; j++){ 
		for (i=264,k = 2; k < NPROC;i++, k++){
	        	page_table[k][j] = i << 20 | (3 << 10) | CACHE_DISABLED;
		}
	}
    	for (i = 2064; i < NUM_PAGE_TABLE_ENTRIES; i++){
		for (j = 0; j < NPROC; j++){
        		page_table[j][i] = i << 20 | (3 << 10) | CACHE_DISABLED;
		}
	}
	setPageTable();    
	FlushTLB();
    	setAccessControl();   
	enableMMU(); 
}


void setPageTable(){
	uint32 pagetable =(uint32) page_table[currpid];
	/* Copy the page table address to cp15 */
    asm volatile("mcr p15, 0, %0, c2, c0, 0" : : "r" (pagetable) : "memory");
}

void setAccessControl(){
/* Set the access control to all-supervisor */
    asm volatile("mcr p15, 0, %0, c3, c0, 0" : : "r" (~0));
}

void FlushTLB(){
/* Set the access control to all-supervisor */
    asm volatile("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
}

void enableMMU(){
    uint32 reg;
/* Enable the MMU */
    asm("mrc p15, 0, %0, c1, c0, 0" : "=r" (reg) : : "cc");
    reg|=0x1;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r" (reg) : "cc");
}

void pageFaultHandler(){
	kprintf("Segmentation Fault\n");
}

