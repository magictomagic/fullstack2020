/*tcp_chat_server.c*/
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<resolv.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define BUFFER_SIZE 1024
#define SERVER_PORT 8000
#define SERVER_PORT_R 9999
void chatServer(){
int pid;
  int sockfd,client_fd, client_fd_control, sockfd_control;
  socklen_t len, len_control;
  struct sockaddr_in server_addr,client_addr;
  unsigned int lisnum=5;
  char buf[BUFFER_SIZE+1];  
  char buf_control[BUFFER_SIZE+1];  
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
   perror("socket");
   exit(EXIT_FAILURE);
  }
  if((sockfd_control=socket(AF_INET,SOCK_STREAM,0))==-1){
   perror("socket");
   exit(EXIT_FAILURE);
  }


  bzero(&server_addr,sizeof(server_addr));
  server_addr.sin_family=AF_INET;//地址协议
  server_addr.sin_port=htons(SERVER_PORT);//端口号
  server_addr.sin_addr.s_addr=htons(INADDR_ANY);//设置本机地址
  if(bind(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))==-1)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  if(listen(sockfd,lisnum)==-1)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

 if(bind(sockfd_control,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))==-1)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  if(listen(sockfd_control,lisnum)==-1)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("wait for connection\n");
  len=sizeof(struct sockaddr);
  if((client_fd=accept(sockfd,(struct sockaddr*)&client_addr,&len))==-1)
  {//堵塞等待连接
    perror("accept");
    exit(EXIT_FAILURE);
  }
  else
    printf("server:got connection from %s,port %d,socket %d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),client_fd);
  if(-1==(pid=fork()))//创建新进程
  {
   perror("fork");
   exit(EXIT_FAILURE);
  }
  else if(pid==0)
  {
    while(1)
    {
      bzero(buf,BUFFER_SIZE+1);
      printf("input the message to send:\n");
      fgets(buf,BUFFER_SIZE,stdin);
 	//if(buf == EOF)
	//printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
      /*if(!strncasecmp(buf,"quit",4));
      {
         printf("I will close the connection!");
         break;
      }*/
      len=send(client_fd,buf,strlen(buf)-1,0);
      if(len<0)
      {
        printf("message'%s' send failure! error code is %d,errno  message is '%s'\n",buf,errno,strerror(errno));
        break;
      }
   }
 }
 else
 {
  while(1)
  {
   bzero(buf,BUFFER_SIZE+1);
   len=recv(client_fd,buf,BUFFER_SIZE,0);

   len_control=recv(client_fd_control,buf_control,BUFFER_SIZE,0);
   if(len_control == 2){
    printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
      break;
    }
   if(len>0)
     printf("message recv successful:'%s',%dByte recv\n",buf,len);
   else if (len<0)
   {
     printf("recv failure!errno code is %d,errno message is '%s'\n",errno,strerror(errno));
     break;
   }
  }
}
close(client_fd);
close(sockfd);
}




int main(int argc,char *argv[]){
  chatServer();
return 0;
}
      
         
   
   
  
  
