#ifndef T_NET_H_
#define T_NET_H_
/*头文件的包含*/
#include "t_stdio.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

/*类型的定义*/
typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;

/*函数的定义*/
int s_bind(int domain,int type,uint16_t port);
int s_listen(int domain,int type,uint16_t port,int backlog);
int n_accept(int fd);
int h_accept(int fd);
#endif

