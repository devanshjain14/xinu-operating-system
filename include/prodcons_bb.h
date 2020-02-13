#include <xinu.h>
// declare globally shared array
extern sid32 prod, cons;
// declare globally shared semaphores
extern int32 arr_q[5];
// declare globally shared read and write indices
extern int readme, writ;
// function prototypes
void consumer_bb(int count);
void producer_bb(int count);