#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <time.h>

#define SERVERPORT "3000"
#define FMT_STAMP		"%ld\r\n"
static void alrm_handler(int n);
char ipstr[64];

int main(int argc, char **argv)
{
	if(argc < 2) {
		fprintf(stderr,"Usage: %s ip\n", argv[0]);
		exit(0);
	}

	strcpy(ipstr, argv[1]);
	signal(SIGALRM,alrm_handler);
	alarm(1);


	while(1) pause();

	exit(0);
}

static void alrm_handler(int n)
{
	time_t stamp;
	FILE *fp;
	int sd;
	struct sockaddr_in raddr;
	sd = socket(AF_INET,SOCK_STREAM,0);
	if(sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(atoi(SERVERPORT));
	inet_pton(AF_INET, ipstr,&raddr.sin_addr);

	while(1) {
		if(connect(sd,(void *)&raddr,sizeof(raddr)) < 0) {
			perror("connect()");
			sleep(3);
			continue;
		} else
			break;
	}

	fp = fdopen(sd,"r+");
	if(fp == NULL)
	{
		perror("fdopen()");
		exit(1);
	}

	if(fscanf(fp,FMT_STAMP,&stamp) < 1)
		fprintf(stderr,"fscanf()\n");
	else {
		struct timespec tmspec;
		tmspec.tv_sec = stamp;
		tmspec.tv_nsec = 0;

		printf("stamp = %ld\n",stamp);
		if( clock_settime( CLOCK_REALTIME, &tmspec) == -1 ) {
			perror( "setclock" );
		}
	}
	alarm(60);
	fclose(fp);
}
