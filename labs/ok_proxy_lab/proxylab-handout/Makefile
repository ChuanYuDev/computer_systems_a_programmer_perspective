# Makefile for Proxy Lab 
#
# You may modify this file any way you like (except for the handin
# rule). You instructor will type "make" on your specific Makefile to
# build your proxy from sources.

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread

all: proxy

# csapp.o: csapp.c csapp.h
# 	$(CC) $(CFLAGS) -c csapp.c

# proxy.o: proxy.c csapp.h
# 	$(CC) $(CFLAGS) -c proxy.c

# proxy: proxy.o csapp.o
# 	$(CC) $(CFLAGS) proxy.o csapp.o -o proxy $(LDFLAGS)

helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c

sbuf.o: sbuf.c sbuf.h
	$(CC) $(CFLAGS) -c sbuf.c

cache.o: cache.c cache.h
	$(CC) $(CFLAGS) -c cache.c

proxy.o: proxy.c helper.h sbuf.h
	$(CC) $(CFLAGS) -c proxy.c

proxy: proxy.o helper.o sbuf.o cache.o
	$(CC) $(CFLAGS) proxy.o helper.o sbuf.o cache.o -o proxy $(LDFLAGS)

# Creates a tarball in ../proxylab-handin.tar that you can then
# hand in. DO NOT MODIFY THIS!
handin:
	(make clean; cd ..; tar cvf $(USER)-proxylab-handin.tar proxylab-handout --exclude tiny --exclude nop-server.py --exclude proxy --exclude driver.sh --exclude port-for-user.pl --exclude free-port.sh --exclude ".*")

clean:
	rm -f *~ *.o proxy core *.tar *.zip *.gzip *.bzip *.gz

