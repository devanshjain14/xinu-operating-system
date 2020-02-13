#include <xinu.h>
#include <prodcons_bb.h>
#include <prodcons.h>

void producer(int count) {
    // Iterates from 0 to count, setting
    // the value of the global variable 'n'
    // each time.
    //print produced value e.g. produced : 8
    for (int i = 0 ; i <= count ; i++)
    {
        // wait(cons);
        n = i;
        printf("produced : %d \n" , n);
        // signal(prod);
    }
}


void producer_bb(int count) {
  // Iterate from 0 to count and for each iteration add iteration value to the global array `arr_q`, 
  // print producer process name and written value as,
  // name : producer_1, write : 8
    for (int i = 0 ; i <= count ; i++)
    {   
        wait(cons);
        arr_q[(writ++)%5]= i;
        char *proc_name = proctab[getpid()].prname;
        printf("%s : %d \n" , proc_name , i);
        signal(prod);
    }
}