CFLAGS = -g -Wall
CC = gcc

all: ptrace_debugger

controller:
	${CC} ${CFLAGS} ptrace_debugger.h ptrace_debugger.c -o ptrace_debugger

clean:
	rm -f ptrace_debugger *.o log