/**
 * @file kprintf.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <stdarg.h>
#include <kernel.h>

syscall kvprintf(const char *format, va_list ap);

/**
 * @ingroup uartgeneric
 *
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */
syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = kvprintf(format, ap);
    va_end(ap);
    return retval;
}
