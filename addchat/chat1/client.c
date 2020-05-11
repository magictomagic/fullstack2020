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
int main(int argc,char *argv[])
{
  int sockfd,len;
  struct sockaddr_in ser_addr;
  char buf[MAXBUF+1];
  if(argc!=3)
  {
    printf("error format,it must be:\n%s IP port\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
  {
   perror("socket");
   exit(errno);
  }
  printf("socket created\n");
  bzero(&ser_addr,sizeof(ser_addr));
  ser_addr.sin_family=AF_INET;
  ser_addr.sin_port=htons(atoi(argv[2]));/*atoi（）把命令行字符串转化为整数*/
  /*inet_aton()将点分十进制IP转化为网络流IP，并用新的IP更新第二个参数的结构体*/
  if(inet_aton(argv[1],(struct in_addr*)&ser_addr.sin_addr.s_addr)==0)
  {
   perror(argv[1]);
   exit(errno);
  }
  if(connect(sockfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr))==-1)//发起连接请求
  {
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
   /*if(!strncasecmp(buf,"quit",4));
    {
      printf("I will quit!\n");
      break;
    }*/
   len=send(sockfd,buf,strlen(buf)-1,0);
   if(len<0)
   {
     perror("send");
     break;
   }
  }
}
close(sockfd);
return 0;
}
