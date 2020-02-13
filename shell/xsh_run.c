#include <prodcons_bb.h>   
#include <xinu.h>
#include <prodcons.h>             

// definition of array, semaphores and indices 
sid32 prod ;
sid32 cons ;
int arr_q[5];
int readme, writ;


void prodcons_bb(int nargs, char *args[]) {
  
  prod = semcreate(0);
  cons = semcreate(1);
  readme = 0;
  writ = 0;
  int producerProcesses = atoi(args[1]);
  int consumerProcesses = atoi(args[2]);
  int producerCount = atoi(args[3]);
  int consumerCount = atoi(args[4]);

  
  for (int i = 1 ; i<=producerProcesses ; i++)
  {
    char c[15] ;
    sprintf(c,"producer_ %d", i);
  producerCount = atoi(args[3]);
    resume( create(producer_bb, 1024, 20, c, 1, producerCount));
  }
  for (int j = 1 ; j< consumerProcesses ; j++)
  {
    char d[15] ;
    sprintf(d,"consumer_ %d", j);
    resume( create(consumer_bb, 1024, 20, d, 1, consumerCount));

  }
}
shellcmd xsh_run(int nargs, char *args[]){

  if ((nargs == 1) || (strncmp(args[1], "list", 5) == 0)) {
      printf("my_function_1\n");
      printf("my_function_2\n");
      return OK;
    }

    /* This will go past "run" and pass the function/process name and its
    * arguments.
    */
  args++;
  nargs--;

  if(strncmp(args[0], "prodcons_bb", 13) == 0) {
    /* create a process with the function as an entry point. */
    resume (create((void *)prodcons_bb, 4096, 20, "producer_bb", 2, nargs, args));
  }


  return (0);
}