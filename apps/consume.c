#include <xinu.h>
#include <prodcons_bb.h>
#include <prodcons.h>
void consumer(int count) {
  // reads the value of the global variable 'n'
  // 'count' times.
  // print consumed value e.g. consumed : 8
  for (int i = 0 ; i <= count ; i++)
  {
    // wait(prod);
    printf("consumed : %d \n" , n);
    // signal(cons);
  }
}


void consumer_bb(int count) {
  // Iterate from 0 to count and for each iteration read the next available value from the global array `arr_q`
  // print consumer process name and read value as,
  // name : consumer_1, read : 8

  for (int i = 0 ; i <= count ; i++)
  {
    wait(prod);
    int c = (readme++) % 5 ;
    int consumed= arr_q[c] ;
    char *proc_name = proctab[getpid()].prname;
    printf("name : %s : %d \n" , proc_name , consumed);
    signal(cons);
  }
}


