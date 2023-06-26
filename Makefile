all: main

CC = gcc
override CFLAGS += -g -lm -ldl

EXCLUDE = modules

SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print | grep -v $(EXCLUDE))
HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)

main: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o "$@" -g -lm -ldl

main-debug: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -O0 $(SRCS) -o "$@" -g -lm -ldl

clean:
	rm -f main main-debug
