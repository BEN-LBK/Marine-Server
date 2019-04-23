#ifndef _TCP_LIB_H_
#define _TCP_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int Socket(int domain, int type, int protocol);

int Bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

int Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int Connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

void IP_print(int domain,const struct sockaddr_in *addr);

ssize_t Read(int fd, void *ptr, size_t nbytes);

ssize_t Write(int fd, const void *ptr, size_t nbytes);

ssize_t Readn(int fd, void *vptr, size_t n);

ssize_t Writen(int fd, const void *vptr, size_t n);

ssize_t Readline(int fd, void *vptr, ssize_t maxlen);

#endif
