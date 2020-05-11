
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
#include <assert.h>
#define SERVER_PORT_R 9999
//47.94.157.136
#define SERVER_PORT 8000
#define MAX_CONNETCION_COUNT 20 //最大连接数
#define BUFFER_SIZE 1024
#define MAX_SIZE_OF_FILE_NAME 512
//#define FILE_NAME "ss.jpg"
void chatServer(){
int pid;
  int sockfd,client_fd;
  socklen_t len;
  struct sockaddr_in server_addr,client_addr;
  unsigned int lisnum=5;
  char buf[BUFFER_SIZE+1];   
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
   perror("socket");
   exit(EXIT_FAILURE);
  }
  bzero(&server_addr,sizeof(server_addr));
  server_addr.sin_family=AF_INET;//地址协议
  server_addr.sin_port=htons(SERVER_PORT);//端口号
  server_addr.sin_addr.s_addr=htons(INADDR_ANY);//设置本机地址
    int on = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));


  if(ret = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr)) < 0)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  if(listen(sockfd,lisnum)==-1)
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
    while(1){
    bzero(buf,BUFFER_SIZE+1);
    len=recv(client_fd,buf,BUFFER_SIZE,0);
    if(len>0 && len < 1001)
     	printf("message recv successful:'%s',%dByte recv\n",buf,len);
    else if (len<0){
     	printf("recv failure!errno code is %d,errno message is '%s'\n",errno,strerror(errno));
     	break;
    }else{
	printf("return to menu\n");
	break;

    }
  }
}
close(client_fd);
close(sockfd);
}

struct TCP_STRUCT_DATA
{
int m_cmd;
int m_data_len;
};

enum TCP_CMD
{
GET_FILE = 0,
//...
};

long get_file_size(char *file_name)
{
assert(file_name != NULL);
if(access(file_name, F_OK))
{
printf("####L(%d) file:%s not exist!", __LINE__, file_name);
return -1;
}

FILE *fp = fopen(file_name, "rb");
if(NULL == fp )
{
printf("file:%s can not open!\n", file_name);
return -1;
}

long file_size = 0;
fseek(fp, 0, SEEK_END); //把文件内部指针移动到文件尾部
file_size = ftell(fp); //返回指针偏离文件头的位置(即文件中字符个数)
fseek(fp, 0, SEEK_SET); //把文件内部指针移回到文件头部

fclose(fp);

return file_size;
}

void sendFile(){
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);//允许任何IP连接server
//server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    int server_socket = socket(PF_INET, SOCK_STREAM, 0); //创建TCP socket套接字
    int on = 1;
    int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if( server_socket < 0){
	printf("####L(%d) create socket failed!", __LINE__);
	exit(1);
    }

    if(ret = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))){ //绑定端口
	printf("####L(%d) bind port : %d failed!", __LINE__, SERVER_PORT);
	exit(1);
    }

    if ( listen(server_socket, MAX_CONNETCION_COUNT) ){//监听

	printf("####L(%d) server listen failed!", __LINE__);
	exit(1);
    }
  
    while(1){ //服务器端一直运行

// 输入文件名 并放到缓冲区buffer中等待发送
    	char file_name[MAX_SIZE_OF_FILE_NAME+1];
    	bzero(file_name, MAX_SIZE_OF_FILE_NAME+1);
    	printf("Please Input File Name On Server:\t");
   	scanf("%s", file_name);
   	struct sockaddr_in client_addr;
   	socklen_t length = sizeof(client_addr);

  	int new_server_socket = accept(server_socket, (struct sockaddr *)&client_addr, &length);
    	if ( new_server_socket < 0){
	    printf("####L(%d) server accept failed!\n", __LINE__);
            break;
       	}

    	char data_tmp[512] = {0};//要发的数据部分内容
    	long file_size = get_file_size(file_name);
    	if(-1 == file_size){
	    printf("####L(%d) get_file_size err!\n", __LINE__);
	    break;
    	}

        sprintf(data_tmp, "<file><name>%s</name><size>%ld</size></file>", file_name, file_size); //要发送的数据部分内容

        struct TCP_STRUCT_DATA struct_data;//数据头
        memset(&struct_data, 0x0, sizeof(struct_data));
    	struct_data.m_cmd = GET_FILE; //获取文件命令
    	struct_data.m_data_len = strlen(data_tmp); //数据部分真实的长度
//struct_data.m_data_len =sizeof(data_tmp);//512是错误的

/*关键部分*/
    	char buffer[BUFFER_SIZE];
    	bzero(buffer, BUFFER_SIZE);
    	int send_len = sizeof(struct_data);
    	memcpy(buffer, &struct_data, send_len);

//向服务器发送buffer中的数据
    	int len = send(new_server_socket, buffer, send_len, 0); //发数据头部分
    	if(len < 0){
   	    printf("####L(%d) send err...\n", __LINE__);//发送失败
	    break;
    	}

    	usleep(100 * 1000); //要休眠一下 否则第二次发过去的数据可能来不及接收到
    	len = send(new_server_socket, data_tmp, struct_data.m_data_len, 0);
    	if(len < 0){
    	    printf("####L(%d) send err...\n", __LINE__);//发送失败
	    break;
    	}

    	usleep(100 * 1000); //要休眠一下 否则第二次发过去的数据可能来不及接收到

    	FILE *fp = fopen(file_name, "rb");//以二进制方式打开文件
    	if(NULL == fp ){
  	    printf("####L(%d) file:%s not found!\n", __LINE__, file_name);
    	}else{
	    int file_block_length = 0;
	    bzero(buffer, BUFFER_SIZE);//缓冲区清0
	    while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {//读文件

	    	printf("####L(%d) file_block_length = %d\n", __LINE__, file_block_length);
	    	if(send(new_server_socket, buffer, file_block_length, 0) < 0) {//发送文件数据

		    printf("####L(%d) send file:%s failed\n", __LINE__, file_name);
		    break;
	        }
	    bzero(buffer, BUFFER_SIZE);
	    }
    	fclose(fp);
    	printf("####L(%d) file:%s transfer finished!\n", __LINE__, file_name);
    	}
//关闭与客户端的连接
    close(new_server_socket);
    break;
    } // while(1)

//关闭监听用的socket
close(server_socket);

}

void transFS(int new_sockfd, char *buff){
    send(new_sockfd,buff,strlen(buff),0);
}


int main(int argc,char *argv[])
{
  
    int sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0){ 
        perror("socker error\n");
        return -1;
    }
    struct sockaddr_in ser_addr;
    ser_addr.sin_family=AF_INET;
    // if you want to appoint port
    // ser_addr.sin_port=htons(atoi(argv[2]));
    ser_addr.sin_port=htons(SERVER_PORT_R);
    ser_addr.sin_addr.s_addr=htons(INADDR_ANY);
    socklen_t len=sizeof(struct sockaddr_in);

    int on = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    ret=bind(sockfd,(struct sockaddr*)&ser_addr,len);
    //为socket绑定地址
    if(ret<0){
   	perror("bind error\n");
   	return -1;
    }
    //服务端开始监听 这时候服务端可以请求链接
    if(listen(sockfd,5)<0){//listen 函数服务端用来监听, 第二个参数为 最大的同时并发连接数
      	perror("listen error\n");
      	return -1;
    }
    while(1){
      	int new_sockfd; 
      	struct sockaddr_in addr;
      	len=sizeof(struct sockaddr_in);
      	new_sockfd=accept(sockfd,(struct sockaddr*)&addr,&len);
      	if(new_sockfd<0){
            perror("accept error\n");
            continue;//这里不能直接退出，因为这个链接失败了可以链接别的socket
        }
      	printf("Login in, success!\n");
      	printf("Welcom: %s,%d,\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
       //这里我们输出一下链接过来的socket的ip地址和port端口号
       while(1){
          //接受数据
          char buff[1024]={0};
	  char check1[1024]={123};
          ssize_t rlen=recv(new_sockfd,buff,1023,0);
          if(rlen<0){
              perror("recv error\n");
              close(new_sockfd);
              continue;//这里和客户端不同不能直接关闭你的程序
          }else if(rlen==0){
              printf("perr shutdown\n");
              close(new_sockfd);
              continue;//如果返回0的话代表那个链接断开了
          }
	
          printf("client %s:%d say:%s\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port),buff);
	
	  switch(rlen){
		case 1: transFS(new_sockfd, buff);
			sendFile();
		    break;
		case 2: transFS(new_sockfd, buff);
			chatServer();
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

