#include <xinu.h>
#include <tscdf.h>
#include <tscdf_input.h>
uint pcport;

int32 stream_proc(int nargs, char *args[])
{
    ulong secs, msecs, time;
    int32 num_streams, work_queue_depth, time_window, output_time, stream_input;
    secs = clktime;
    char st, ts, v, a, ch, c;
    msecs = clkticks;
    char a[100];
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

    // Your code for Phase 1 goes here
    for (int i = 0; i < stream_input.length(); i++)
    {
        a = (char *)stream_input[i];
        st = atoi(a);
        while (*a++ != '\t')
            ts = atoi(a);
        while (*a++ != '\t')
            v = atoi(a);
    }

    for (int i = 0; i < num_streams; i++)
    {
        uint32 pm;
        pm = ptrecv(pcport);
        printf("process %d exited\n", pm);
    }

    ptdelete(pcport, 0);

    time = (((clktime * 1000) + clkticks) - ((secs * 1000) + msecs));
    printf("time in ms: %u\n", time);

    return (0);
}
