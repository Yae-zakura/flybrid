#include"stdio.h"
#include"unistd.h"
#include"stdlib.h"
#include"string.h"
#include"time.h"
#include"netinet/in.h"
#include"sys/socket.h"
#include"sys/types.h"
#include"sys/stat.h"
#include"arpa/inet.h"
#include"pwd.h"
#include"IPMSG.H"
#include"pthread.h"
#include"client.h"
#include"ip.h"
#include"fcntl.h"

extern char path[40];

void *senddata(void *arg1){
	
	unsigned short port=2425;
	struct sockaddr_in data_addr;
	int sockfd=socket(AF_INET,SOCK_STREAM,0);


	bzero(&data_addr,sizeof(data_addr));
	data_addr.sin_family=AF_INET;
	data_addr.sin_port=htons(port);
	data_addr.sin_addr.s_addr  = htonl(INADDR_ANY);

//	connect(sockfd,(struct sockaddr *)&data_addr,sizeof(data_addr));
	
	
	bind(sockfd,(struct sockaddr *)&data_addr,sizeof(data_addr));
	perror("bind");
	
	socklen_t data_addr_len=sizeof(data_addr);


	listen(sockfd,20);
	perror("listen");

	struct sockaddr_in client_addr;

	char cli_ip[INET_ADDRSTRLEN]="";
	socklen_t client_addr_len=sizeof(client_addr);

	int connfd=accept(sockfd,(struct sockaddr*)&client_addr,&client_addr_len);

	perror("accept");

//	inet_ntop(AF_INET,&client_addr.sin_addr,cli_ip,INET_ADDRSTRLEN);

	char recv_buf[1024]="";
	unsigned long num=0,bar,offset=0,barnum;

	barnum=(unsigned long)arg1;
	
	char buf[1024]="";
	unsigned long t=time(NULL);
	unsigned long cmd;
	char host[20]="";
	struct passwd *pwd;
	char name[20]="hzw";
	char text[512]="";
	char var[64]="";
	
	pwd=getpwuid(getuid());

//	bar=time(NULL);

//	bar=b;

//  int len=sprintf(buf,"%d:%lu:%s:%s:%lu:%s",VAR,t,name,pwd->pw_name,IPMSG_GETFILEDATA,text);
//	sendto(sockfd,buf,len,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
//	int len=sprintf(buf,"%d:%lu:%s:%s:%lu:%lx:%lu:%lu",VAR,t,name,pwd->pw_name,IPMSG_GETFILEDATA,bar,num,offset);
//	sendto(sockfd,buf,len,0,(struct sockaddr *)&data_addr,sizeof(data_addr));

//	printf("getdata:%s\n",buf);

	recvfrom(connfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&client_addr,&client_addr_len);

	sscanf(recv_buf,"%[^:]:%lu:%[^:]:%[^:]:%ld:%lx:%lx:%lx",var,&t,name,host,&cmd,&bar,&num,&offset);
	printf("path:%s\n",path);

	printf("recvname:%s\n",name);

	if(bar==barnum && GET_MODE(cmd)==IPMSG_GETFILEDATA){

		int fd=open(path, O_RDWR|O_CREAT, 0666);

		int len=1024;
		do{
	
			
			len=read(fd, buf, sizeof(buf));

			len=sendto(connfd,buf,len,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
			
			bzero(&buf,sizeof(buf));


	//	len=recvfrom(sockfd,recv_buf,sizeof(recv_buf), 0, (struct sockaddr*)&data_addr, &data_addr_len);

	//	char var[64]="";
	
//		printf("recv:%lu\n", memory);
																		
	//		sendto(sockfd, recv_buf, 4, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));

//	}while(1);
		}while( len== sizeof(buf));
	}
return;
}
