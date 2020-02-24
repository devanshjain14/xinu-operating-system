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
} future_t;

/* Interface for the Futures system calls */
future_t *future_alloc(future_mode_t mode, uint size, uint nelems);
syscall future_free(future_t *);
syscall future_get(future_t *, char *);
syscall future_set(future_t *, char *);

#endif /* _FUTURE_H_ */