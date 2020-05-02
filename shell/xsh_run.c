#include <prodcons_bb.h>
#include <xinu.h>
#include <prodcons.h>
#include <future.h>
#include <future_test.h>
#include <stream.h>
#include <fs.h>
// definition of array, semaphores and indices
sid32 prod;
sid32 cons;
int arr_q[5];
int readme, writ;
int two = 2;
int one = 1;

//future_queue_tests
void futureq_test1(int nargs, char *args[])
{
  int three = 3, four = 4, five = 5, six = 6;
  future_t *f_queue;
  f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);

  resume(create(future_cons, 1024, 20, "fcons6", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons8", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons9", 1, f_queue));
  resume(create(future_prod, 1024, 20, "fprod3", 2, f_queue, (char *)&three));
  resume(create(future_prod, 1024, 20, "fprod4", 2, f_queue, (char *)&four));
  resume(create(future_prod, 1024, 20, "fprod5", 2, f_queue, (char *)&five));
  resume(create(future_prod, 1024, 20, "fprod6", 2, f_queue, (char *)&six));
  sleep(1);
}

void futureq_test2(int nargs, char *args[])
{
  int seven = 7, eight = 8, nine = 9, ten = 10, eleven = 11;
  future_t *f_queue;
  f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);

  resume(create(future_prod, 1024, 20, "fprod10", 2, f_queue, (char *)&seven));
  resume(create(future_prod, 1024, 20, "fprod11", 2, f_queue, (char *)&eight));
  resume(create(future_prod, 1024, 20, "fprod12", 2, f_queue, (char *)&nine));
  resume(create(future_prod, 1024, 20, "fprod13", 2, f_queue, (char *)&ten));
  resume(create(future_prod, 1024, 20, "fprod13", 2, f_queue, (char *)&eleven));

  resume(create(future_cons, 1024, 20, "fcons14", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons15", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons16", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons17", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons18", 1, f_queue));
  sleep(1);
}

void futureq_test3(int nargs, char *args[])
{
  int three = 3, four = 4, five = 5, six = 6;
  future_t *f_queue;
  f_queue = future_alloc(FUTURE_QUEUE, sizeof(int), 3);

  resume(create(future_cons, 1024, 20, "fcons6", 1, f_queue));
  resume(create(future_prod, 1024, 20, "fprod3", 2, f_queue, (char *)&three));
  resume(create(future_prod, 1024, 20, "fprod4", 2, f_queue, (char *)&four));
  resume(create(future_prod, 1024, 20, "fprod5", 2, f_queue, (char *)&five));
  resume(create(future_prod, 1024, 20, "fprod6", 2, f_queue, (char *)&six));
  resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons8", 1, f_queue));
  resume(create(future_cons, 1024, 20, "fcons9", 1, f_queue));
  sleep(1);
}

void fut_test(int nargs, char *args[])
{
  if (strncmp(args[1], "-pc", 3) == 0)
  {

    future_t *f_exclusive, *f_shared;
    f_exclusive = future_alloc(FUTURE_EXCLUSIVE, sizeof(int), 1);
    f_shared = future_alloc(FUTURE_SHARED, sizeof(int), 1);

    // Test FUTURE_EXCLUSIVE
    resume(create(future_cons, 1024, 20, "fcons1", 1, f_exclusive));
    resume(create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, (char *)&one));

    // Test FUTURE_SHARED
    resume(create(future_cons, 1024, 20, "fcons2", 1, f_shared));
    resume(create(future_cons, 1024, 20, "fcons3", 1, f_shared));
    resume(create(future_cons, 1024, 20, "fcons4", 1, f_shared));
    resume(create(future_cons, 1024, 20, "fcons5", 1, f_shared));
    resume(create(future_prod, 1024, 20, "fprod2", 2, f_shared, (char *)&two));
  }
  else if (strncmp(args[1], "-f", 3) == 0)
  {
    int fib = -1, i;

    fib = atoi(args[2]);

    if (fib > -1)
    {
      int final_fib;
      int future_flags = FUTURE_SHARED; // TODO - add appropriate future mode here

      // create the array of future pointers
      if ((fibfut = (future_t **)getmem(sizeof(future_t *) * (fib + 1))) == (future_t **)SYSERR)
      {
        printf("getmem failed\n");
        return (SYSERR);
      }

      // get futures for the future array
      for (i = 0; i <= fib; i++)
      {
        if ((fibfut[i] = future_alloc(future_flags, sizeof(int), 1)) == (future_t *)SYSERR)
        {
          printf("future_alloc failed\n");
          return (SYSERR);
        }
      }

      // spawn fib threads and get final value
      // TODO - you need to add your code here
      for (int i = 0; i <= fib; i++)
      {
        resume(create(ffib, 1024, 20, "fprods", 1, i));
      }
      future_get(fibfut[fib], (char *)&final_fib);

      for (i = 0; i <= fib; i++)
      {
        future_free(fibfut[i]);
      }

      freemem((char *)fibfut, sizeof(future_t *) * (fib + 1));
      printf("Nth Fibonacci value for N=%d is %d\n", fib, final_fib);
      return (OK);
    }
  }
}
uint fstest(int nargs, char *args[]);

void prodcons_bb(int nargs, char *args[])
{

  prod = semcreate(0);
  cons = semcreate(1);
  readme = 0;
  writ = 0;
  int producerProcesses = atoi(args[1]);
  int consumerProcesses = atoi(args[2]);
  int producerCount = atoi(args[3]);
  int consumerCount = atoi(args[4]);

  for (int i = 1; i <= producerProcesses; i++)
  {
    char c[15];
    sprintf(c, "producer_ %d", i);
    producerCount = atoi(args[3]);
    resume(create(producer_bb, 1024, 20, c, 1, producerCount));
  }
  for (int j = 1; j < consumerProcesses; j++)
  {
    char d[15];
    sprintf(d, "consumer_ %d", j);
    resume(create(consumer_bb, 1024, 20, d, 1, consumerCount));
  }
}

shellcmd xsh_run(int nargs, char *args[])
{

  if ((nargs == 1) || (strncmp(args[1], "list", 5) == 0))
  {
    printf("my_function_1\n");
    printf("my_function_2\n");
    return OK;
  }

  args++;
  nargs--;

  if (strncmp(args[0], "prodcons_bb", 13) == 0)
  {
    /* create a process with the function as an entry point. */
    resume(create((void *)prodcons_bb, 4096, 20, "producer_bb", 2, nargs, args));
  }

  else if (strncmp(args[0], "futures_test", 13) == 0)
  {

    if (strncmp(args[1], "-fq1", 4) == 0)
    {
      resume(create((void *)futureq_test1, 4096, 20, "futureq_test1", 2, nargs, args));
    }
    else if (strncmp(args[1], "-fq2", 4) == 0)
    {
      resume(create((void *)futureq_test2, 4096, 20, "futureq_test2", 2, nargs, args));
    }
    else if (strncmp(args[1], "-fq3", 4) == 0)
    {
      resume(create((void *)futureq_test3, 4096, 20, "futureq_test3", 2, nargs, args));
    }
    else
    {
      resume(create((void *)fut_test, 4096, 20, "fut_test", 2, nargs, args));
    }
  }

  else if (strncmp(args[0], "tscdf", 13) == 0)
  {
    /* create a process with the function as an entry point. */
    resume(create((void *)stream_proc, 4096, 20, "stream_proc", 2, nargs, args));
  }
  else if (strncmp(args[0], "tscdf_fq", 13) == 0)
  {
    resume(create(stream_proc_futures, 4096, 20, "stream_proc_futures", 2, nargs, args));
  }
  else if (strncmp(args[0], "fstest", 6) == 0)
  {
    resume(create((void *)fstest, 4096, 20, "fstest", 2, nargs, args));
  }
  return (0);
}