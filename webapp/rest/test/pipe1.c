#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
 

void main(void){
    int cgi_output[2];
    int cgi_input[2];
    pid_t pid;
    int status;
    int i;
    char c;

    /* 建立管道*/
    if (pipe(cgi_output) < 0) {
        /*错误处理*/  
        return;
    }
    /*建立管道*/
    if (pipe(cgi_input) < 0) {
        /*错误处理*/
        return;
    }
 
    if ((pid = fork()) < 0 ) {
        /*错误处理*/
        return;
    }
    if (pid == 0){
	printf("son\n");
        /* 把 STDOUT 重定向到 cgi_output 的写入端 */
       // dup2(cgi_output[1], 1);
        /* 把 STDIN 重定向到 cgi_input 的读取端 */
        //dup2(cgi_input[0], 0);
        /* 关闭 cgi_input 的写入端 和 cgi_output 的读取端 */
        close(cgi_output[0]);
        close(cgi_input[1]); 
	char *p_str = "son";
	char buff[20];
	while (read(cgi_input[0], &c, 1) > 0){
		printf("son read:%c\n",c);
		strcat(buff,&c);
	}
	printf("end:%d",c);
	strcat(buff, p_str);
	printf("buff%s\n", buff);
	for (i = 0; i < strlen(buff); i++) {
		printf("son write%c\n",buff[i]);
		write(cgi_output[1],&buff[i], 1);
	}
        exit(0);
    } else {    /* parent */
        /* 关闭 cgi_input 的读取端 和 cgi_output 的写入端 */
	printf("farther\n");
        close(cgi_output[1]);
        close(cgi_input[0]);      
	char *p_f = "farther";
	char buff[20];
	for (i = 0; i < sizeof(p_f); i++) {
		printf("farther write\n");
		write(cgi_input[1], p_f++, 1);
	}
        close(cgi_input[1]);
        while (read(cgi_output[0], &c, 1) > 0){
		printf("farther read%c\n",c);
        	strcat(buff,&c);
	}	
        /*关闭管道*/
        close(cgi_output[0]);
        //close(cgi_input[1]);
	printf("%s\n", buff);
	waitpid(pid, &status, 0);
    }	
}
