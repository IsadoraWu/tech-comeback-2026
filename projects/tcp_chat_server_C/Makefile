CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
CPPFLAGS = -Iheader
LDLIBS = -pthread

COMMON_SOURCES = src/client.c src/protocol.c
HEADERS = header/server.h header/client.h header/protocol.h

.PHONY: all server client

all: server client

server: bin/server

client: bin/client

bin/server: src/main.c src/server.c $(COMMON_SOURCES) $(HEADERS) | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) src/main.c src/server.c $(COMMON_SOURCES) $(LDLIBS) -o $@

bin/client: test/main.c $(COMMON_SOURCES) $(HEADERS) | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) test/main.c $(COMMON_SOURCES) $(LDLIBS) -o $@

bin:
	mkdir -p $@
