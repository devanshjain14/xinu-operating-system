#include <xinu.h>
#include <future.h>
#include <stdio.h>
#include <future_test.h>

future_t *future_alloc(future_mode_t mode, uint size, uint nelems)
{
    future_t *f;
    f = (future_t *)getmem((size * nelems) + sizeof(future_t));
    f->mode = mode;
    f->state = FUTURE_EMPTY;
    f->size = size;
    f->data = sizeof(future_t) + (char *)f;
    f->max_elems = nelems;
    f->count = 0;
    f->head = 0;
    f->tail = 0;
    if (mode == FUTURE_SHARED || mode == FUTURE_QUEUE)
    {
        f->get_queue = newqueue();
        f->set_queue = newqueue();
    }
    return f;
}

syscall future_free(future_t *f)
{
    intmask mask;
    mask = disable();
    // if (f->get_queue != NULL)
    // {
    //     freemem(f->get_queue, sizeof(struct future_t));
    // }
    restore(mask);
    return freemem(f, sizeof(future_t) + f->size);
}

syscall future_set(future_t *f, char *in)
{
    intmask mask;
    mask = disable();
    if (f->mode == FUTURE_EXCLUSIVE && f->state == FUTURE_EMPTY)
    {
        memcpy(f->data, in, sizeof(in));
        f->state = FUTURE_READY;
    }

    if (f->mode == FUTURE_EXCLUSIVE && f->state == FUTURE_WAITING)
    {
        memcpy(f->data, in, sizeof(in));
        f->state = FUTURE_READY;
        resume(f->pid);
    }
    if (f->mode == FUTURE_SHARED)
    {
        memcpy(f->data, in, sizeof(in));
        f->state = FUTURE_READY;
        pid32 proid;
        while ((proid = dequeue(f->get_queue)) != EMPTY)
        {
            resume(proid);
        }
    }
    if (f->mode == FUTURE_QUEUE)
    {
        if (f->count == f->max_elems - 1)
        {
            enqueue(getpid(), f->set_queue);
            suspend(getpid());
        }
        char *tailelemptr = f->data + (f->tail * f->size);
        memcpy(tailelemptr, in, f->size);
        f->state = FUTURE_READY;
        f->tail = (f->tail + 1) % f->max_elems;
        f->count = f->count + 1;
        resume(dequeue(f->get_queue));
    }
    restore(mask);
}

syscall future_get(future_t *f, char *out)
{
    intmask mask;
    mask = disable();
    if (f->state == FUTURE_EMPTY && f->mode == FUTURE_EXCLUSIVE)
    {
        f->state = FUTURE_WAITING;
        f->pid = getpid();
        suspend(f->pid);
        memcpy(out, f->data, sizeof(f->data));
        return OK;
    }
    if (f->state == FUTURE_READY && f->mode == FUTURE_EXCLUSIVE)
    {
        memcpy(out, f->data, sizeof(f->data));
        f->state = FUTURE_EMPTY;
    }

    if (f->state == FUTURE_WAITING && f->mode == FUTURE_SHARED)
    {
        enqueue(getpid(), f->get_queue);
        suspend(getpid());
    }
    else if (f->state == FUTURE_EMPTY && f->mode == FUTURE_SHARED)
    {
        f->state = FUTURE_WAITING;
        enqueue(getpid(), f->get_queue);
        suspend(getpid());
        memcpy(out, f->data, sizeof(f->data));
    }

    if (f->state == FUTURE_READY && f->mode == FUTURE_SHARED)
    {
        memcpy(out, f->data, sizeof(f->data));
    }

    else if (f->mode == FUTURE_QUEUE)
    {
        if (f->state == FUTURE_EMPTY)
        {
            f->state = FUTURE_WAITING;
            enqueue(getpid(), f->get_queue);
            suspend(getpid());
            char *headelemptr = f->data + (f->head * f->size);
            memcpy(out, headelemptr, f->size);
            f->head = (f->head + 1) % f->max_elems;
            f->count = f->count - 1;
            resume(dequeue(f->set_queue));
            restore(mask);
            return OK;
        }
        else if (f->state == FUTURE_WAITING)
        {
            enqueue(getpid(), f->get_queue);
            suspend(getpid());
        }

        if (f->state == FUTURE_READY)
        {
            if (f->count == 0)
            {
                enqueue(getpid(), f->get_queue);
                suspend(getpid());
            }
            char *headelemptr = f->data + (f->head * f->size);
            memcpy(out, headelemptr, f->size);
            f->head = (f->head + 1) % f->max_elems;
            f->count = f->count - 1;
            resume(dequeue(f->set_queue));
            restore(mask);
            return OK;
        }
    }
    restore(mask);
    return OK;
}