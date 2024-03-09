CC ?= cc
CFLAGS ?= -g -Wall -O2

all:
	$(CC) $(CFLAGS) -o fileSys fileStruct.c `pkg-config fuse --cflags --libs`
	echo 'To make mount: mkdir [Mount point]'
	echo 'To Mount: ./fileSys -f [mount point]'

