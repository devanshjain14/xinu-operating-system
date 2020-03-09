#include "tscdf.h"


struct tscdf *
tscdf_init(int maxvals) {
  struct tscdf *new_tscdf;

  new_tscdf = (struct tscdf *)getmem(sizeof(struct tscdf));

  if (new_tscdf == (struct tscdf *)SYSERR) {
    printf("tscdf: getmem failed\n");
    return(NULL);
  }

  new_tscdf->data = (struct tscdf_element *)getmem(maxvals 
                     * sizeof(struct tscdf_element));

  if (new_tscdf->data == (struct tscdf_element *)SYSERR) {
    printf("tscdf: getmem failed\n");
    return(NULL);
  }

  new_tscdf->vhead = new_tscdf->vtail = NULL;
  
  new_tscdf->max_vals = maxvals;
  new_tscdf->num_vals = 0;
  new_tscdf->newest = -1;
  if ((new_tscdf->mutex = semcreate(1)) == SYSERR) {
      printf("tscdf: semcreate failed\n");
      return(NULL);
  }

  return (new_tscdf);
  
}

int32
tscdf_free(struct tscdf *tc) {

  semdelete(tc->mutex);

  freemem((char *)tc->data, tc->max_vals * sizeof(struct tscdf_element));

  freemem((char *)tc, sizeof(struct tscdf));

  return(OK);
}


/* insert a new value and remove the oldest one */
int
tscdf_update(struct tscdf *tc, int timestamp, int value) {
  struct tscdf_element *new_te, *old_te, *tte, *ttep;

  if (tc == NULL) { return SYSERR; }

  tc->newest = (tc->newest + 1) % tc->max_vals;

  old_te = &tc->data[tc->newest];

  wait(tc->mutex);

  /* full queue -- the steady state */
  if (tc->num_vals == tc->max_vals) {

    /* remove from value list */
    if (old_te->vnext == NULL) {
      /* then it was vhead */
      old_te->vprev->vnext = NULL;
      tc->vhead = old_te->vprev;
    }
    else {
      old_te->vnext->vprev = old_te->vprev;
    }
    if (old_te->vprev == NULL) {
      /* then it was vtail */
      old_te->vnext->vprev = NULL;
      tc->vtail = old_te->vnext;
    }
    else {
      old_te->vprev->vnext = old_te->vnext;
    }
    
    tc->num_vals--;
  }

  /* now copy values into tc->data[tc->newest] */
  new_te = old_te;

  new_te->vprev = new_te->vnext = NULL;
  new_te->timestamp = timestamp;
  new_te->value = value;

  /* first element in the list */
  if (tc->vhead == NULL) {
    tc->vhead = tc->vtail = new_te;
    tc->num_vals++;
    signal(tc->mutex);
    return(OK);
  }

  /* insert new element in sorted value list */

  /* new lowest value.  insert at ttail and we are finished. */
  if(value <= tc->vtail->value) {
    new_te->vnext = tc->vtail;
    tc->vtail->vprev = new_te;
    tc->vtail = new_te;
    tc->num_vals++;
    signal(tc->mutex);
    return(OK);
  }
  
  tte = tc->vtail;
  ttep = NULL;

  while(tte != NULL && tte->value < value) {
    ttep = tte;
    tte = tte->vnext;
  }
  /* tte now points just after where we want new_te */

  new_te->vnext = tte;
  new_te->vprev = ttep;
  ttep->vnext = new_te;
  if (tte != NULL) {
    tte->vprev = new_te;
  }
  else {
    tc->vhead = new_te;
  }
  
  tc->num_vals++;
  signal(tc->mutex);

  return(OK);
}

/* this should return a null-terminated array of ints */
int *
tscdf_walk(struct tscdf *tc) {
  struct tscdf_element *te;
  
  te = tc->vtail;
  kprintf("(%d) values: ", currpid);
  while(te != NULL) {
    kprintf("%d ", te->value);
    te = te->vnext;
  }
  kprintf("\n");

  /* te = tc->ttail; */
  /* kprintf("(%d) timestamps: ",currpid); */
  /* while(te != NULL) { */
  /*   kprintf("%d ", te->timestamp); */
  /*   te = te->tnext; */
  /* } */
  /* kprintf("\n"); */

  return(0);
}

/* return a null-terminated array (though just 5 elements) */
int32 *
tscdf_quartiles(struct tscdf *tc) {
  int32 q1, med, q3;
  int32 *qout;
  int32 i;
  struct tscdf_element *te;

  if(tc->num_vals < tc->max_vals) {
    printf("We don't report when the window isn't full\n");
    return(NULL);
  }

  qout = (int32 *)getmem(6 * sizeof(int32));
  if (qout == (int32 *)SYSERR) {
    printf("getmem failed\n");return(NULL);
  }

  qout[5] = NULL;

  /* if we are referring to ordinal numbers */
  /*
  q1 = (tc->max_vals / 4) + 1;j
  med = (tc->max_vals / 2) + 1;
  q3 = q1 + med;
  */

  /* if we are referring to 0-based indexes */
  if(tc->max_vals == 5) {
    q1 = 1; med = 2; q3 = 3;
  }
  q1 = (tc->max_vals / 4);
  med = (tc->max_vals / 2);
  q3 = q1 + med;

  //printf("nv: %d: %d, %d, %d\n", tc->max_vals, q1, med, q3);
  te = tc->vtail;
  
  for(i=0; i < tc->max_vals; i++) {
    
    if(i==0) {
      qout[0] = te->value;
    }
  
    if(i==q1) {
      qout[1] = te->value;
    }
    
    if(i==med) {
      qout[2] = te->value;
    }
    
    if(i==q3) {
      qout[3] = te->value;
    }
    
    if(i==tc->max_vals - 1) {
      qout[4] = te->value;
    }
 
    te = te->vnext;
  }

#if 0
  for(i=0; i < 5; i++) {
    printf("%d ", qout[i]);
  }
  printf("\n");
#endif

  return(qout);
}

int32
tscdf_test(int nargs, char *args[]) {
  
  struct tscdf *tc;
  int i;
  int32 *qarray;
  
  tc = tscdf_init(5);
  if (tc == NULL) {printf("tscdf_init failed\n"); return (-1);};

  for(i=0;i < 5;i++) {
    tscdf_update(tc, i, i);
  }
  qarray = tscdf_quartiles(tc);

  if(qarray == NULL) {
    printf("tscdf_quartiles returned NULL\n");
    return(SYSERR);
  }

  for(i=0; i < 5; i++) {
    printf("%d ", qarray[i]);
  }
  printf("\n");

  freemem((char *)qarray, (6*sizeof(int32)));

  return(OK);

  /*
  tscdf_update(tc, 10, 10);
  tscdf_update(tc, 11, 11);
  tscdf_update(tc, 12, 12);
  tscdf_update(tc, 13, 13);
  tscdf_update(tc, 14, 14);
  tscdf_update(tc, 15, 15);
  tscdf_update(tc, 16, 14);
    tscdf_update(tc, 17, 13);
  */
    //    tscdf_update(tc, 18, 12);
  /*
  tscdf_update(tc, 10, 20);
  tscdf_update(tc, 11, 21);
  tscdf_update(tc, 12, 19);
  tscdf_update(tc, 13, 22);
  tscdf_update(tc, 14, 18);
  tscdf_update(tc, 15, 25);
  tscdf_update(tc, 16, 10);
  */
  tscdf_walk(tc);
  
  return(0);

}
