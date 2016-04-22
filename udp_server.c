#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>

#define PORT 8080

int main()
{
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		perror("sock");
		return 1;
	}
	struct sockaddr_in server;
	struct sockaddr_in client;
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	if(bind(sock,(struct sockaddr*)&server,sizeof(server))<0)
	{
		perror("bind");
		close(sock);
		return 1;
	}
	printf("bind success\n");

	socklen_t len = sizeof(client);
	char buf[1024];
	char buf_ip[16];
	memset(buf,'\0',sizeof(buf));
	memset(buf_ip,'\0',sizeof(buf_ip));
	while(1)
	{
		ssize_t ret = recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&client,&len);
		if(ret < 0)
		{
			perror("recvfrom");
			close(sock);
			return 1;
		}
		buf[ret-1] = '\0';
		printf("massage:%s\nip is:%s,port is:%d\n",buf,inet_ntop(AF_INET,&client.sin_addr,buf_ip,sizeof(buf_ip)),ntohs(client.sin_port));
		sendto(sock,"welcome to server\n",22,0,(struct sockaddr*)&client,len);
		write(1,buf,ret);
		printf("\n");
		if(!strncasecmp(buf,"quit",4))
		{
			printf("client close!\nIts ip is:%s,port is:%d\n",inet_ntop(AF_INET,&client.sin_addr,buf_ip,sizeof(buf_ip)),ntohs(client.sin_port));
		}
	}
		close(sock);
		return 0;
}
