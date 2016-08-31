/**
 * @file loopback.h
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#ifndef _LOOPBACK_H_
#define _LOOPBACK_H_

#include <device.h>
#include <stddef.h>

#define LOOP_BUFFER 1024        /**< loopback buffer length             */
#define NLOOPBACK 1
/* LOOP device states */
#define LOOP_STATE_FREE     0
#define LOOP_STATE_ALLOC    1

/* loopbackControl() functions  */
#define LOOP_CTRL_SET_FLAG  0x01 /**< set flags                         */
#define LOOP_CTRL_CLR_FLAG  0x02 /**< clear flags                       */

/* loopback flags */
#define LOOP_NONBLOCK       0x01 /**< Reading and writing do not block  */

/**
 * Loopback device control block
 */
struct loopback
{
    int state;                  /**< LOOP_STATE_*above                  */
    int index;                  /**< index of first char in buffer      */
    int flags;                      /**< loopback control flags             */
    sid32 sem;              /**< number of characters in buffer     */
    uchar buffer[LOOP_BUFFER];   /**< input buffer                       */
};

extern struct loopback looptab[];

/* Driver functions */
devcall loopbackInit(struct dentry	*);
devcall loopbackOpen(struct dentry	*);
devcall loopbackClose(struct dentry	*);
devcall loopbackRead(struct dentry	*, void *, uint);
devcall loopbackWrite(struct dentry	*, const void *, uint);
devcall loopbackGetc(struct dentry	*);
devcall loopbackPutc(struct dentry	*, char);
devcall loopbackControl(struct dentry	*, int, long, long);

#endif                          /* _LOOPBACK_H_ */
