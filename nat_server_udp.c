#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
 
#define SER "31.220.48.103"
#define PORT 4511
 
typedef struct
{
 struct in_addr ip;
 int port;
}IP; //ip与端口
 
typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;
 
//回射服务
void echo_ser(int sockfd,SA *addr,socklen_t *len)
{
 char buf[1024];
 
 while(1)
 {
  bzero(buf,sizeof(buf));
  //接收B发来的数据
  recvfrom(sockfd,buf,sizeof(buf)-1,0,
    addr,len);
  printf("%s",buf);
  //向B发送数据
  sendto(sockfd,buf,strlen(buf),0,
    addr,sizeof(SA_IN));
 
  buf[strlen(buf)-1]='\0';
  if(strcmp(buf,"exit") == 0)
   break;
 }
}
 
int main(int argc,char **argv)
{
 int sockfd;
 SA_IN server;
 IP ip;
 socklen_t addrlen=sizeof(SA_IN);
 char s='a';
 
 sockfd=socket(AF_INET,SOCK_DGRAM,0);
 
 bzero(&server,sizeof(SA_IN));
 server.sin_family=AF_INET;
 server.sin_addr.s_addr=inet_addr(SER);
 server.sin_port=htons(PORT);
 
 //向S发包
 sendto(sockfd,&s,sizeof(char),0,
   (SA *)&server,sizeof(SA_IN));
 //接收B的ip与端口
 recvfrom(sockfd,&ip,sizeof(IP),0,
   (SA *)&server,&addrlen);
 
 printf("IP:%s\tPort:%d\n",inet_ntoa(ip.ip),ntohs(ip.port));
 server.sin_addr=ip.ip;
 server.sin_port=ip.port;
 
 echo_ser(sockfd,(SA *)&server,&addrlen);
 
 close(sockfd);
 return 0;
}