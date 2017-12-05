#include"stdio.h"
#include"unistd.h"
#include"stdlib.h"
#include"string.h"
#include"netinet/in.h"
#include"sys/socket.h"
#include"sys/types.h"
#include"arpa/inet.h"
#include"pwd.h"
#include"IPMSG.H"
#include"pthread.h"
#include"client.h"
#include"ip.h"


void exitline(int s){//int sockfd){

//	int sockfd=3;
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);


	unsigned short port=2425;
	struct sockaddr_in exit_addr;


	bzero(&exit_addr,sizeof(exit_addr));
	exit_addr.sin_family=AF_INET;
	exit_addr.sin_port=htons(port);

	inet_pton(AF_INET,IP,&exit_addr.sin_addr);


//	send_addr.sin_addr.s_addr=inet_addr("192.168.255.255");

	char buf[1024]="";
	unsigned long t=time(NULL);
	unsigned long cmd;
	char host[20]="";
	struct passwd *pwd;
	char name[20]="hzw";
	char text[512]="bye";

	int yes=1;
	setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(yes));

	pwd=getpwuid(getuid());
	
	int len=sprintf(buf,"%d:%lu:%s:%s:%lu:%s",VAR,t,name,\
			pwd->pw_name,IPMSG_BR_EXIT,text);

	sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&exit_addr,len);

	exit(0);

}
