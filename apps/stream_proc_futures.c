#include <xinu.h>
#include <stream.h>
#include <stdio.h>
#include "tscdf.h"
#include <future.h>

uint pcport;
future_t **farr;
int depth, interval, otime;
void stream_consumer_future(int32 id, future_t *f);
int32 stream_proc_futures(int nargs, char *args[])
{
    ulong secs, msecs, time;
    int32 num_streams, work_queue_depth, time_window, output_time;
    secs = clktime;
    int st, ts, v, head;
    char *ch, c, *a;
    msecs = clkticks;
    char usage[] = "Usage: -s num_streams -w work_queue_depth -t time_window -o output_time\n";

    /* Parse arguments out of flags */
    /* if not even # args, print error and exit */
    if (!(nargs % 2))
    {
        printf("%s", usage);
        return (-1);
    }
    else
    {
        int i = nargs - 1;
        while (i > 0)
        {
            ch = args[i - 1];
            c = *(++ch);

            switch (c)
            {
            case 's':
                num_streams = atoi(args[i]);
                break;

            case 'w':
                work_queue_depth = atoi(args[i]);
                break;

            case 't':
                time_window = atoi(args[i]);
                break;

            case 'o':
                output_time = atoi(args[i]);
                break;

            default:
                printf("%s", usage);
                return (-1);
            }

            i -= 2;
        }
    }

    if ((pcport = ptcreate(num_streams)) == SYSERR)
    {
        printf("ptcreate failed\n");
        return (-1);
    }
    //streams creation
    // struct stream **s;
    depth = work_queue_depth;
    interval = time_window;
    otime = output_time;

    if ((farr = (future_t **)getmem(sizeof(future_t *) * (num_streams))) == (future_t **)SYSERR)
    {
        printf("ERROR: GETMEM FAILED\n");
    }

    //initializing stream
    for (int i = 0; i < num_streams; i++)
    {
        if ((farr[i] = future_alloc(FUTURE_QUEUE, sizeof(de), work_queue_depth)) == (future_t *)SYSERR)
        {
            printf("ALLOCATION ERROR");
            return;
        }
    }
    //consumer process spawned
    for (int j = 0; j < num_streams; j++)
    {
        resume(create((void *)stream_consumer_future, 4096, 20, "stream_consumer_future", 2, j, farr[j]));
    }
    for (int i = 0; i < n_input; i++)
    {
        //input parsing
        a = (char *)stream_input[i];
        st = atoi(a);
        while (*a++ != '\t')
            ;
        ts = atoi(a);
        while (*a++ != '\t')
            ;
        v = atoi(a);
        de *tempdata = (de *)getmem(sizeof(de));
        tempdata->time = ts;
        tempdata->value = v;
        future_set(farr[st], tempdata);
    }

    for (int i = 0; i < num_streams; i++)
    {
        uint32 pm;
        pm = ptrecv(pcport);
        printf("process %d exited\n", pm);
    }

    ptdelete(pcport, 0);

    time = (((clktime * 1000) + clkticks) - ((secs * 1000) + msecs));
    kprintf("time in ms: %u\n", time);

    return (0);
}

void stream_consumer_future(int32 id, future_t *f)
{
    int tail, res = 0;
    struct tscdf *tc;
    pid32 proc_id = getpid();
    kprintf("stream_consumer_future id:%d (pid:%d)\n", id, proc_id);
    tc = tscdf_init(interval);
    while (1)
    {
        de *tempData;
        future_get(f, tempData);

        if (tempData->time == 0 && tempData->value == 0)
        {
            break;
        }

        tscdf_update(tc, tempData->time, tempData->value);

        if (res++ == (otime - 1))
        {
            char output[10];
            int *qarray;
            qarray = tscdf_quartiles(tc);
            if (qarray == NULL)
            {
                kprintf("tscdf_quartiles returned NULL\n");
                continue;
            }
            sprintf(output, "s%d: %d %d %d %d %d \n", id, qarray[0], qarray[1], qarray[2], qarray[3], qarray[4]);
            kprintf("%s", output);
            freemem((char *)qarray, (6 * sizeof(int32)));
            res = 0;
        }
    }
    tscdf_free(tc);
    ptsend(pcport, getpid());
}