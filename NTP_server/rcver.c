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

		if(recvfrom(sd,(char *)&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len) < 0) {
			perror("recvfrom()");
			exit(1);
		}

		inet_ntop(AF_INET,&raddr.sin_addr,ipstr,IPSTRSIZE);
		printf("----MESSAGE FROM : %s:%d-----\n",ipstr,ntohs(raddr.sin_port));	

		//val = ntohl(rbuf.rootdelay);
		//printf("rootDelay = %d\n", rbuf.rootdelay);

		//val = ntohl(rbuf.rootdisp);
		//printf("rootDisp = %f\n", val);

		rbuf.version  = ntohl(rbuf.version);
		rbuf.leap     = ntohl(rbuf.leap);
		rbuf.stratum  = ntohl(rbuf.stratum);
		rbuf.precision= ntohl(rbuf.precision);

		rbuf.rootdelay= ntohl(rbuf.rootdelay);
		rbuf.rootdisp = ntohl(rbuf.rootdisp);
		rbuf.refid    = ntohl(rbuf.refid);

		rbuf.reftime_hi   = ntohl(rbuf.reftime_hi);
		rbuf.reftime_lo   = ntohl(rbuf.reftime_lo);
		rbuf.origtime_hi  = ntohl(rbuf.origtime_hi);
		rbuf.origtime_lo  = ntohl(rbuf.origtime_lo);
		rbuf.recetime_hi  = ntohl(rbuf.recetime_hi);
		rbuf.recetime_lo  = ntohl(rbuf.recetime_lo);
		rbuf.transtime_hi = ntohl(rbuf.transtime_hi) - FROM1900TO1970;
		rbuf.transtime_lo = ntohl(rbuf.transtime_lo);


		timeStamp = rbuf.reftime_hi;
		printf("reftime = %ld  time: %s", timeStamp, ctime(&timeStamp));
		timeStamp = rbuf.origtime_hi;
		printf("origtime = %ld  time: %s", timeStamp, ctime(&timeStamp));
		timeStamp = rbuf.recetime_hi;
		printf("recetime = %ld  time: %s", timeStamp, ctime(&timeStamp));
		timeStamp = rbuf.transtime_hi;
		printf("transtime = %ld  time: %s", timeStamp, ctime(&timeStamp));

		memset(&rbuf, 0, sizeof(rbuf));
		printf("\n\n");
	}

	close(sd);
	exit(0);
}

