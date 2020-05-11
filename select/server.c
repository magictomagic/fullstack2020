
//这是TCP协议来实现服务端的程序
//1.创建socket
//2.为socket绑定地址
//3.开始监听，可以接受客户端的链接请求
//4.获取连接成功的socket
//5.接受数据
//6.发送数据
//7.关闭socket
//
//
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define SERVER_PORT_R 9999
//47.94.157.136

void transFS(int new_sockfd, char *buff){
    send(new_sockfd,buff,strlen(buff),0);



}


int main(int argc,char *argv[])
{
  
   int sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   if(sockfd<0)
   { 
      perror("socker error\n");
      return -1;
   }
   struct sockaddr_in ser_addr;
   ser_addr.sin_family=AF_INET;
  // ser_addr.sin_port=htons(atoi(argv[2]));

   ser_addr.sin_port=htons(SERVER_PORT_R);

   ser_addr.sin_addr.s_addr=htons(INADDR_ANY);
   socklen_t len=sizeof(struct sockaddr_in);
   int ret=bind(sockfd,(struct sockaddr*)&ser_addr,len);
   //为socket绑定地址
   if(ret<0)
   {
   perror("bind error\n");
   return -1;
   }
   //服务端开始监听 这时候服务端可以请求链接
   if(listen(sockfd,5)<0)
   {
      //listen 函数服务端用来监听
      //第一个参数为socket描述符
      //第二个参数为 最大的同时并发连接数
      perror("listen error\n");
      return -1;
   }
   while(1)
   {
      int new_sockfd; 
      struct sockaddr_in addr;
      len=sizeof(struct sockaddr_in);
      new_sockfd=accept(sockfd,(struct sockaddr*)&addr,&len);
      if(new_sockfd<0)
      {
          perror("accept error\n");
          continue;//这里不能直接退出，因为这个链接失败了可以链接别的socket
      }
      printf("Login in, success!\n");
      printf("Welcom: %s,%d,\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
      int flagSRM = 0;
      //这里我们输出一下链接过来的socket的ip地址和port端口号
      while(1)
      {
          //接受数据
          char buff[1024]={0};
	char check1[1024]={123};
          ssize_t rlen=recv(new_sockfd,buff,1023,0);
          if(rlen<0)
          {
              perror("recv error\n");
              close(new_sockfd);
              continue;//这里和客户端不同不能直接关闭你的程序
          }
          else if(rlen==0)
          {
              printf("perr shutdown\n");
              close(new_sockfd);
              continue;//如果返回0的话代表那个链接断开了
          }
	
          printf("client %s:%d say:%s\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port),buff);
	
	  switch(rlen){
		case 1: transFS(new_sockfd, buff);
		    break;
		case 2: printf("two\n");
	 	    break;
		case 3: printf("three\n");
		    break;
		case 4: goto label1;
		    break;
		default: printf("input length shoud between 1 and 3\n");
		    break;
	  }

          
      }
   }
    label1:
    printf("Bye Bye, I'm Server\n");
     close(sockfd);
     return 0;
}

