/**
 * @file     loopbackInit.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */
#include <xinu.h>
#include <stddef.h>
#include <device.h>
#include <loopback.h>
#include <stdlib.h>

struct loopback looptab[NLOOPBACK];

/**
 * @ingroup loopback
 *
 * Initialize loopback device
 * @param devptr 
 * @return OK on open
 */
devcall loopbackInit(struct dentry *devptr)
{
    struct loopback *lbkptr = NULL;

    lbkptr = &looptab[devptr->dvminor];
    lbkptr->state = LOOP_STATE_FREE;
    lbkptr->index = 0;

    return OK;
}
