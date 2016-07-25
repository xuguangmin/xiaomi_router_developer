#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <arpa/inet.h>
       #define _BSD_SOURCE             /* See feature_test_macros(7) */
              #include <endian.h>
long lfptoa(uint32_t nhi, uint32_t nnf);

#include "proto.h"

#define IPSTRSIZE	40


int main()
{
	time_t timeStamp;
	int sd;
	float val;
	struct sockaddr_in laddr,raddr;
	socklen_t raddr_len;
	struct msg_st rbuf;
	char ipstr[IPSTRSIZE];

	sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
	if(sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi("123"));
	inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);

	if(bind(sd,(void *)&laddr,sizeof(laddr)) < 0)
	{
		perror("bind()");
		exit(1);
	}

	raddr_len = sizeof(raddr);	/*!!*/
	memset(&rbuf, 0, sizeof(rbuf));

	while(1)
	{

		if(recvfrom(sd,(char *)&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len) < 0)
		{
			perror("recvfrom()");
			exit(1);
		}

		inet_ntop(AF_INET,&raddr.sin_addr,ipstr,IPSTRSIZE);
		printf("----MESSAGE FROM : %s:%d-----\n",ipstr,ntohs(raddr.sin_port));	

		//val = ntohl(rbuf.rootdelay);
		printf("rootDelay = %d\n", rbuf.rootdelay);

		val = ntohl(rbuf.rootdisp);
		printf("rootDisp = %f\n", val);

//		timeStamp = ntohl(rbuf.oritime);
//		printf("origtime = %ld  time: %s", timeStamp, ctime(&timeStamp));
//
//		timeStamp = ntohl(rbuf.reftime);
//		printf("recetime = %ld  time: %s", timeStamp, ctime(&timeStamp));
//
		timeStamp = be64toh(rbuf.transtime);
		printf("transtime = %ld  time: %s", timeStamp, ctime(&timeStamp));
		timeStamp = le64toh(rbuf.transtime);
		printf("transtime = %ld  time: %s", timeStamp, ctime(&timeStamp));
		timeStamp = ntohl(rbuf.transtime);
		printf("transtime = %ld  time: %s", timeStamp, ctime(&timeStamp));
		//printf("transtime = %ld  time: %s", timeStamp, ctime(&timeStamp));

		//timeStamp = lfptoa(rbuf.transtime.ui, rbuf.transtime.uf);
		//printf("transtime = %lu  time: %s", timeStamp, ctime((time_t*)&(rbuf.transtime)));

		printf("refId = %u",ntohl(rbuf.refid));

		memset(&rbuf, 0, sizeof(rbuf));
		printf("\n\n");

		//if(sendto(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,sizeof(raddr)) < 0)
		//{
		//	perror("sendto()");
		//	exit(1);
		//}
	}

	close(sd);

	exit(0);
}


long lfptoa(uint32_t nhi, uint32_t nnf)
{
	uint64_t ret;

	ret = ntohl(nhi);

	ret = ret << 32;
	ret = ntohl(nnf);

	return ret;
}
