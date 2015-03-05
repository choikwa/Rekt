# author: thinkdoge

CC=g++
CFLAGS=-c -Wall -Wextra -O3 -fpic -std=c++1y
LDFLAGS=
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: Rekt

Rekt: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@
	
depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend
	
include .depend

clean:
	rm -f $(LIB) $(OBJS)
	
.PHONY: clean