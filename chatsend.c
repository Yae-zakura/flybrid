#include"stdio.h"
#include"unistd.h"
#include"stdlib.h"
#include"string.h"
#include"netinet/in.h"
#include"sys/socket.h"
#include"sys/types.h"
#include"sys/stat.h"
#include"arpa/inet.h"
#include"fcntl.h"
#include"pwd.h"
#include"IPMSG.H"
#include"pthread.h"
#include"client.h"
#include"ip.h"

/*
struct in_addr{
	in_addr_t s_addr;
}

struct sockaddr_in{
	sa_family_t sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
	unsigned char sin_zero[8];
}
*/
extern IPMSG_USER *head;
extern IPMSG_USER *rear;

void senddata(void *arg1);

int find(char *name);

void print();

char path[40]="";

void *chatsend(void *arg){
//	sleep(1);
//	print();
//	printf("printOK\n");


	int sockfd=(int)arg;

//	char *ser_ip="192.168.1.116";

//	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in send_addr;
	socklen_t lsaddr=sizeof(send_addr);
	unsigned short port=2425;

	bzero(&send_addr,sizeof(send_addr));
	send_addr.sin_family=AF_INET;
	send_addr.sin_port=htons(port);

	sleep(2);
	printf("input a name:\n");
	char recver[20]="";
	fgets(recver,sizeof(recver),stdin);
	recver[strlen(recver)-1]='\0';
	send_addr.sin_addr.s_addr=find(recver);

//	find(recver);
	printf("addr:%d\n",find(recver));
//	printf("addr:%d\n",send_addr.sin_addr.s_addr);
	printf("addrOK\n");
	
//	inet_pton(AF_INET,ser_ip,&server_addr.sin_addr);

	unsigned long t=time(NULL);
	unsigned long cmd;
	char host[20]="";
	
	struct passwd *pwd;
	pwd=getpwuid(getuid());
	
	char name[20]="hzw";
	char text[512]="hezhangwei";
	char buf[1024]="";

while(1){
	fgets(text,sizeof(text),stdin);
	text[strlen(text)-1]='\0';

	char ss[20]="";
	
	sscanf(text,"%[^:]:%[^\e]",ss,path);
	printf("%s\n",ss);
	printf("%s\n",path);
	
	char send_buf[1024]="";
	
	if(strcmp(ss,"&senddata")==0){

		
		struct stat statbuf;

		stat(path,&statbuf);

		int num=0;
		t=time(NULL);
//		time_t time=statbuf.st_mtime;

		sprintf(text,"%d:%s:%lx:%lx:%lx:\a",num,path,statbuf.st_size,statbuf.st_mtime,IPMSG_FILE_REGULAR);

		printf("text=%s\n",text);
		printf("size:%ld\n",statbuf.st_size);
//		printf("time:%ld\n",statbuf.st_mtime);

		int len=sprintf(send_buf,"%d:%lu:%s:%s:%lu:%c%s",VAR,t,name,pwd->pw_name,\
				IPMSG_SENDMSG|IPMSG_FILEATTACHOPT|IPMSG_SENDCHECKOPT,'\0',text);
		printf("send_buf=%s\n",send_buf);
		sendto(sockfd,send_buf,len,0,(struct sockaddr*)&send_addr,sizeof(send_addr));


		len=recvfrom(sockfd,send_buf,sizeof(send_buf), 0, (struct sockaddr*)&send_addr,&lsaddr);

		char var[64]="";
		unsigned long barnum=0;
		sscanf(send_buf,"%[^:]:%lu:%[^:]:%[^:]:%ld:%lu",var,&t,name,host,&cmd,&barnum);

//		printf("%lu\n",barnum);

		pthread_t tid4;
		pthread_create(&tid4,NULL,(void*)senddata,(void *)barnum);
		pthread_detach(tid4);
		pthread_join(tid4,NULL);
	}else{

	
	
	int len=sprintf(buf,"%d:%lu:%s:%s:%lu:%s",VAR,t,name,pwd->pw_name,IPMSG_SENDMSG,text);

	sendto(sockfd,buf,len,0,(struct sockaddr*)&send_addr,sizeof(send_addr));

	printf("do you want get userlist start new chat y(yes)\n");
	char c='n';
	scanf("%c",&c);
	char i=getchar();
	if(c=='y'){
	//	system("clear");
		print();
		fgets(recver,sizeof(recver),stdin);
		recver[strlen(recver)-1]='\0';
		send_addr.sin_addr.s_addr=find(recver);
//		find(recver);
		printf("addr:%d\n",find(recver));
//		printf("addr:%d\n",send_addr.sin_addr.s_addr);
//		printf("addrOK\n");
		perror("sendto");
	}
	}
}
//	len=recvfrom((int)sockfd,send_buf,sizeof(send_buf),0,NULL,NULL);

}
