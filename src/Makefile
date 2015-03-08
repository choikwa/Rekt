# author: thinkdoge

CC=clang++
CFLAGS=-Wall -Wextra -g -c -std=c++0x
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

LexemeNameMap.hpp: Lexemes.txt genTokenNameMap.py
	python genTokenNameMap.py

clean:
	rm -f $(LIB) $(OBJS) Rekt
	touch Lexemes.txt
	
.PHONY: clean
