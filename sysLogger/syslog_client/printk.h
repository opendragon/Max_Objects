#ifndef __PRINTK_H
#define __PRINTK_H

#include <stdio.h>
#include <stdarg.h>
#include <process.h>

#if !defined(_MSC_VER)
#include <unistd.h>
#endif

#define NAMESPACE(x)  syslog_ ## x

#define printk_init   NAMESPACE (printk_init)
#define printk        NAMESPACE (printk)
#define snprintk      NAMESPACE (snprintk)
#define vsnprintk     NAMESPACE (vsnprintk)
#define time_now      NAMESPACE (time_now)

extern int printk_init (int size, FILE *file);
extern int vsnprintk   (char *buf, int len, const char *fmt, va_list ap);

extern const char *time_now (void);

extern void printk (const char *fmt, ...)
#if defined(__GNUC__)
  __attribute__((format(printf,1,2)))
#endif
;

extern int snprintk (char *buf, int len, const char *fmt, ...)
#if defined(__GNUC__)
  __attribute__((format(printf,3,4)))
#endif
;

#endif  /* __PRINTK_H */

