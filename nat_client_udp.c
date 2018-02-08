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
}IP;
 
typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;
 
void echo_cli(int sockfd,SA *addr,socklen_t *len)
{
 char buf[1024];
 
 while(1)
 {
  bzero(buf,sizeof(buf));
  printf(">");
  fflush(stdout);
  fgets(buf,sizeof(buf)-1,stdin);
  sendto(sockfd,buf,strlen(buf),0,
    addr,sizeof(SA_IN));
 
  bzero(buf,sizeof(buf));
  recvfrom(sockfd,buf,sizeof(buf)-1,0,
    addr,len);
  printf("%s",buf);
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
 
 sendto(sockfd,&s,sizeof(char),0,
   (SA *)&server,sizeof(SA_IN));
 recvfrom(sockfd,&ip,sizeof(IP),0,
   (SA *)&server,&addrlen);
 
 printf("IP:%s\tPort:%d\n",inet_ntoa(ip.ip),ntohs(ip.port));
 
 server.sin_addr=ip.ip;
 server.sin_port=ip.port;
 echo_cli(sockfd,(SA *)&server,&addrlen);
 
 close(sockfd);
 return 0;
}