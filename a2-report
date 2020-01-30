## Report Assignment 2 (devajain)

### Functions

#### include/prodcons.h
/*Global variable for producer consumer*/
extern int n; /*this is just declaration*/
extern sid32 prod, cons;

/*function Prototype*/
void consumer(int count);
void producer(int count);

#### shell/xsh_prodcons.c

#include <xinu.h>
#include <prodcons.h>

int n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/
sid32 prod ;
sid32 cons ;
shellcmd xsh_prodcons(int nargs, char *args[])
{
  //Argument verifications and validations
  int count = 2000;             //local varible to hold count
  if (nargs == 1 )
  {
    prod = semcreate(0);
    cons = semcreate(1);
    resume( create(producer, 1024, 20, "producer", 1, count));
    resume( create(consumer, 1024, 20, "consumer", 1, count));
    return (0);
  }
  else if (nargs==2 )
  {
    if (strncmp(args[1], "--help", 4) == 0)
    {
      printf("prodcons \n");
      printf("Syntax: prodcons [Integer] \n");
      printf("Description: It exhibits how two processes are able to exchange data using a global variable.");
      return 0;
    } 
    else
    {
      int integer = atoi(args[1]);
      if (integer)
      {
        count = integer;
        prod = semcreate(0);
        cons = semcreate(1);
        //check args[1] if present assign value to count
        //create the process producer and consumer and put them in ready queue.
        //Look at the definations of function create and resume in the system folder for reference. 
        resume( create(producer, 1024, 20, "producer", 1, count));
        resume( create(consumer, 1024, 20, "consumer", 1, count));
        return (0);
      }
      else
      {

        printf("Only integers are accepted. Use 'prodcons --help' command to learn about the command.");

      }
    }
  }

  else if (nargs>2)
  {
    printf("Too many arguments to unpack. Use 'prodcons --help' command to learn about the command.");
    return 0;
  }
    
}

#### apps/produce.c

#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
    // Iterates from 0 to count, setting
    // the value of the global variable 'n'
    // each time.
    //print produced value e.g. produced : 8
    for (int i = 0 ; i <= count ; i++)
    {
        wait(cons);
        n = i;
        printf("produced : %d \n" , n);
        signal(prod);
    }
}

#### apps/consume.c

#include <xinu.h>
#include <prodcons.h>

void consumer(int count) {
  // reads the value of the global variable 'n'
  // 'count' times.
  // print consumed value e.g. consumed : 8
  for (int i = 0 ; i <= count ; i++)
  {
    wait(prod);
    printf("consumed : %d \n" , n);
    signal(cons);
  }
}
