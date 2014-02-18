SOURCEFILES=peerchat.c client.c host.c queue.c peer.c
CFLAGS=-ggdb

all: peerchat ctags

peerchat:
	gcc $(CFLAGS) $(SOURCEFILES) -o peerchat

clean:
	-rm server client peerchat tags

ctags:
	ctags -R
