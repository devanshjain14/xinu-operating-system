/**
 * @file loopbackClose.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */
#include <xinu.h>
#include <stddef.h>
#include <device.h>
#include <loopback.h>
#include <stdlib.h>
#include <interrupt.h>

/**
 * @ingroup loopback
 *
 * Close a loopback device.
 * @param devptr loopback device table entry
 * @return OK if loopback is closed properly, otherwise SYSERR
 */
devcall loopbackClose(struct dentry *devptr)
{
    struct loopback *lbkptr;
    intmask im;

    lbkptr = &looptab[devptr->dvminor];

    im = disable();
    if (LOOP_STATE_ALLOC != lbkptr->state)
    {
        restore(im);
        return SYSERR;
    }

    /* free the semaphore */
    semdelete(lbkptr->sem);
    lbkptr->state = LOOP_STATE_FREE;

    restore(im);
    return OK;
}
