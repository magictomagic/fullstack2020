//这是一个通过TCP协议来实现聊天的程序
//1.创建socket
//2.为socket绑定地址
//3.向服务端发送链接请求
//4.发送数据
//5.接受数据
//6.断开连接
//47.94.157.136
 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define SERVER_PORT_R 9999
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("please input:tcp ip port\n");
        return -1;
    }
    int sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0){
        perror("socket error\n");
        return -1;
    }
    struct sockaddr_in cli_addr;
    cli_addr.sin_family=AF_INET;
    cli_addr.sin_port=htons(SERVER_PORT_R);
    cli_addr.sin_addr.s_addr=inet_addr(argv[1]);
    //通过命令行参数来给结构体赋值
    socklen_t len=sizeof(struct sockaddr_in);
    int ret=connect(sockfd,(struct sockaddr*)&cli_addr,len);
    if(ret<0){
      perror("connect error\n");
      return -1;
    }
    while(1){
        char buff[1024]={0};
        scanf("%s",buff);
        send(sockfd,buff,strlen(buff),0);
        memset(buff,0x00,1024);
        ssize_t rlen=recv(sockfd,buff,1023,0);
        if(rlen<=0)
        {
            perror("recv error\n");
            return -1;
        }
        printf("server say:%s",buff);

	switch(rlen){
		case 1: printf("ref\n");
		    break;
		case 2: printf("two\n");
	 	    break;
		case 3: printf("three\n");
		    break;
		case 4: goto label1;
		    break;
		default: printf("input length shoud between 1 and 3");
		    break;
	  }
    }
    label1:
    printf("Bye Bye, I'm Client\n");
    close(sockfd);
    return 0;
 
}

