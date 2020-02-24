#include <future.h>
#include <xinu.h>

extern int zero, one;
extern int two;
future_t **fibfut;
uint future_prod(future_t *fut, char *value);
uint future_cons(future_t *fut);
int ffib(int n);