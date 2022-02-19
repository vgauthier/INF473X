# JUST a reminder:
# 	$@ : goal of the rule or the objective of the rule
# 	$< : first dependency of the rule
# 	%.c: all the filename *.c

RM=rm -f 
CFLAGS=-g -Wall
CC=clang
LIBS=-lpthread
SRC=server.c utils.c
OBJS=$(SRC:.c=.o)

all: server

server: $(OBJS)
	$(CC) -Wall -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) *.o server