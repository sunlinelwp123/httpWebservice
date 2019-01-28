#ifndef T_NET_H_
#define T_NET_H_
/*头文件的包含*/
#include <t_stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>  
#ifndef MAX_EVENTS
/*最大epoll事件数*/
#define MAX_EVENTS 1000 
#endif
#ifndef TIMEOUT
/*超时时间 单位秒*/
#define TIMEOUT 60
#endif
#ifndef E_BUFF_SIZE
/*每次epoll事件 buff的最大大小*/
#define E_BUFF_SIZE 128
#endif

/*类型的定义*/
typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;
typedef struct ioEvent_s{  
    int fd;  
    void (*call_back)(int fd, int events, void *arg);  
    int events;  
    void *arg;  
    int status; // 1: in epoll wait list, 0 not in  
    char buff[E_BUFF_SIZE]; // recv data buffer  一次事件最大输送buff
    int len, s_offset;  
    long last_active; // last active time  
} ioEvent_s;

extern int g_epollFd;  
extern ioEvent_s g_Events[MAX_EVENTS+1] ; // g_Events[MAX_EVENTS] is used by listen fd  

/*函数的定义*/
/*设置epoll事件*/
void eventSet(ioEvent_s *, int, void (*call_back)(int, int, void*), void *);
/*增改epoll事件*/
void eventAdd(int, int, ioEvent_s *);
/*删除epoll事件*/
void eventDel(int, ioEvent_s *);

int s_bind(int, int, uint16_t, int);
int s_listen(int, int, uint16_t, int, int);
int n_accept(int);
int h_accept(int);
/*accept  添加 epoll 事件监控 下一步事件是 http_handle*/
void e_accept(int, int, void *);

void recvData(int, int, void *);
void sendData(int, int, void *);
void http_handle(int, int, void*);
void serve_file(int, const char *);
void cat(int , FILE *);
int read_line(int , char *, int);
void header(int , const char *);
void unknowmethod(int );
void not_found(int );
void cannot_execute(int);
void execute_cgi(int, const char *, const char *, const char *);
void bad_request(int);
#endif

