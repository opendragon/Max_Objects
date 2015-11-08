/*
 *  Simple syslog handler for Win32.
 *
 *  Loosely based on BSD-version.
 *    by G. Vanem <giva@bgnett.no>  Jun-2003
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#include <iphlpapi.h>

#include "printk.h"

#define SYSLOG_BUF_SIZE 2048

static SOCKET      logSock     = INVALID_SOCKET;  /* UDP socket for log-daemon */
static FILE       *logFile     = NULL;            /* FILE* for log-file */
static const char *logFileName = NULL;            /* name of log-file */
static DWORD       logHostAddr = 0UL;             /* IP-address of log-daemon (network order) */
static WORD        logHostPort = 0;               /* destination port for syslog daemon */
static BOOL        logOpened   = FALSE;           /* have done openlog() */
static int         logStat     = 0;               /* status bits, set by openlog() */
static char       *logTag      = NULL;            /* string to tag the entry with */
static int         logFacil    = LOG_USER;        /* default facility code */
static int         logMask     = 0xFF;            /* mask of priorities to be logged */

static DWORD my_ip_addr  = INADDR_ANY;     /* IP of local interface (network order) */
static DWORD net_mask    = INADDR_NONE;    /* netmask for IP (network order) */
static char *logHostName = NULL;           /* name of syslogd host ("localhost") */
static char  err_buf [256];

static const char *sock_error (void);
static BOOL get_netmask (DWORD *mask);

/*
 * syslog, vsyslog --
 *    print message on log file or to loghost.
 */
int syslog (int pri, const char *fmt, ...)
{
  int rc;
  va_list args;
  va_start (args, fmt);
  rc = vsyslog (pri, fmt, args);
  va_end (args);
  return (rc);
}

int vsyslog (int pri, const char *fmt, va_list ap)
{
  const char *pri_end;
  char  tbuffer [SYSLOG_BUF_SIZE];
  char *p = tbuffer;
  int   left = sizeof(tbuffer);
  int   saved_errno = errno;
  int   rc = 0;

  err_buf[0] = '\0';

  /* Check priority against setlogmask() values
   * Note: higher priorities are lower values !!
   */
  if (LOG_PRI(pri) > LOG_PRI(logMask))
     return (rc);

  /* Set default facility if none specified
   */
  if (!(pri & LOG_FACMASK))
     pri |= logFacil;

  /* Build the message
   */
  p    += _snprintf (p, left, "<%3d>%s ", pri, time_now());
  left -= p - tbuffer;
  pri_end = 1 + strchr (tbuffer, '>');

  if (logTag)
  {
    p += _snprintf (p, left, logTag);
    left -= p - tbuffer;
  }

  if (logStat & LOG_PID)
  {
    p += _snprintf (p, left, "[%d]", getpid());
    left -= p - tbuffer;
  }

  if (logTag)
  {
    p += _snprintf (p, left, ": ");
    left -= 2;
  }

  errno = saved_errno;
  p += vsnprintk (p, left, fmt, ap);
  if (*(p-1) != '\n')
  {
    *p++ = '\n';
    *p = '\0';
  }

  if (!logOpened)
     rc = openlog (logTag, logStat | LOG_NDELAY, logFacil);

  if (logFile)
  {
    int rc2 = (fputs (tbuffer, logFile) != -1) ? 0 : -1;
    if (rc == 0)
       rc = rc2;
  }

  if (logSock != INVALID_SOCKET)
  {
    struct sockaddr_in addr;
    char   tx_buf[SYSLOG_BUF_SIZE];
    int    len;

    p   = strchr (tbuffer, '>') + strlen ("YYYY-MM-DD HH:MM:SS") + 2;
    len = sprintf (tx_buf, "<%3d>%s", pri, p);
    
    memset (&addr, 0, sizeof(addr));    
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = logHostAddr;
    addr.sin_port        = logHostPort;    
    if (sendto(logSock, tx_buf, len, 0, (struct sockaddr*)&addr, sizeof(addr)) < 0 &&
        WSAGetLastError() != WSAEMSGSIZE) 
    {
      const char *err = sock_error();
      char  tmp[256];

      closesocket (logSock);
      logSock = INVALID_SOCKET;
      _snprintf (tmp, sizeof(tmp), "UDP-write failed; %s\n", err);
      syslog (LOG_ERR, tmp);
      strcpy (err_buf, tmp);
      rc = -1;
    }
  }

  if ((logStat & LOG_PERROR) ||
      ((logStat & LOG_CONS) && rc == -1))
     fputs (pri_end, stdout);

  return (rc);
}

static const char *getlogname (void)
{
  char  path [MAX_PATH] = { 0 };
  char *name, *exe;

  if (!GetModuleFileName(NULL,path,sizeof(path)) || !path[0])
     goto unknown;

  name = strdup (path);
  if (!name)
     goto unknown;

  exe = strrchr (name, '.');            /* ".exe" or ".dll" */
  if (exe && exe <= strchr(name,'\0')-4)
  {
    strcpy (exe, ".log");
    return (name);
  }
unknown:
  return strdup ("$unknown.log");
}

static char *rip (char *s)
{
  char *p;

  if ((p = strrchr(s,'\n')) != NULL) *p = '\0';
  if ((p = strrchr(s,'\r')) != NULL) *p = '\0';
  return (s);
}

static const char *sock_error (void)
{
  static char buf[256];
  DWORD  err  = WSAGetLastError();
  DWORD  lang = LANG_NEUTRAL;

  if (!FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, lang,
                      buf, sizeof(buf)-1, NULL))
    sprintf (buf, "Unknown error 0x%08lX", err);
  return rip (rip(buf));
}

static BOOL sock_init (void)
{
  static  BOOL done = FALSE;
  static  BOOL rc   = FALSE;
  char    myHostName[1024];
  struct  servent *se;
  WSADATA wsaData;

  if (done)
     return (rc);

  done = 1;
  if (WSAStartup (MAKEWORD(1,1), &wsaData) != 0)
  {
    _snprintf (err_buf, sizeof(err_buf), "Winsock init failed; %s", sock_error());
    return (rc);
  }

  if (gethostname(myHostName, sizeof(myHostName)) == 0)
  {
    struct  hostent *he = gethostbyname (myHostName);    
    if (!he)
    {
      _snprintf (err_buf, sizeof(err_buf), "Failed to get local IP-address");
      return (rc);
    }
    my_ip_addr = *(DWORD*)he->h_addr;
  }

  se = getservbyname ("syslog", "udp");
  logHostPort = se ? se->s_port : htons(514);

  if (!get_netmask(&net_mask))  /* fallback to classfull mask if this fails */
  {
    if (IN_CLASSA(my_ip_addr))       
         net_mask = IN_CLASSA_NET;
    else if (IN_CLASSB(my_ip_addr))
         net_mask = IN_CLASSB_NET;
    else if (IN_CLASSC(my_ip_addr))
         net_mask = IN_CLASSC_NET;
    else net_mask = INADDR_NONE;   /* point-to-point */
  }
  rc = TRUE;
  return (rc);
}

/*
 * Return IP-address of name on network order 
 */
static DWORD resolve (const char *name)
{
  struct hostent *he;
  DWORD  ip = inet_addr (name);

  if (ip != INADDR_NONE)
     return (ip);
  
  he = gethostbyname (name);
  if (!he)
  {
    _snprintf (err_buf, sizeof(err_buf), "Unknown host `%s'", name);
    return (0);
  }
  ip = *(DWORD*)he->h_addr;
  return (ip);
}

/*
 * Check if host address 'ip' replies on ARP.
 */
static BOOL check_arp (DWORD ip)
{
  HMODULE mod = LoadLibrary ("iphlpapi.dll");
  BOOL    rc = TRUE;
  
  if (mod)
  {
    DWORD (*SendARP) (IPAddr, IPAddr, DWORD*, DWORD*);
    DWORD mac_len = 6;
    BYTE  mac_addr[6] = { 0 };

    (void*)SendARP = GetProcAddress (mod, "SendARP");
    if (SendARP)
    {
      (*SendARP) (ip, 0, (DWORD*)&mac_addr, &mac_len);
      if (!memcmp("\0\0\0\0\0\0", &mac_addr, sizeof(mac_addr)))
         rc = FALSE;
#if 0
      printf ("MAC-addr %02X:%02X:%02X:%02X:%02X:%02X\n", 
              mac_addr[0] & 255, mac_addr[1] & 255, mac_addr[2] & 255, 
              mac_addr[3] & 255, mac_addr[4] & 255, mac_addr[5] & 255);
#endif
    }
    FreeLibrary (mod);
  }
  if (!rc)
     _snprintf (err_buf, sizeof(err_buf), "Host not reachable by ARP-request");
  return (rc);
}

static BOOL get_netmask (DWORD *mask)
{
  BOOL    rc  = FALSE;
  HMODULE mod = LoadLibrary ("iphlpapi.dll");
  
  if (mod)
  {
    IP_ADAPTER_INFO info;
    DWORD (*GetAdaptersInfo) (IP_ADAPTER_INFO*, DWORD*);
    DWORD   len = sizeof (info);
  
    (void*)GetAdaptersInfo = GetProcAddress (mod, "GetAdaptersInfo");
    if (GetAdaptersInfo && (*GetAdaptersInfo) (&info, &len) == ERROR_SUCCESS)
    {
      *mask = inet_addr (info.IpAddressList.IpMask.String);
      rc = TRUE;
    }
    FreeLibrary (mod);
  }
  if (!rc)
     _snprintf (err_buf, sizeof(err_buf), "Failed to get netmask");
  return (rc);
}

/* 
 * Return true if 'ip' is on this LAN.
 */
static BOOL is_on_LAN (DWORD ip)
{  
  return (((ntohl(ip) ^ ntohl(my_ip_addr)) & ntohl(net_mask)) == 0);
}

/* 
 * Return true if 'ip' is a (directed) ip-broadcast address.
 */
static BOOL is_broadcast (DWORD ip)
{
  return ((~ntohl(ip) & ~ntohl(net_mask)) == 0);
}

/*
 * Open a UDP "connection" to log-host
 */
static int openloghost (const char *hostName)
{
  if (!sock_init())
     return (-1);

  if (logSock != INVALID_SOCKET)
  {
    closesocket (logSock);
    logSock = INVALID_SOCKET;
  }

  logHostAddr = resolve (hostName);
  if (!logHostAddr)
     return (-1);

  if (is_on_LAN(logHostAddr) && !is_broadcast(logHostAddr) && !check_arp(logHostAddr))
     return (-1);

  logSock = socket (AF_INET, SOCK_DGRAM, 0);
  if (logSock == INVALID_SOCKET)
  {
    _snprintf (err_buf, sizeof(err_buf), "socket() failed; %s\n", sock_error());
    return (-1);
  }
  if (is_broadcast(logHostAddr))
  {
    BOOL on = TRUE;
    setsockopt (logSock, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof(on));
  }
  return (0);
}

int openlog (const char *ident, int options, int logfac)
{
  int rc = -1;  /* pessimistic */

  if (ident)
     logTag = (char*)ident;

  logStat = options;

  if (logfac && (logfac & ~LOG_FACMASK) == 0)
     logFacil = logfac;

  if (!logFileName)        /* not already set */
     logFileName = getlogname();
  
  if (logMask)
     setlogmask (logMask);

  if (!(logStat & LOG_NDELAY))
     return (0);
 
  logOpened = TRUE;  /* prevent recursion below */
  
  logFile = fopen (logFileName, "at");
  if (logFile && fputs("\n",logFile) != EOF)
  {
    atexit ((void (*)(void))closelog);
    rc = 0; 
  }
  else
  {
    _snprintf (err_buf, sizeof(err_buf), "Failed to append to `%s': %s",
               strerror(errno));
    logFile = NULL;
  }

  if (!logHostName)
     logHostName = strdup ("127.0.0.1");

  if (!logHostName || !strcmp(logHostName,"0.0.0.0")) /* user didn't want logging to daemon */
     return (rc);  

  rc = openloghost (logHostName);
  if (rc >= 0)
  {
    syslog (LOG_INFO, "syslog client at %I started.", my_ip_addr);
    rc = syslog (LOG_INFO, "Logging to host %s (%I)", logHostName, logHostAddr);
    if (!logFile)
       atexit ((void (*)(void))closelog);
  }
  return (rc);
}

int closelog (void)
{
  if (logFile)
     fclose (logFile);
  if (logSock != INVALID_SOCKET)
     closesocket (logSock);
  if (logFileName)
     free ((void*)logFileName);
  if (logHostName)
     free ((void*)logHostName);

  logSock     = INVALID_SOCKET;
  logFile     = NULL;
  logFileName = NULL;
  logHostName = NULL;
  logOpened   = FALSE;
  err_buf[0]  = '\0';
  return (0);
}

int setlogmask (int pri_mask)
{
  int old = logMask;
  if (pri_mask)
     logMask = pri_mask;
  return (old);
}

char *setlogtag (char *new)
{
  char *old = logTag;
  logTag = new;
  return (old);
}

const char *syslog_logfilename (void)
{
  return (logFileName);
}

const char *syslog_strerror (void)
{
  return (err_buf);
}

const char *syslog_loghost (const char *host)
{
  if (logHostName)
     free (logHostName);
  logHostName = NULL;
  if (!host)
     return (NULL);

  logHostName = strdup (host);
  if (!strcmp(host,"0.0.0.0"))
     return (logHostName);

  if (!sock_init() || !resolve(host))
     return (NULL);
  return (logHostName);
}
