#include <xinu.h>
#include <stream.h>
#include <stdio.h>
#include "tscdf.h"

uint pcport;
void stream_consumer(int32 id, struct stream *str);
int len, interval, otime;
int32 stream_proc(int nargs, char *args[])
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
    struct stream **s;
    len = work_queue_depth;
    interval = time_window;
    otime = output_time;

    if ((s = (struct stream **)getmem(sizeof(struct stream *) * (num_streams))) == (struct stream **)SYSERR)
    {
        printf("ERROR: GETMEM FAILED\n");
    }

    //initializing stream
    for (int i = 0; i < num_streams; i++)
    {
        if ((s[i] = (struct stream *)getmem(sizeof(struct stream) + (sizeof(de) * work_queue_depth))) == (struct stream *)SYSERR)
        {
            printf("ALLOCATION ERROR");
            return;
        }
        s[i]->items = semcreate(work_queue_depth);
        s[i]->mutex = semcreate(1);
        s[i]->spaces = semcreate(0);
        s[i]->head = 0;
        s[i]->tail = 0;
        s[i]->queue = sizeof(struct stream) + (char *)s[i];
    }
    //consumer process spawned
    for (int j = 0; j < num_streams; j++)
    {
        resume(create((void *)stream_consumer, 4096, 20, "stream_consumer", 2, j, s[j]));
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
        wait(s[st]->items);
        wait(s[st]->mutex);
        head = s[st]->head;
        s[st]->queue[head].time = ts;
        s[st]->queue[head].value = v;
        head = ++head % work_queue_depth;
        s[st]->head = head;
        signal(s[st]->mutex);
        signal(s[st]->spaces);
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

void stream_consumer(int32 id, struct stream *str)
{
    int tail, res = 0;
    struct tscdf *tc;
    pid32 proc_id = getpid();
    kprintf("stream_consumer id:%d (pid:%d)\n", id, proc_id);
    tc = tscdf_init(interval);
    while (1)
    {
        wait(str->spaces);
        wait(str->mutex);
        tail = str->tail;
        if (str->queue[str->tail].time == 0 && str->queue[str->tail].value == 0)
        {
            kprintf("stream_consumer exiting\n");
            break;
        }
        tscdf_update(tc, str->queue[tail].time, str->queue[tail].value);

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
        tail = (++tail) % len;

        str->tail = tail;
        signal(str->mutex);
        signal(str->items);
    }
    tscdf_free(tc);
    ptsend(pcport, getpid());
}