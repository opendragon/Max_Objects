#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include <winsock.h>

#ifndef EHOSTDOWN
#define EHOSTDOWN WSAEHOSTDOWN
#endif

#define TEST(what) do {                         \
                     if (what < 0)              \
                          result (1, __LINE__); \
                     else result (0, __LINE__); \
                     Sleep (250);               \
                   } while (0)

void result (int fail, unsigned line)
{
  const char *err = syslog_strerror();

  if (fail)
       fprintf (stderr, "demo failed at line %d: %s\n", line, err);
  else fprintf (stderr, "demo result at line %d: %s\n", line, err[0] ? err : "<ok>");
}

int main (int argc, char **argv)
{
  char big_buf[2000];

  if (argc > 1 && !syslog_loghost(argv[1]))
  {
    fprintf (stderr, "Failed to log to daemon %s; %s\n", argv[1], syslog_strerror());
    return (-1);
  }

  /* Log to console if send() fails.
   */
  TEST (openlog ("demo", LOG_PID | LOG_CONS, LOG_LOCAL2));

  TEST (syslog (LOG_NOTICE, "Time now %t. file %s at line %d\n", __FILE__, __LINE__));

  errno = ENOMEM;
  TEST (syslog (LOG_ERR, "Allocation failed; %m"));
  TEST (syslog (LOG_INFO, "Loopback address = %I", htonl(INADDR_LOOPBACK)));

  errno = EHOSTDOWN;
  TEST (syslog (LOG_ERR, "Connection failed; %m"));
  
  memset (&big_buf, '1', sizeof(big_buf));
  big_buf [sizeof(big_buf)-1] = '\0';
  TEST (syslog (LOG_INFO, "Big string %s", big_buf));
  
  TEST (syslog (LOG_INFO, "Leaving main()"));

  printf ("Done. Look at %s\n", syslog_logfilename());

  TEST (closelog());
  return (0);
}
