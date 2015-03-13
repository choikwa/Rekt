# author: thinkdoge

CC=g++
CFLAGS=-Wall -Wextra -g -c -std=c++1y
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

idToNameMap.txt nameToIdMap.txt: Lexemes.txt genTokenNameMap.py
	python genTokenNameMap.py

clean:
	rm -f $(LIB) $(OBJS) .depend idToNameMap.txt nameToIdMap.txt Rekt
	touch Lexemes.txt
	
.PHONY: clean
