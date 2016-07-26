#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <arpa/inet.h>
#include "proto.h"

#define IPSTRSIZE	40

int main()
{
	int sd;

	struct sockaddr_in laddr,raddr;
	socklen_t raddr_len;

	struct msg_st rmsg, smsg;
	char ipstr[IPSTRSIZE];

	sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
	if(sd < 0) {
		perror("socket()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi("123"));
	inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);

	if(bind(sd,(void *)&laddr,sizeof(laddr)) < 0) {
		perror("bind()");
		exit(1);
	}

	raddr_len = sizeof(raddr);	/*!!*/
	memset(&rmsg, 0, sizeof(rmsg));

	while(1) {
		if(recvfrom(sd,(char *)&rmsg,sizeof(rmsg),0,(void *)&raddr,&raddr_len) < 0) {
			perror("recvfrom()");
			exit(1);
		}

		/***********************     接收部分    ************************/
		inet_ntop(AF_INET,&raddr.sin_addr,ipstr,IPSTRSIZE);
		printf("----MESSAGE FROM : %s:%d-----\n",ipstr,ntohs(raddr.sin_port));	

		rmsg.LI_VN_Mode  = ntohl(rmsg.LI_VN_Mode);
		rmsg.stratum     = ntohl(rmsg.stratum);
		rmsg.poll  = ntohl(rmsg.poll);
		rmsg.precision= ntohl(rmsg.precision);

		rmsg.rootdelay= ntohl(rmsg.rootdelay);
		rmsg.rootdisp = ntohl(rmsg.rootdisp);
		rmsg.refid    = ntohl(rmsg.refid);

		rmsg.reftime   = be64toh(rmsg.reftime);
		rmsg.origtime  = be64toh(rmsg.origtime);
		rmsg.recetime  = be64toh(rmsg.recetime);
		//rmsg.transtime = be64toh(rmsg.transtime);

		time_t timest = ntohl(rmsg.transtime) - FROM1900TO1970;

		//printf("rootDelay = %f\n", (float)rmsg.rootdelay);
		//printf("rootDisp = %f\n", (float)rmsg.rootdisp);

		printf("reftime : %s",   ctime((time_t*)(&rmsg.reftime)));
		printf("origtime : %s",  ctime((time_t*)(&rmsg.origtime)));
		printf("recetime : %s",  ctime((time_t*)(&rmsg.recetime)));
		printf("transtime : %s", ctime(&timest));

		/***********************     发送部分    ************************/
		smsg.LI_VN_Mode = htonl((LI_00<<30) | (VN<<27) | (MODE4<<24));
		smsg.stratum    = 4;	/* 对本地时钟级别的整体识别 */
		smsg.poll	= 5;	/* 测试间隔 */
		smsg.precision	= 0;	/* 本地时钟精度 */

		smsg.reftime   = htonl(time(NULL) + FROM1900TO1970);
		smsg.origtime  = rmsg.transtime;
		smsg.recetime  = htonl(time(NULL) + FROM1900TO1970);
		smsg.transtime = htonl(time(NULL) + FROM1900TO1970);

		if(sendto(sd,&smsg,sizeof(smsg),0,(void *)&raddr,sizeof(raddr)) < 0) {
			perror("sendto()");
			exit(1);
		}

		printf("\n");
		memset(&rmsg, 0, sizeof(rmsg));
		memset(&smsg, 0, sizeof(smsg));
	}

	close(sd);
	exit(0);
}

