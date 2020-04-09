#include <xinu.h>
#ifndef _FUTURE_H_
#define _FUTURE_H_

typedef enum
{
  FUTURE_EMPTY,
  FUTURE_WAITING,
  FUTURE_READY
} future_state_t;

typedef enum
{
  FUTURE_EXCLUSIVE,
  FUTURE_SHARED,
  FUTURE_QUEUE
} future_mode_t;

typedef struct future_t
{
  char *data;
  uint size;
  future_state_t state;
  future_mode_t mode;
  pid32 pid;
  qid16 set_queue;
  qid16 get_queue;
  uint16 max_elems;
  uint16 count;
  uint16 head;
  uint16 tail;
} future_t;

/* Interface for the Futures system call */
future_t *future_alloc(future_mode_t mode, uint size, uint nelems);
syscall future_free(future_t *);
syscall future_get(future_t *, char *);
syscall future_set(future_t *, char *);

int stream_proc_futures(int nargs, char *args[]);
void stream_consumer_future(int32 id, future_t *f);

#endif /* _FUTURE_H_ */