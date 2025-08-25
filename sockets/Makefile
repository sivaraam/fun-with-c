.PHONY: clean
CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -pedantic

BINARIES = server client

all: server client

#.PHONY: DEFAULT_TARGET
#DEFAULT_TARGETS ?= server client
#DEFAULT_TARGET: $(DEFAULT_TARGETS)

clean:
	rm -f $(BINARIES)
	rm -rf *.o a.out*

server: server.c
	$(CC) $(CFLAGS) $^ -o $@

client: client.c
	$(CC) $(CFLAGS) $^ -o $@
