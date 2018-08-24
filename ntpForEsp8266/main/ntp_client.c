#include "user_def.h"

#include <netdb.h>

#define ALIYUN_NTP		"182.92.12.11" //ntp1.aliyun.com

/***************global param define start ***********************/
NTP ntpRecvMsg, ntpSentMsg;
uint32_t sockfd = -1;
/***************global param define end *************************/

void ntp_client_end()
{
	close(sockfd);
}

in_addr_t inet_host(const char *host)
{
	in_addr_t saddr;
	struct hostent *hostent;

	if ((saddr = inet_addr(host)) == INADDR_NONE) {
		if ((hostent = gethostbyname(host)) == NULL)
			return INADDR_NONE;

		memmove(&saddr, hostent->h_addr, hostent->h_length);
	}

	return saddr;
}

int get_ntp_packet(void *buf, size_t *size)
{
    NTP *ntp;
    struct timeval tv;

    if (!size || *size < sizeof(NTP))
        return -1;

    memset(buf, 0, *size);

    ntp = (NTP *) buf;
    ntp->li = LI_00;
    ntp->version = VERSION;
    ntp->mode = MODE3;
    ntp->stratum = 4;
    ntp->poll = 1;
    ntp->precision = 0;

	ntp->transmit_timestamp[0] = ntohl(DS3231_Get_Time_Base1900());

    *size = sizeof(NTP);

    return 0;
}

void ntp_client_begin(void * para)
{
    uint32_t addrlen, maxfd1;
	int32_t nbytes = sizeof(NTP);
	NTP ntpRecvMsg, ntpSentMsg;
    struct sockaddr_in servaddr;
	char *pbuf = NULL;
    fd_set readfds;
	struct timeval timeout;
	time_t setTimeStamp;

	addrlen = sizeof(struct sockaddr_in);
	pbuf = (char *) zalloc(64);
	memset(pbuf, '0', 64);

	if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		close(sockfd);
		perror("socket fail !");
		return ;
	}
	printf("socket ok!\r\n");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(NTP_SERVER_PORT);
	servaddr.sin_addr.s_addr = inet_host(ALIYUN_NTP);

	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr)) != 0) {
		perror("connect error:");
		return ;
	}

	int i=5;
	while (i--) {
		bzero(&ntpRecvMsg, sizeof(NTP));
		bzero(&ntpSentMsg, sizeof(NTP));

		if (get_ntp_packet(&ntpSentMsg, &nbytes) != 0) {
			fprintf(stderr, "construct ntp request error \n");
			exit(-1);
		}
		if(send(sockfd, &ntpSentMsg, sizeof(NTP), 0) < 0)
        	perror("sendto fail");

		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		maxfd1 = sockfd + 1;

		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		if (select(maxfd1, &readfds, NULL, NULL, &timeout) > 0) {
			if (FD_ISSET(sockfd, &readfds)) {
				if ((nbytes = recv(sockfd, &ntpRecvMsg, sizeof(NTP), 0)) < 0) {
					perror("recv error");
					return;
				}

				/*
				gettimeofday(&recvtv, NULL);
				offset = get_offset((struct ntphdr *) buf, &recvtv);

				gettimeofday(&tv, NULL);
				tv.tv_sec += (int) offset;
				tv.tv_usec += offset - (int) offset;

				settimeofday(&tv, NULL);
				*/

				setTimeStamp = ntohl(ntpRecvMsg.transmit_timestamp[0]) 
										- FROM1900TO1970 + SECONDS_8_HOUS;
				DS3231_Set(&setTimeStamp);

				struct timeval tv = {
					.tv_sec = DS3231_Get_Time_Base1970(),
					.tv_usec = 0
				};

				if (settimeofday(&tv, NULL))
					printf("settimeofday fail!\n");
			}
		}
		vTaskDelay(300);

		/*=============  for print  ==================*/
#if 0
		time_t ref_time = ntohl(ntpRecvMsg.reference_timestamp[0]) - FROM1900TO1970 + SECONDS_8_HOUS;
		time_t ori_time = ntohl(ntpRecvMsg.orignate_timestamp[0]) -FROM1900TO1970 + SECONDS_8_HOUS;
		time_t rec_time = ntohl(ntpRecvMsg.receive_timestamp[0]) - FROM1900TO1970 + SECONDS_8_HOUS;
		time_t tra_time = ntohl(ntpRecvMsg.transmit_timestamp[0])-FROM1900TO1970 + SECONDS_8_HOUS;

		printf("reference_time:%s", ctime(&ref_time));
		printf("orignate_time :%s", ctime(&ori_time));
		printf("receive_time  :%s", ctime(&rec_time));
		printf("transmit_time :%s", ctime(&tra_time));
		vTaskDelay(400);
		//drv_print_buff("ntpSentMsg", (char *)&ntpSentMsg, sizeof(ntpSentMsg));
#endif
	}
	close(sockfd);
	vTaskDelete(NULL);
}


