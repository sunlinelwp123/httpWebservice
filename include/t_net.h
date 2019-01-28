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
#include <string.h>
#ifndef MAX_EVENTS
/*最大epoll事件数*/
#define MAX_EVENTS 1024 
#endif
#ifndef TIMEOUT
/*超时时间 单位秒*/
#define TIMEOUT 60
#endif
#ifndef E_BUFF_SIZE
/*每次epoll事件 buff size*/
#define E_BUFF_SIZE 128
#endif

/*类型的定义*/
typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;
typedef struct ioEvent_s{  
    int fd;  //文件描述符 
    void (*call_back)(int fd, int events, void *arg); //回调函数 
    int events;  //epoll事件类型
    void *arg;  //一般用于指向自身
    int status; // 1: in epoll wait list, 0 not in  
    char buff[E_BUFF_SIZE]; // recv data buffer  一次事件最大输送buff
    int len, s_offset; //输入长度和输出长度 
    long last_active; // last active time  
} ioEvent_s;
/* 引用主文件全局变量 */
extern int g_epollFd;  
extern ioEvent_s g_Events[MAX_EVENTS + 1] ; // g_Events[MAX_EVENTS] is used by listen fd  

/*函数的定义*/
/*设置epoll事件*/
void eventSet(ioEvent_s *, int, void (*call_back)(int, int, void*), void *);
/*增改epoll事件*/
void eventAdd(int, int, ioEvent_s *);
/*删除epoll事件*/
void eventDel(int, ioEvent_s *);
/*将socket 绑定到epoll事件 有client 接入则触发回调函数e_accept*/
int s_bind(int, int, uint16_t, int);
int s_listen(int, int, uint16_t, int, int);
int n_accept(int);
int h_accept(int);
/*accept  添加 epoll 事件监控 下一步事件是 http_handle*/
void e_accept(int, int, void *);
void e_clientSet(int);

/*接受和返回数据*/
void recvData(int, int, void *);
void sendData(int, int, void *);
//void thread_pool(int, int, void *);
/* 对http请求的处理 类似Filter过滤器 */
void http_handle(int,int,void *);
void *thread_handle(void *);
void serve_file(int, const char *);
void cat(int, FILE *);
int read_line(int, char *, int);
void header(int, const char *);
void unknowmethod(int);
void not_found(int);
void cannot_execute(int);
/*使用cgi通用网关接口调用服务类似interceptor 拦截器*/
void execute_cgi(int, const char *, const char *, const char *,ioEvent_s *);
void bad_request(int);
#endif

