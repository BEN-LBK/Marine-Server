src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

all: server client

server: server.o tcp_lib.o
	gcc server.o tcp_lib.o -o server -Wall 
client: client.o tcp_lib.o
	gcc client.o tcp_lib.o -o client -Wall 

%.o:%.c
	gcc -c $< -Wall 

.PHONY: clean all
clean: 
	-rm -rf server client $(obj)
