#include <xinu.h>
#include <future.h>
#include <future_test.h>

int ffib(int n)
{

  int minus1 = 0;
  int minus2 = 0;
  int this = 0;
  int one = 1;
  int zero = 0;

  if (n == 0)
  {
    future_set(fibfut[0], (char *)&zero);
    return OK;
  }

  if (n == 1)
  {
    future_set(fibfut[1], (char *)&one);
    return OK;
  }

  int status = (int)future_get(fibfut[n - 2], (char *)&minus2);

  if (status < 1)
  {
    printf("future_get failed\n");
    return -1;
  }

  status = (int)future_get(fibfut[n - 1], (char *)&minus1);

  if (status < 1)
  {
    printf("future_get failed\n");
    return -1;
  }

  this = minus1 + minus2;

  future_set(fibfut[n], (char *)&this);

  return (0);
}