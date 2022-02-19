# JUST a reminder:
# 	$@ : goal of the rule or the objective of the rule
# 	$< : first dependency of the rule
# 	%.c: all the filename *.c

RM=rm -f 
CFLAGS=-g -Wall
CC=gcc
LIBS=-lpthread
SRC=server.c utils.c
OBJS=$(SRC:.c=.o)
BIN=bin/
MD=mkdir

all: directories server 

.PHONY: directories
directories:
	$(MD) -p $(BIN)

server: $(OBJS)
	$(CC) -Wall -o $(BIN)$@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) *.o $(BIN)server 
	$(RM) -r $(BIN)