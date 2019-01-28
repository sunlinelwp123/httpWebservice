#跟我一起写Makefile 陈皓
CC = gcc
C+ = g++
PREFIX ?= ./
INCLUDE_PATH ?= include
LIBRARY_PATH ?= lib
SERVERNAME = wbserver
R_CFLAGS = -std=c89
WEB_SRC = webcore.c t_net.c
LDLIBS = -lpthread
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(PREFIX)$(LIBRARY_PATH)
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$(PREFIX)$(INCLUDE_PATH)

build:$(WEB_SRC)
	$(CC)  $(WEB_SRC)  -c
install:$(WEB_SRC)
	$(CC) -o $(SERVERNAME)  *.o  $(LDLIBS)
clean:$(WEB_SRC)
	rm *.o	
