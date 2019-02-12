#include <t_net.h>

/* 设置（自用）事件结构体*/  
void eventSet(ioEvent_s *ev, int fd, void (*call_back)(int, int, void*), void *arg){
	/*文件描述符 socket描述符*/	
	ev->fd = fd;  
	/* 回调函数指针*/
	ev->call_back = call_back;  
	/* 具体事件 */
	ev->events = 0;  
	/*回调函数参数*/
	ev->arg = arg;
	/* 是否在epoll （epoll_wait）等待list中 0不在list中  1在等待list中*/
	ev->status = 0;
	/* 写入 读取buff */
	bzero(ev->buff, sizeof(ev->buff));

	ev->s_offset = 0;  
	ev->len = 0;
	/*最后活跃时间*/
	ev->last_active = time(NULL);  
}  
/* 添加和修改epoll事件*/  
void eventAdd(int epollFd, int events, ioEvent_s *ev){  
	struct epoll_event epv = {0, {0}};  
	int op;  
	epv.data.ptr = ev;  
	epv.events = ev->events = events;  
	if(ev->status == 1){  
		op = EPOLL_CTL_MOD;  
	}  
	else{  
		op = EPOLL_CTL_ADD;  
		ev->status = 1;  
	}  
	if(epoll_ctl(epollFd, op, ev->fd, &epv) < 0)  
		printf("Event Add failed[fd=%d], evnets[%d]\n", ev->fd, events);  
	else  
		printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, op, events);  
}  
/* 删除事件  */
void eventDel(int epollFd, ioEvent_s *ev){  
	struct epoll_event epv = {0, {0}};  
	if(ev->status != 1) return;  
	epv.data.ptr = ev;  
	ev->status = 0;
	epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv);  
}  

int s_bind(int domain, int type, uint16_t port){
	SA4 serv;
	/*创建socket,返回文件描述符sfd*/
	int sfd=socket(domain,type,0);
	if(sfd==-1)E_MSG("socket",-1);
	/**
	 * fcntl修改文件描述符性质
	 * @param fd 要修改的文件描述符
	 * @param cmd 操作类型
	 *F_DUPFD：复制文件描述符
	 *F_DUPFD_CLOEXEC：复制文件描述符，新文件描述符被设置了close-on-exec
	 *F_GETFD：读取文件描述标识
	 *F_SETFD：设置文件描述标识
	 *F_GETFL：读取文件状态标识
	 *F_SETFL：设置文件状态标识
	 *F_GETLK：如果已经被加锁，返回该锁的数据结构。如果没有被加锁，将l_type设置为F_UNLCK
	 *F_SETLK：给文件加上进程锁
	 *F_SETLKW：给文件加上进程锁，如果此文件之前已经被加了锁，则一直等待锁被释放。
	 * @param arg 操作参数
	 */
	fcntl(sfd, F_SETFL, O_NONBLOCK);/* set non-blocking */
	/*fcntl(sfd, F_SETFL, ~O_NONBLOCK);*/ /*set blocking */
	/* 设置事件到事件数组中 */
	eventSet(&g_Events[MAX_EVENTS], sfd, &e_accept, &g_Events[MAX_EVENTS]);  
	/* add listen socket  */
	eventAdd(g_epollFd, EPOLLIN, &g_Events[MAX_EVENTS]);  

	/*初始化本地ip地址和端口号*/
	serv.sin_family=domain;
	serv.sin_port=htons(port);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	//将sfd绑定到具体的地址空间
	int b=bind(sfd,(SA *)&serv,sizeof(serv));
	if(b==-1)E_MSG("bind",-1);

	return sfd;
}
int s_listen(int domain,int type,uint16_t port,int backlog){
	int fd=s_bind(domain,type,port);
	if(fd==-1)return -1;
	listen(fd,backlog);
	return fd;
}

//无来电显
int n_accept(int fd){
	int cfd=accept(fd,NULL,NULL);
	if(cfd==-1)E_MSG("accpt",-1);
	return cfd;
}

//有来电显示
int h_accept(int fd){
	SA4 cli;
	socklen_t len;
	char IP[32];
	len=sizeof(SA4);
	int cfd=accept(fd,(SA *)&cli,&len);
	if(cfd==-1)E_MSG("accept",-1);
	printf("%s\n",inet_ntop(AF_INET,&cli.sin_addr,IP,32));
	return cfd;
}
/*accept  添加 epoll 事件监控 下一步事件是 thread_handle*/
void e_accept(int fd, int events, void *arg){  
	SA4 sin;  
	socklen_t len = sizeof(SA4);  
	int nfd;  
	if((nfd = accept(fd, ( SA*)&sin, &len)) == -1){  
		if(errno != EAGAIN && errno != EINTR){  
		}
		printf("%s: accept, %d", __func__, errno);  
		return;  
	}
	e_clientSet(nfd);
}

void e_clientSet(int nfd){	
	int i;
	do{  
		for(i = 0; i < MAX_EVENTS; i++){  
			if(g_Events[i].status == 0){  
				break;  
			}  
		}  
		if(i == MAX_EVENTS){  
			printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);  
			break;  
		}  
		// set nonblocking
		int iret = 0;
		if((iret = fcntl(nfd, F_SETFL, O_NONBLOCK)) < 0){
			printf("%s: fcntl nonblocking failed:%d", __func__, iret);
			break;
		}
		// add a read event for receive data 将请求文件描述符放入epoll监控中 
		eventSet(&g_Events[i], nfd, thread_handle, &g_Events[i]);  
		eventAdd(g_epollFd, EPOLLIN, &g_Events[i]);  
	}while(0);  
	printf("new conn[time:%d], pos[%d]\n", g_Events[i].last_active, i);  
}
void thread_handle(int fd, int events, void *arg){
	ioEvent_s *e = (ioEvent_s *)arg;
	//e->call_back(e->fd, e->events, e->arg);
	//int nfd = e->fd;
	pthread_t newthread;
        int p = pthread_create(&newthread, NULL, http_handle, &fd);//创建新线程
	if(p != 0)perror("thread create error");
	eventDel(g_epollFd, e);
}

// receive data  
void recvData(int fd, int events, void *arg){  
	ioEvent_s *ev = (ioEvent_s*)arg;  
	int len;  
	// receive data
	len = recv(fd, ev->buff+ev->len, sizeof(ev->buff)-1-ev->len, 0);    
	eventDel(g_epollFd, ev);
	if(len > 0){
		ev->len += len;
		ev->buff[len] = '\0';  
		printf("C[%d]:%s\n", fd, ev->buff);  
		// change to send event  
		eventSet(ev, fd, sendData, ev);  
		eventAdd(g_epollFd, EPOLLOUT, ev);  
	}  
	else if(len == 0){  
		close(ev->fd);  
		printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev-g_Events);  
	}  
	else{  
		close(ev->fd);  
		printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));  
	}  
}  
// send data  
void sendData(int fd, int events, void *arg){  
	ioEvent_s *ev = (ioEvent_s*)arg;  
	int len;  
	// send data  
	len = send(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset, 0);
	if(len > 0){
		printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);
		ev->s_offset += len;
		if(ev->s_offset == ev->len){
			// change to receive event
			eventDel(g_epollFd, ev);  
			eventSet(ev, fd, recvData, ev);  
			eventAdd(g_epollFd, EPOLLIN, ev);  
		}
	}  
	else{  
		close(ev->fd);  
		eventDel(g_epollFd, ev);  
		printf("send[fd=%d] error[%d]\n", fd, errno);  
	}  
}  

