/**
 * @file kvprintf.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */
#include <xinu.h>
#include <device.h>
#include <kernel.h>
#include <stdarg.h>
#include <stdio.h>

extern	void	_doprnt(const char *, va_list ap, int (*)(int, void *), void *);

/**
 * @ingroup uartgeneric
 *
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ap
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */
syscall kvprintf(const char *format, va_list ap)
{
    intmask im;

    /* Note: it isn't strictly necessary to disable global interrupts here, but
     * it prevents kprintf()'s from stepping on each other toes if you happen to
     * call kprintf() from an interrupt handler.  */
    im = disable();
    _doprnt(format, ap, (int (*)(int, void*))kputc, &devtab[CONSOLE]);
    restore(im);
    return OK;
}
