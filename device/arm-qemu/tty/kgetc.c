/*
 * @file kgetc.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */
#include <xinu.h>
#include <uart.h>
/**
 * @ingroup uarthardware
 *
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @param devptr
 *      Pointer to the device table entry for a UART.
 *
 * @return
 *      The character read from the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kgetc(struct dentry *devptr)
{
  struct	ttycblk	*typtr;
  volatile struct uart_csreg *regptr;
  uint uart_im;
  uchar c;

    /* Get pointers to the UART and to its registers.  */
    typtr = &ttytab[devptr->dvminor];
    regptr = devptr->dvcsr;

    /* Save the UART's interrupt state and disable the UART's interrupts.  Note:
     * we do not need to disable global interrupts here; only UART interrupts
     * must be disabled, to prevent race conditions with the UART interrupt
     * handler.  */
    uart_im = regptr->imsc;
    regptr->imsc = 0;

    /* Wait until a character is ready to be received.  */
    while ((regptr->fr & PL011_FR_RXFE))
    {
        /* Do nothing */
    }

    /* Get the next character from the UART by reading it from the Data
     * Register.  */
    c = regptr->dr;

    /* Tally one character received.  */
    typtr->cin++;

    /* Restore UART interrupts and return the read character.  */
    regptr->imsc = uart_im;
    return c;
}
