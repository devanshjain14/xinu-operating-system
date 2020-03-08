#include <xinu.h>
#include <tscdf.h>
#include <stdio.h>
#include "tscdf_input.h"
uint pcport;
void stream_consumer(int32 id, struct stream *str);
int32 stream_proc(int nargs, char *args[])
{
    ulong secs, msecs, time;
    int32 num_streams, work_queue_depth, time_window, output_time;
    secs = clktime;
    char st, ts, v, *ch, c, *a;
    msecs = clkticks;
    char usage[] = "Usage: -s num_streams -w work_queue_depth -t time_window -o output_time\n";
    struct stream **s;
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

    if (s == ((struct stream *)getmem(sizeof(struct stream) * num_streams)) == (struct stream **)SYSERR)
    {
        printf("ERROR");
        return (SYSERR);
    }
    struct stream *id;
    id = (struct stream *)getmem(sizeof(struct stream));
    id->tail = 0;
    id->head = 0;
    id->spaces = semcreate(0);
    id->items = semcreate(1);

    for (int i = 0; i < n_input; i++)
    {
        a = (char *)stream_input[i];
        st = atoi(a);
        while (*a++ != '\t')
            ;
        ts = atoi(a);
        while (*a++ != '\t')
            ;
        v = atoi(a);
        s[st] = id;
        id->head++;
        id->head = id->head % work_queue_depth;
        id->queue = (de **)getmem(sizeof(de) * work_queue_depth);
        wait(id->items);
        resume(create(stream_consumer, 1024, 20, "stream_consumer", 1, 0, s[i]));
        signal(id->spaces);
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
    while (1)
    {
        wait(str->spaces);
        pid32 proc_id = getpid();

        if (id == 0)
        {
            kprintf("stream_consumer exiting\n");
            ptsend(pcport, getpid());
        }
        signal(str->items);
    }
}
kprintf("stream_consumer id:%d (pid:%d)\n", id, proc_id);