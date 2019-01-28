#include "t_net.h"
#include "../include/t_param.h"
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
int g_epollFd;
ioEvent_s g_Events[MAX_EVENTS+1]; 
//线程执行函数
void http_handle(int client, int events, void *arg){
	char buf[1024];	//读请求第一行数组
	char method[255];//请求方法
	char url[255];//url 字符数组
	char *p_query = NULL;//处理请求url的指针
	struct stat st;//获取静态文件
	char path[512];
	int cgi = 0;
	ioEvent_s *ev = (ioEvent_s*)arg;  
	//printf("client success %d\n",client);
	//获取请求第一行
	int _line = read_line(client, buf, sizeof(buf));
	if(_line <=0 ){
		close(client);
		return;
	}
	size_t i = 0,j = 0;
        //isspace检查空白字符。在“C”和“POSIX”区域设置中，
        //它们是：space、表单提要（“\\f”）、换行（“\\\n”）、回车（“\\\r”）、.。tab('\\t')和.tab('\v')。
	// 获得请求方法
	//printf("buf:%s",buf);
	while(!isspace(buf[j]) && (i < (sizeof(method)-1))){
		method[i] = buf[j];
		i++;j++;
	}
	method[i]='\0';
	//printf("method:%s\n", method);
	//只处理GET和POST请求
	if (strcasecmp(method, "GET") && strcasecmp(method, "POST")){
        	unknowmethod(client);
        	return;
   	}
	i = 0;
	//读取url地址 上次读到j个字节
	while (isspace(buf[j]) && (j < sizeof(buf)))j++;
    	while (!isspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf))) {
        	/*存下 url */
        	url[i] = buf[j];
       		i++; j++;
	}
 	url[i] = '\0';
	#if 1	//处理 GET 方法
	//处理get参数
	if (strcasecmp(method, "GET") == 0){
		// 待处理请求为 url 
		p_query = url;
		while ((*p_query != '?') && (*p_query != '\0'))p_query++;
		// GET 方法特点，? 后面为参数
		if (*p_query == '?'){
			*p_query = '\0';
			p_query++;
			cgi = 1;
		}
	}
	/*POST请求需要调用其他服务执行 执行路径webapp/rest/...*/
	if (strcasecmp(method, "POST") == 0){
		cgi = 1;
	}
	/*TODO log 文件日志打印 */
	printf("param:%s\n", p_query);
	//格式话url 到 path数组 html 文件在 webapp文件夹中
	// js 在 webapp/js
	// 文件夹中css在 webapp/css文件夹中 
	// 图片在webapp/image文件夹中 
	// 服务路径webapp/rest/...
	sprintf(path, "webapp%s", url);
	printf("path:%s\n", path);
	 //默认情况为 index.html 
	if (path[strlen(path) - 1] == '/')strcat(path, "index.html");
	//根据路径找到对应文件 
	if (stat(path, &st) == -1) {
		//把所有 headers 的信息都丢弃
		while ((_line > 0) && strcmp("\n", buf))
			_line = read_line(client, buf, sizeof(buf));
		//回应客户端找不到
		not_found(client);
		//	eventSet(ev, client, sendData, ev);
        	//	eventAdd(g_epollFd, EPOLLOUT, ev);
		
	}else{
		//如果是个目录，则默认使用该目录下 index.html 文件
		if ((st.st_mode & S_IFMT) == S_IFDIR)strcat(path, "/index.html");
		//if(!access(path,R_OK))serve_file(client, path);
		//else not_found(client);
		if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))cgi = 1;
      		/*不是 cgi,直接把服务器文件返回，否则执行 cgi */
      		if (!cgi)
          		serve_file(client, path);
     		 else
         		execute_cgi(client, path, method, p_query);
	}

	#endif
	close(client);
	return ;
	
}
/* 执行调用服务实现 */
void execute_cgi(int client, const char *path, const char *method, const char *query_string){
    char buf[1024];
    int cgi_output[2];
    int cgi_input[2];
    pid_t pid;
    int status;
    int i,j;
    char c;
    int numchars = 1;
    int content_length = -1;
 
    buf[0] = 'A'; buf[1] = '\0';
    if (strcasecmp(method, "GET") == 0){
        /*把所有的 HTTP header 读取并丢弃*/
        while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
            numchars = read_line(client, buf, sizeof(buf));
    }else{
        /* 对 POST 的 HTTP 请求中找出 content_length */
        numchars = read_line(client, buf, sizeof(buf));
        while ((numchars > 0) && strcmp("\n", buf)){
            /*利用 \0 进行分隔 */
            buf[15] = '\0';
            /* HTTP 请求的特点*/
            if (strcasecmp(buf, "Content-Length:") == 0)
                content_length = atoi(&(buf[16]));
            numchars = read_line(client, buf, sizeof(buf));
        }
        /*没有找到 content_length */
        if (content_length == -1) {
            /*错误请求*/
            bad_request(client);
            return;
        }
    }
    printf("service_start，path:%s,data_length:%d\n", path, content_length); 
    /* 正确，HTTP 状态码 200 */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
 
    /* 建立管道 父 <-*/
    if (pipe(cgi_output) < 0) {
        /*错误处理*/
        cannot_execute(client);
        return;
    }
    /*建立管道 父->子*/
    if (pipe(cgi_input) < 0) {
        /*错误处理*/
        cannot_execute(client);
        return;
    }
     do{
	/*如果有多进程操作 g_Events是否应该加锁？？TODO*/
	for(j = 0; j < MAX_EVENTS; j++){
	    if(g_Events[j].status == 0){
		break;
	    }
	}
	if(j == MAX_EVENTS){
	    printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);
	    break;
	}
	// set nonblocking
	int iret = 0;
	if((iret = fcntl(cgi_output[0], F_SETFL, O_NONBLOCK)) < 0){
	    printf("%s: fcntl nonblocking failed:%d", __func__, iret);
	    break;
	}
	// add a read event for receive data
	eventSet(&g_Events[j], cgi_output[0], NULL, &g_Events[j]);
	eventAdd(g_epollFd, EPOLLIN, &g_Events[j]);
    }while(0);

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
	if((iret = fcntl(cgi_input[1], F_SETFL, O_NONBLOCK)) < 0){
	    printf("%s: fcntl nonblocking failed:%d", __func__, iret);
	    break;
	}
	// add a read event for receive data
	eventSet(&g_Events[i], cgi_input[1],NULL, &g_Events[i]);
	eventAdd(g_epollFd, EPOLLOUT, &g_Events[i]);
    }while(0);
 
    if ((pid = fork()) < 0 ) {
        /*错误处理*/
        cannot_execute(client);
        return;
    }

    if (pid == 0){/*子进程 利用重定向stdin 和stdout 来实现数据向调用服务的input和output操作 
			printf不可用，可以使用 stderr来实现打印操作 c++ 可以使用clog或者cerr一般使用clog*/
        char meth_env[255];
        char query_env[255];
        char length_env[255];
	char port_env[255];
        /* 把 STDOUT 重定向到 cgi_output 的写入端 */
        dup2(cgi_output[1], 1);/*参数output[1]是管道的输出写入端 参数1是stdout*/
        /* 把 STDIN 重定向到 cgi_input 的读取端 */
        dup2(cgi_input[0], 0);/*参数input[0]是管道的输入读取端 参数0是stdin*/
        /* 关闭 cgi_input 的写入端 和 cgi_output 的读取端 */
        close(cgi_output[0]);
        close(cgi_input[1]);
        /*设置 request_method 的环境变量*/
        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);/*向环境变量写入*/
        if (strcasecmp(method, "GET") == 0) {
            /*设置 query_string 的环境变量*/
            sprintf(query_env, "QUERY_STRING=%s", query_string);
            putenv(query_env);
        }
        else {   /* POST */
            /*设置 content_length 的环境变量*/
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }
	sprintf(port_env,"SERVER_PORT",SERVER_PORT);
	//fprintf(2, "cgi-start111\n");
	putenv(port_env);
        //printf("cgi-end\n");
	 /*用 execl 运行 cgi 程序*/
        execl(path, path, NULL);
        exit(0);
    } else {    /* parent */
        /* 关闭 cgi_input 的读取端 和 cgi_output 的写入端 */
        close(cgi_output[1]);
        close(cgi_input[0]);
        if (strcasecmp(method, "POST") == 0){
	    /*接收 POST 过来的数据*/
            for (i = 0; i < content_length; i++) {
                recv(client, &c, 1, 0);
                /*把 POST 数据写入 cgi_input，现在重定向到 STDIN */
                write(cgi_input[1], &c, 1);
            }
	}   
        close(cgi_input[1]);
        /*读取 cgi_output 的管道输出到客户端，该管道输入是 STDOUT */
        while (read(cgi_output[0], &c, 1) > 0){
		printf("%c",c);/*TODO 子进程输出数据打印log文件输出*/ 
		send(client, &c, 1, 0);
 	}
	printf("\n");
        /*关闭管道*/
        close(cgi_output[0]);
        /*close(cgi_input[1]);*/
        /*等待子进程*/
	eventDel(g_epollFd, &g_Events[i]);
	printf("del******** %d\n",i);
	eventDel(g_epollFd, &g_Events[j]);
        waitpid(pid, &status, 0);
    }
}
#if 0
int test(){ 
   	 int length;
	char *method;
	char *inputstring;
	fprintf(2, "cgi-start\n");
	method = getenv("REQUEST_METHOD");//将返回赋予method指针
	printf("method:%s\n", method);
	if(method == NULL) return 1;
	if(!strcmp(method, "POST")){
		length = atoi(getenv("CONTENT_LENGTH"));
		if(length !=0){
			inputstring = malloc(sizeof(char)*length);//stdin不带缓存
			fread(inputstring, sizeof(char), length, stdin);//从标准输入里读取一定数据
		}
	}else if(!strcmp(method, "GET")){
		inputstring =getenv("QUERY_STRING");
		length = strlen(inputstring);
	}
	printf("%S%C%C","Content-Type:text/html;charset=UTF-8", 13, 10);
	printf("<title><title>");
	printf("<h3> 111111</h3>");	
	printf("%C%C", 13, 10);	
	
	return 0;
 }
#endif
void cannot_execute(int client){
    char buf[1024];
 
    /* 回应客户端 cgi 无法执行*/
    sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
    send(client, buf, strlen(buf), 0);
}

void bad_request(int client){
    char buf[1024];
 
    /*回应客户端错误的 HTTP 请求 */
    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "<P>Your browser sent a bad request, ");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    send(client, buf, sizeof(buf), 0);
}
 
//将静态页面写入http
void serve_file(int client, const char *filename){
	FILE *resource = NULL;
	int numchars = 1;
	char buf[1024];
	//printf("serve_file......");
	/*读取并丢弃 header */
	buf[0] = 'A'; 
	buf[1] = '\0';
	while ((numchars > 0) && strcmp("\n", buf)) {
		numchars = read_line(client, buf, sizeof(buf));
	}
	/*打开 sever 的文件*/
	resource = fopen(filename, "r");
	if (resource == NULL){
		//not_found(client);
		NULL;
	}else{
		/*写 HTTP header */
		header(client, filename);
		/*写入文件*/
		cat(client, resource);
	}
	fclose(resource);
}

void cat(int client, FILE *resource){
	char buf[1024];
	//printf("cat ......\n");
	/*读取文件中的所有数据写到 socket */
	fgets(buf, sizeof(buf), resource);
	while (!feof(resource)){
		send(client, buf, strlen(buf), 0);
		fgets(buf, sizeof(buf), resource);
	}
}
 
//读一行
int read_line(int sock, char *buf, int size){
	int i = 0;
	char c = '\0';
	int n;
	/*把终止条件统一为 \n 换行符，标准化 buf 数组*/
	while ((i < size - 1) && (c != '\n')){
		/*一次仅接收一个字节*/
		n = recv(sock, &c, 1, 0);
		if (n > 0){
			/*收到\r则继续接收下个字节，因为换行符可能是 \r\n 如果下一个字节不是\n则把当前\r替换成\n */
			if (c == '\r'){
				/*使用 MSG_PEEK 从\r开始预览下一个字节,并没有读到c里*/
				n = recv(sock, &c, 1, MSG_PEEK);
				/*但如果是换行符说明是\r\n 把\n 读到c里并继续*/
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else 
					c = '\n';
		    	}
		    	/*存到缓冲区*/
		    	buf[i] = c;
		    	i++;
		}else{
			c = '\n';
		}
	}
	buf[i] = '\0';
	printf("%s\n",buf);
	/*返回 buf 数组大小*/
	return(i);
}

//将文件写入http 输出流
void header(int client, const char *filename){
    char buf[1024];
    (void)filename;  
    /*正常的 HTTP header */
    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    /*服务器信息*/
    strcpy(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

//404页面
void not_found(int client){
	char buf[1024];
	/* 404 页面 */
	sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
	/*send(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset, 0);*/
	send(client, buf, strlen(buf), 0);


	/*服务器信息*/
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "your request because the resource specified\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "is unavailable or nonexistent.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}

//不支持的请求类型
void unknowmethod(int client){
	char buf[1024];
	//http请求类型不被支持
	sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	//服务器信息
	sprintf(buf, SERVER_STRING);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</TITLE></HEAD>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}
 

int main(int argc,char *argv[]){
	int server_socket = -1;
	int client_socket = -1;
	pthread_t newthread;
	g_epollFd = epoll_create(MAX_EVENTS);
    	if(g_epollFd <= 0) printf("create epoll failed.%d\n", g_epollFd);
	/*t net 封装socket创建*/
	server_socket = s_listen(AF_INET, SOCK_STREAM, SERVER_PORT, BACKLOG, g_epollFd);
	printf("HttpServer running on port %d\n", SERVER_PORT);//t_param.h  8080
	struct epoll_event events[MAX_EVENTS];
	int checkPos = 0;
	while(1){
		// a simple timeout check here, every time 100, better to use a mini-heap, and add timer event
		/*这里进行简单的超时检查，每次100次，最好使用mini-heap，并添加计时器事件*/
		long now = time(NULL);
		int i = 0;
		for(i = 0; i < 100; i++, checkPos++){
		    if(checkPos == MAX_EVENTS) checkPos = 0; // recycle
		    if(g_Events[checkPos].status != 1) continue;
		    long duration = now - g_Events[checkPos].last_active;
		    if(duration >= 60){ // 60s timeout
			close(g_Events[checkPos].fd);
			printf("[fd=%d] timeout[%d--%d].\n", g_Events[checkPos].fd, g_Events[checkPos].last_active, now);
			eventDel(g_epollFd, &g_Events[checkPos]);
		    }
		}
		// wait for events to happen
		int fds = epoll_wait(g_epollFd, events, MAX_EVENTS, 1000);
		if(fds < 0){
		    printf("epoll_wait error, exit\n");
		    break;
		}
		for(i = 0; i < fds; i++){
			printf("main 500");
		    ioEvent_s *ev = (ioEvent_s*)events[i].data.ptr;
		    if((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)){ // read event
			ev->call_back(ev->fd, events[i].events, ev->arg);
		    }
		    if((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)){ // write event
			ev->call_back(ev->fd, events[i].events, ev->arg);
		    }
		}
	}	
	close(server_socket);
	return 0;
}

