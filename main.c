#include"stdio.h"
#include"unistd.h"
#include"stdlib.h"
#include"string.h"
#include"netinet/in.h"
#include"sys/socket.h"
#include"sys/types.h"
#include"signal.h"
#include"arpa/inet.h"
#include"pwd.h"
#include"IPMSG.H"
#include"pthread.h"
#include"client.h"

void add(char *name,char *host,int addr);

void re(int addr);

void *online(void *arg);
void *chatsend(void *arg);
//void *chatrecv(void *arg);

void print();

void exitline(int s);


int main(){

	unsigned short port=2425;
	struct sockaddr_in my_addr;

	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(port);
	my_addr.sin_addr.s_addr  = htonl(INADDR_ANY);

	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	bind(sockfd,(struct sockaddr *)&my_addr,sizeof(my_addr));


	signal(SIGINT,exitline);
	

	pthread_t tid1,tid2;

	pthread_create(&tid1,NULL,(void*)online,(void *)sockfd);
	pthread_create(&tid2,NULL,(void*)chatsend,(void *)sockfd);

	pthread_join(tid2,NULL);
	pthread_join(tid1,NULL);


}

