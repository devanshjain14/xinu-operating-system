## Report Assignment 2 (devajain)

### Functions

#### include/prodcons.h
```
/*Global variable for producer consumer*/
extern int n; /*this is just declaration*/
extern sid32 prod, cons;

/*function Prototype*/
void consumer(int count);
void producer(int count);
```

#### shell/xsh_prodcons.c
```
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
    // prod = semcreate(0);
    // cons = semcreate(1);
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
        //prod = semcreate(0);
        ///cons = semcreate(1);
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
```

#### apps/produce.c

```
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
```

#### apps/consume.c
```
#include <xinu.h>
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
```

#### Does your program output any garbage? If yes, why?

Yes, the output of the program outputs garbage value and random outputs. The output also displays merged output from the concurrent processes (producer and consumer), for example: "prodonsumed : 7". The value that the output showcases are random. The program also outputs that, not all values are being consumed. There is no uniform order in which the output is displayed. 

One sample output with command comes as, 
```
xsh $ prodcons 5
produced : 0 
produced : 1 
produced : 2 
produced : 3 
produced : 4 
producedconsumed : 5 
consumed : 5 
consumed : 5 xsh $  : 5 

consumed : 5 
consumed : 5 
consumed : 5 
```
The reason for the incorrect output is that, two processes are trying to concurrently access the same global variable without any one having complete control at any given time. Since there is no concurrency in the control access over the global variable, the values are coming out to be random. 

#### Are all the produced values getting consumed? Check your program for a small count like 20.

No. All the values produces are not getting consumed. One example with count = 20 has been shown below,

```
xsh $ prodcons 20
produced : 0 
produced : 1 
produced : 2 
proconsumed : 3 
consumed : 3 
consumexsh $ duced : 3 
produced : 4 
produced : 5d : 3 
consumed : 5 
consumed : 5 
consume 
produced : 6 
produced : 7 
produced :d : 5 
consumed : 8 
consumed : 8 
consum 8 
produced : 9 
produced : 10 
produced :ed : 8 
consumed : 11 
consumed : 11 11 
produced : 12 
produced : 13 
produced : 14 
consumed : 14 
consumed : 14 
consumed : 14 
consumed : 14 
consumed : 14 
consumed : 14 
consumed : 14 
consumed : 14 
consumed : 14 
consumed : 14 
 
produced : 15 
produced : 16 
produced : 17 
produced : 18 
produced : 19 
produced : 20
```


