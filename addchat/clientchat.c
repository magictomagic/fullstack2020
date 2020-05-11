/*tcp_chat_client.c*/
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<resolv.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define MAXBUF 1024
#define SERVER_PORT 8000
#define SERVER_PORT_R 9999
void exitOut(){
 printf("i will end server\n");
len_control=send(sockfd_control,buf_control,strlen(buf_control)-1,0);
}


void chatClient(int argc, char * aaaa){
 int sockfd,len_control, sockfd_control, len;
  struct sockaddr_in ser_addr, ser_addr_control;
  char buf[MAXBUF+1];
  char buf_control[MAXBUF+1] = "as";
  if(argc!=2){
    printf("error format\n");
    exit(EXIT_FAILURE);
  }

  if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
   perror("socket");
   exit(errno);
  }
  if((sockfd_control=socket(AF_INET,SOCK_STREAM,0))==-1){
   perror("socket");
   exit(errno);
  }



  printf("socket created\n");
  bzero(&ser_addr,sizeof(ser_addr));
  bzero(&ser_addr_control,sizeof(ser_addr_control));

  ser_addr.sin_family=AF_INET;
  ser_addr.sin_port=htons(SERVER_PORT);/*atoi（）把命令行字符串转化为整数*/
  ser_addr_control.sin_family=AF_INET;
  ser_addr_control.sin_port=htons(SERVER_PORT);/*atoi（）把命令行字符串转化为整数*/

  /*inet_aton()将点分十进制IP转化为网络流IP，并用新的IP更新第二个参数的结构体*/
    if(inet_aton(aaaa,(struct in_addr*)&ser_addr.sin_addr.s_addr)==0){
   	perror(aaaa);
   	exit(errno);
    }
    if(connect(sockfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr))==-1){//发起连接请求
  	perror("connect");
   	exit(errno);
    }

    if(inet_aton(aaaa,(struct in_addr*)&ser_addr_control.sin_addr.s_addr)==0){
   	perror(aaaa);
   	exit(errno);
    }
    if(connect(sockfd_control,(struct sockaddr*)&ser_addr_control,sizeof(ser_addr_control))==-1){//发起连接请求
  	perror("connect");
   	exit(errno);
    }



  printf("server connected\n");
  pid_t pid;
  if(-1==(pid=fork()))//创建新进程
  {
   perror("fork");
   exit(EXIT_FAILURE);
  }
  else if(pid==0)//子进程用于数据接收
  {
    while(1)
    {
      bzero(buf,MAXBUF+1);
      len=recv(sockfd,buf,MAXBUF,0);
      if(len>0)
          printf(" recv successful:'%s',%dByte recv\n",buf,len);
      else if (len<0)
      {
         perror("recv");
         break;
      }
      else
      {
        printf(" the other one close,quit\n");
        break;
      }
    }
 }
else//父进程用于数据发送
{
 while(1)
 {
   bzero(buf,MAXBUF+1);
   printf("pls send message to send:\n");
   fgets(buf,MAXBUF,stdin);
	
    signal(SIGTSTP, exitOut);

   len=send(sockfd,buf,strlen(buf)-1,0);
   if(len<0)
   {
     perror("send");
     break;
   }
  }
}
close(sockfd);

}

int main(int argc,char *argv[]){
    char * aaaa;
    aaaa = argv[1];
    chatClient(argc, aaaa);
return 0;
}
