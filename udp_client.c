#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<strings.h>
#include<arpa/inet.h>

#define ADDRESS "192.168.136.129"
#define PORT 8080

int main()
{
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		printf("sock error\n");
		return 1;
	}
	struct sockaddr_in server;
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	inet_pton(AF_INET,ADDRESS,&server.sin_addr);
	char buf[1024];
	socklen_t len = sizeof(server);
	while(1)
	{
		printf("please enter:\n");
		memset(buf,'\0',sizeof(buf));
		ssize_t size = read(0,buf,sizeof(buf));
		sendto(sock,buf,size,0,(struct sockaddr*)&server,len);
		buf[size-1] = '\0';
		if(!strncasecmp(buf,"quit",4))
		{
			break;
		}
		memset(buf,'\0',sizeof(buf));
		ssize_t ret = recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&server,&len);
		write(1,buf-3,ret);
	}
	return 0;
}
