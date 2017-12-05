#include"stdio.h"
#include"unistd.h"
#include"stdlib.h"
#include"string.h"
#include"IPMSG.H"
#include"pthread.h"
#include"client.h"
/*
//文件信息结构体
typedef struct filelist
{
	char name[50];//文件名
	int num;// 文件序号
	long pkgnum;//包编号
	long size;//文件大小
	long ltime;//最后修改时间
	char user[10];	//发送者用户名
	struct filelist *next;
}IPMSG_FILE;
*/

IPMSG_USER *head=NULL;
IPMSG_USER *rear=NULL;

void add(char *name,char *host,int addr){

	IPMSG_USER *p=(IPMSG_USER *)malloc(sizeof(IPMSG_USER));

	if(!p)
		return;

	p->next=NULL;

	if(head==NULL)
		head=rear=p;
	else{
	
		rear->next=p;
		rear=p;
	}

	strcpy(p->name,name);
	strcpy(p->host,host);
	p->s_addr=addr;

}

void addflie(char *name,char num,int bar,long size,char *user,){

	IPMSG_FILE *p=(IPMSG_USER *)malloc(sizeof(IPMSG_USER));

	if(!p)
		return;

	p->next=NULL;

	if(head==NULL)
		head=rear=p;
	else{
	
		rear->next=p;
		rear=p;
	}

	strcpy(p->name,name);
	strcpy(p->host,host);
	p->num=num;
	p->size=size;


}

void print(){

	if(!head)
		return;
	
	IPMSG_USER *p=head;
	
	while(p){
	
		printf("name:%s\n",p->name);
		printf("host:%s\t",p->host);
		printf("address:%d\n",p->s_addr);
		p=p->next;
	}
}

int find(char *name){

	if(!head)
		return;
	
	IPMSG_USER *p=head;
	
	while(p){
		if(strcmp(p->name,name)==0)
			return p->s_addr;
		p=p->next;
	}
}



void re(int addr){

	if(!head)
		return;
	
	IPMSG_USER *p=head;
	IPMSG_USER *s=NULL;

	while(p->next->next!=rear){
	
		if(p->next->s_addr==addr){
		
			s=p->next;
			p->next=p->next->next;
			free(s);
		}
	}
	
	if(p->next->s_addr==addr)
		free(p->next);
	
	if(p->s_addr)
		free(p);

	return;

}
