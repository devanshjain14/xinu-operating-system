#ifndef _MMU_H
#define _MMU_H


void initializeMMU_mmu(void);
void setPageTable();
void setAccessControl();
void enableMMU();
void FlushTLB();
void pageFaultHandler();
#endif
