CFLAGS := -m64 -O2 -g -D_GNU_SOURCE -D_REENTRANT -W -I/usr/local/include
LDFLAGS := -m64 -lm

all: rcontainer.c
	$(CC) $(CFLAGS) -Wall -fPIC -c rcontainer.c
	$(CC) $(CFLAGS) -shared -Wl,-soname,librcontainer.so.1 -o librcontainer.so.1.0 rcontainer.o

install: librcontainer.so.1.0
	cp librcontainer.so.1.0 /usr/lib/librcontainer.so.1
	ln -fs /usr/lib/librcontainer.so.1 /usr/lib/librcontainer.so
	cp rcontainer.h  /usr/local/include


clean:
	rm -f *.so *.o *.1.0


.PHONY: all clean
