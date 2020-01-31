#include <xinu.h>
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
