#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;
 
typedef struct
{
    struct in_addr ip;
    int port;
}IP;  //记录ip与端口
 
int main(int argc,  char **argv)
{
    SA_IN server;
    int sockfd;
    IP ip[2];
    char s;
    socklen_t addrlen = sizeof(SA_IN);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        return -1;
    }
    bzero(&server, sizeof(SA_IN));
    server.sin_port = htons(8888);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (SA *)&server, sizeof(SA_IN)) == -1)
    {
        perror("bind");
        return -1;
    }
 
    while(1)
    {
        bzero(ip,sizeof(IP)*2);
       
        //接收两个心跳包并记录其与此连接的ip与端口
        recvfrom(sockfd,&s,sizeof(char),0,
          (SA *)&server,&addrlen);
        memcpy(&ip[0].ip,&server.sin_addr,sizeof(struct in_addr));
        ip[0].port=server.sin_port;
        printf("%s\t%d OK\n",inet_ntoa(ip[0].ip),ntohs(ip[0].port));
       
        recvfrom(sockfd,&s,sizeof(char),0,
          (SA *)&server,&addrlen);
        memcpy(&ip[1].ip,&server.sin_addr,sizeof(struct in_addr));
        ip[1].port=server.sin_port;
        printf("%s\t%d OK\n",inet_ntoa(ip[1].ip),ntohs(ip[1].port));
       
        //分别向两台机器发送与此服务连接的ip与端口
        server.sin_addr=ip[0].ip;
        server.sin_port=ip[0].port;
        sendto(sockfd,&ip[1],sizeof(IP),0,
          (SA *)&server,sizeof(SA_IN));
       
        server.sin_addr=ip[1].ip;
        server.sin_port=ip[1].port;
        sendto(sockfd,&ip[0],sizeof(IP),0,
          (SA *)&server,sizeof(SA_IN));
    }
 
 return 0;
}