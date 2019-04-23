#include "tcp_lib.h"

int Socket(int domain, int type, int protocol){
    
    int ret_fd = socket(domain,type,protocol);
   if(ret_fd == -1){
        perror("socket error");
    }
    return ret_fd;
}

int Bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen){
    int ret = bind(sockfd,addr,addrlen);
    if(ret == -1){
        perror("bind error");
    }
    return ret;
}

int Listen(int sockfd, int backlog){
    int ret =listen(sockfd,backlog);
    if(ret == -1){
        perror("listen error");
    }
    return ret;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
    int ret;
    
again:if ((ret = accept(sockfd, addr,addrlen)) < 0) {
		if ((errno == ECONNABORTED) || (errno == EINTR))
			goto again;
		else
			perror("accept error");
	}
    return ret;
}
int Connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen){
    int ret = connect(sockfd,addr,addrlen);
    if(ret == -1){
        perror("connect error");
    }
    return ret;
}

void IP_print(int domain,const struct sockaddr_in *addr){
    char buf[BUFSIZ];
    printf("IP addr:%s\t",inet_ntop(domain,&addr->sin_addr.s_addr,buf,BUFSIZ));
    printf("port:%d\n",ntohs(addr->sin_port));    
    
    return;
}
ssize_t Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t n;

again:
	if ( (n = read(fd, ptr, nbytes)) == -1) {
		if (errno == EINTR)
			goto again;
		else
			return -1;
	}

	return n;
}

ssize_t Write(int fd, const void *ptr, size_t nbytes)
{
	ssize_t n;

again:
	if ((n = write(fd, ptr, nbytes)) == -1) {
		if (errno == EINTR)
			goto again;
		else
			return -1;
	}
	return n;
}

int Close(int fd)
{
    int n;
	if ((n = close(fd)) == -1)
		perror("close error");
    return n;
}

/*参三: 应该读取的字节数*/                          //socket 4096  readn(cfd, buf, 4096)   nleft = 4096-1500
ssize_t Readn(int fd, void *vptr, size_t n)
{
	size_t  nleft;              //usigned int 剩余未读取的字节数
	ssize_t nread;              //int 实际读到的字节数
	char   *ptr;

	ptr = (char *)vptr;
	nleft = n;                  //n 未读取字节数

	while (nleft > 0) {
		if ((nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;
			else
				return -1;
		} else if (nread == 0)
			break;

		nleft -= nread;   //nleft = nleft - nread 
		ptr += nread;
	}
	return n - nleft;
}

ssize_t Writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = (char *)vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

static ssize_t my_read(int fd, char *ptr)
{
	static int read_cnt;
	static char *read_ptr;
	static char read_buf[100];

	if (read_cnt <= 0) {
again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {   //"hello\n"
			if (errno == EINTR)
				goto again;
			return -1;
		} else if (read_cnt == 0)
			return 0;

		read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr++;

	return 1;
}

/*readline --- fgets*/    
//传出参数 vptr
ssize_t Readline(int fd, void *vptr, ssize_t maxlen)
{
	ssize_t n, rc;
	char    c, *ptr;
	ptr = (char *)vptr;

	for (n = 1; n < maxlen; n++) {
		if ((rc = my_read(fd, &c)) == 1) {   //ptr[] = hello\n
			*ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			*ptr = 0;
			return n-1;
		} else
			return -1;
	}
	*ptr = 0;

	return n;
}

