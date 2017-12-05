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

void *recvdata();

void add(char *name,char *host,int addr);

void re(int addr);
	

long caddr=0;
unsigned long b=0,memory=0;
char filename[120]="";

void *online(void *arg){
	
	int sockfd=(int)arg;
	

	printf("sockfd:%d\n",sockfd);

	unsigned short port=2425;
	struct sockaddr_in server_addr;


	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);

	inet_pton(AF_INET,IP,&server_addr.sin_addr);


//	send_addr.sin_addr.s_addr=inet_addr("192.168.255.255");

	char buf[1024]="";
	unsigned long t=time(NULL);
	unsigned long cmd;
	char host[20]="";
	struct passwd *pwd;
	char name[20]="hzw";
	char text[512]="hezhangwei";

	int yes=1;
	setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(yes));

	pwd=getpwuid(getuid());
	
	int len=sprintf(buf,"%d:%lu:%s:%s:%lu:%s",VAR,t,name,\
			pwd->pw_name,IPMSG_BR_ENTRY,text);


	printf("buf:%s\n",buf);
	printf("len = %d\n",len);
	printf("sockfd = %d\n",sockfd);
	sendto(sockfd,buf,len,0,(struct sockaddr *)&server_addr,sizeof(server_addr));

	perror("sendto");
	

	len=sprintf(buf,"%d:%lu:%s:%s:%lu:%s",VAR,t,name,pwd->pw_name,IPMSG_ANSENTRY,text);
	

	
	struct sockaddr_in client_addr;
	socklen_t clen=sizeof(client_addr);

	while(1){
		char recv_buf[1024]="";
		recvfrom(sockfd,recv_buf,sizeof(recv_buf),\
			0,(struct sockaddr*)&client_addr,&clen);
//		printf("address:%d\n",client_addr.sin_addr.s_addr);
		caddr=client_addr.sin_addr.s_addr;

		char var[64]="";
		
		sscanf(recv_buf,"%[^:]:%lu:%[^:]:%[^:]:%ld:%[^\e]",var,&t,name,host,&cmd,text);

//		printf("name:%s\n",name);
//		printf("host:%s\n",host);

		
		if(GET_MODE(cmd)==IPMSG_ANSENTRY || GET_MODE(cmd)==IPMSG_BR_ENTRY){
		
			add(name,host,client_addr.sin_addr.s_addr);
			printf("name:%s\thas online\n",name);
			printf("host:%s\n",host);
//			printf("addr:%d\n",client_addr.sin_addr.s_addr);
	
			if(GET_MODE(cmd)==IPMSG_BR_ENTRY){
				sendto(sockfd,buf,len,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
	
			}
		}
		
		
		if(GET_MODE(cmd)==IPMSG_BR_EXIT){
			printf("name:%s\thas exit\n",name);
			printf("host:%s\n",host);
			re(client_addr.sin_addr.s_addr);
		}

		if(GET_MODE(cmd)==IPMSG_SENDMSG){
			




			if(GET_OPT(cmd)==(IPMSG_FILEATTACHOPT|IPMSG_SENDCHECKOPT)){
				
				
				b=t;
				
				
				
				char data[512]="";

				char var[64]="";

				unsigned long length=0,mtime;
		
			//	sscanf(recv_buf,"%[^:]:%lu:%[^:]:%[^:]:%ld:%[^\e]",var,&t,name,host,&cmd,text);
				

				printf("b=%lx\n",b);

				printf("name:%s\n",name);

				char *file=recv_buf+strlen(recv_buf)+1;
		
				sscanf(file,"%lx:%[^:]:%lx:%lx:%lx",&length,filename,&memory,&mtime,&cmd);
				
				printf("length:%lu\n",length);
				printf("filename:%s\n",filename);
				printf("memory:%lu\n",memory);
				printf("mtime:%lu\n",mtime);
			
				t=time(NULL);
				strcpy(name,"hzw");

				len=sprintf(buf,"%d:%lu:%s:%s:%lu:%lu",VAR,t,name,pwd->pw_name,IPMSG_RECVMSG,b);
				sendto(sockfd,buf,len,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
				printf("buf=%s\n",buf);
				
				pthread_t tid;
				pthread_create(&tid,NULL,(void*)recvdata,NULL);
				pthread_detach(tid);
				pthread_join(tid,NULL);

				
			//	strcpy(name,"hzw");
			//	bzero(&text,sizeof(text));
				
		
			//	len=sprintf(buf,"%d:%lu:%s:%s:%lu:%s",VAR,t,name,\
			//		pwd->pw_name,IPMSG_GETFILEDATA,text);
			//	sendto(sockfd,buf,len,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
/*				
			struct sockaddr_in data_addr;
			bzero(&data_addr,sizeof(data_addr));
			data_addr.sin_family=AF_INET;
			data_addr.sin_port=htons(port);
			data_addr.sin_addr.s_addr=caddr;
			printf("add:%d\n",caddr);
*/

			}
			printf("name:%s\t",name);
			printf("host:%s\n",host);
			printf("text:%s\n",text);
		
			t=time(NULL);
			strcpy(name,"hzw");
			bzero(&text,sizeof(text));

			if(GET_OPT(cmd)==IPMSG_SENDCHECKOPT){
				
				t=time(NULL);
				strcpy(name,"hzw");
				bzero(&text,sizeof(text));
			
				len=sprintf(buf,"%d:%lu:%s:%s:%lu:%s",VAR,t,name,\
					pwd->pw_name,IPMSG_RECVMSG,text);
				sendto(sockfd,buf,len,0,(struct sockaddr *)&client_addr,sizeof(client_addr));
		
			}
		
		}

//		print();
	

	
	}

}
