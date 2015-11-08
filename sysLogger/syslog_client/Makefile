#
# Simple Syslog client demo for
#   MS VC 4+
#   gcc / MingW
#

.SUFFIXES: .exe .obj .o

SOURCE = syslog.c printk.c demo.c

LFLAGS = /debug /debugtype:both /map /subsystem:console

all:
	@echo Say "nmake demo-vc.exe" or "make demo-mingw.exe"

demo-vc.exe: $(SOURCE:.c=.obj)
	link $(LFLAGS) /out:$@ $(SOURCE:.c=.obj) ws2_32.lib

demo-mingw.exe: $(SOURCE:.c=.o)
	gcc $(SOURCE:.c=.o) -s -lws2_32 -o demo-mingw.exe

clean:
	del demo-vc.exe demo-mingw.exe $(SOURCE:.c=.obj) $(SOURCE:.c=.o) demo*.log

zip:: demo-vc.exe demo-mingw.exe
	zip syslog_client.zip syslog.c syslog.h printk.c printk.h demo.c syslog.cat \
           demo-vc.exe demo-mingw.exe Makefile

.c.obj:
	cl -c -nologo -Zi -I. $<

.c.o:
	gcc -c -Wall -g -O2 -I. $<

demo-vc.exe:    syslog.obj printk.obj demo.obj
demo-mingw.exe: syslog.o printk.o demo.o
syslog.obj:     syslog.c syslog.h printk.h
printk.obj:     printk.c printk.h
demo.obj:       demo.c syslog.h
syslog.o:       syslog.c syslog.h printk.h
printk.o:       printk.c printk.h
demo.o:         demo.c syslog.h


