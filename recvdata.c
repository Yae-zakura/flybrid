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


extern long caddr;
extern unsigned long b,memory;
extern char filename[120];

void *recvdata(){
/*	
	printf("Do you want to recv file? n(NO)\n");
	char choose=' ';
	scanf("%c",&choose);
	if(choose=='n')
		return;
*/	
//	int sockfd=(int )arg;
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in client_addr;
	unsigned short port=2425;
	
	
	bzero(&client_addr,sizeof(client_addr));
	
	client_addr.sin_family=AF_INET;
	client_addr.sin_port=htons(port);
	client_addr.sin_addr.s_addr=caddr;
	socklen_t clen=sizeof(client_addr);
//	inet_pton(AF_INET,IP,&client_addr.sin_addr);

	char buf[1024]="";
	unsigned long t=time(NULL);
	unsigned long cmd;
	char host[20]="";
	struct passwd *pwd;
	char name[20]="hzw";
	char text[512]="hezhangwei";
	pwd=getpwuid(getuid());
				

	connect(sockfd,(struct sockaddr *)&client_addr,sizeof(client_addr));
	printf("address:%d\n",client_addr.sin_addr.s_addr);
		perror("connect");
	//		socklen_t data_addr_len=sizeof(data_addr);
	
	unsigned long num=0,bar,offset=0;
	
	char rebuf[1024]="";

	t=time(NULL);
	bar=b;
	printf("bar:%lx\n",bar);
	strcpy(name,"hzw");
//	bar=time(NULL);
	int len=sprintf(buf,"%d:%lu:%s:%s:%lu:%lx:%lx:%lx",VAR,t,name,pwd->pw_name,IPMSG_GETFILEDATA,bar,num,offset);
	sendto(sockfd,buf,len,0,(struct sockaddr *)&client_addr,sizeof(client_addr));


	int sum=0;
				
	int fd=open(filename, O_RDWR|O_CREAT, 0666);

	printf("fd:%d\n",fd);
			
	perror("open");


	
	while(sum < memory){	
//	while(len==1024){	
		recvfrom(sockfd,buf,sizeof(buf), 0, (struct sockaddr*)&client_addr,&clen);// &data_addr_len);

//		printf("hjgjfg\n");
		len=write(fd, buf, strlen(buf));

		sum=len+sum;

		bzero(&buf,sizeof(buf));
//	};
	}
	
	printf("recv data success!\n");
	
	return;

}
