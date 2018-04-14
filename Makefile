#Makefile for WordConscious word game.

#libsodium needs to be compiled for whatever
#platform this file is executed on!

#compiler needs to be in the PATH!
COMPILER=gcc

BIN_DIR=bin
EXECUTABLE_NAME=WordConscious

LIBS=-L./lib/libsodium/
LINK=-lsodium
INCLUDE=-I./lib/libsodium/include -I./include
#needs trailing / for compiler statement in main:
SRCDIR=./src/
default: main

main: 
	$(COMPILER) -Wall $(SRCDIR)*.c $(LIBS) $(INCLUDE) $(LINK) -O2 -o ./$(BIN_DIR)/$(EXECUTABLE_NAME)

clean:
	rm -f ./bin/*