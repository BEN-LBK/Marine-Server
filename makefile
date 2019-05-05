src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

all: server client

server: server.o tcp_lib.o
	gcc server.o tcp_lib.o -o server -g
client: client.o tcp_lib.o
	gcc client.o tcp_lib.o -o client -g

%.o:%.c
	gcc -c $< -Wall -g

.PHONY: clean all
clean: 
	-rm -rf server client $(obj)
