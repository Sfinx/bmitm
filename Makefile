
#DEBUG			= -g3
OPTIMIZE		= -O2
DEFINES 		= -D__USE_REENTRANT -D_REENTRANT -D_THREAD_SAFE -D_LIBC_REENTRANT -D_GNU_SOURCE
INCLUDES		=
CFLAGS			= -std=gnu++11 -pthread -Wall $(DEFINES) $(INCLUDES) $(DEBUG) $(OPTIMIZE)
LIBS			= -lucommon -lcommoncpp -lpthread
LDFLAGS			= $(STATIC) $(DEBUG) -Wl,-O6 -Wl,--start-group $(LIBS) -Wl,--end-group
TARGET			= bmitm

OBJS = main.o log.o

all:	do-it-all

ifeq (.depend,$(wildcard .depend))
include .depend
do-it-all:	$(TARGET)
else
do-it-all:	depend $(TARGET)
endif

$(TARGET):	$(OBJS)
	g++ $(OBJS) -o $(TARGET) $(LDFLAGS)
	strip -s $(TARGET)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@
.c.o:
	gcc $(CFLAGS) -c $< -o $@

clean:
	@rm -f DEADJOE .depend *.o *~ *core $(TARGET) nohup.out && echo Clean Ok.
                
dep depend:
	$(CC) $(CFLAGS) -M *.cpp > .depend

dist:	dep clean all
