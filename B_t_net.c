#include "include/t_net.h"

int s_bind(int domain,int type,uint16_t port){
    SA4 serv;
    //创建socket,返回文件描述符sfd
    int sfd=socket(domain,type,0);
    if(sfd==-1)E_MSG("socket",-1);
    //初始化本地ip地址和端口号
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



