#include "user_def.h"


/*********************global param define start ******************************/
NTP ntpRecvMsg, ntpSentMsg;
/*********************global param define end *******************************/

void ntp_server_begin(void * para)
{
    uint32_t fd, addrlen;
	int32_t nbytes;
	NTP ntpRecvMsg, ntpSentMsg;
    struct sockaddr_in addr;
	char *pbuf = NULL;

	addrlen = sizeof(struct sockaddr_in);
	pbuf = (char *) zalloc(64);
	memset(pbuf, '0', 64);

    while (1) {
        if (-1 == (fd = socket(AF_INET,SOCK_DGRAM,0))) {
            close(fd);
			printf("socket fail !\r\n");
			continue;
		}
		printf("socket ok!\r\n");

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(NTP_SERVER_PORT);

		if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
			printf("bind failed\r\n");
            close(fd);
			continue;
		}
		printf("bind ok!\r\n");
		break;
	}

#if 1
		/******* set system time ********/
		struct timeval tv = {
			.tv_sec = DS3231_Get_Time_Base1970(),
			.tv_usec = 0
		};

		if (settimeofday(&tv, NULL))
			printf("settimeofday fail!\n");

		if(gettimeofday(&tv, NULL))
			printf("gettimeofday fail!!!\n");
    	printf("Hello, welcome to NTP server. Current time: %s", ctime(&tv.tv_sec));
#endif

	while (1) {
		bzero(&ntpRecvMsg, sizeof(NTP));
		bzero(&ntpSentMsg, sizeof(NTP));

		if ((nbytes=recvfrom(fd, &ntpRecvMsg, sizeof(NTP), 0, (struct sockaddr *) &addr, (socklen_t *)&addrlen)) < 0) {
			printf("recvfrom failed\r\n");
		}

		//ntpSentMsg.LI_VN_Mode = htonl((LI_00<<30) | (VN<<27) | (MODE4<<24));
		ntpSentMsg.li = LI_00;
		ntpSentMsg.version = VERSION;
		ntpSentMsg.mode = MODE4;

		ntpSentMsg.stratum    = 4;	/* 对本地时钟级别的整体识别 */
		ntpSentMsg.poll	= 1;	/* 测试间隔 */
		ntpSentMsg.precision	= 0;	/* 本地时钟精度 */

		ntpSentMsg.reference_timestamp[0] = ntohl(DS3231_Get_Time_Base1900());
		ntpSentMsg.orignate_timestamp[0]  = ntpRecvMsg.transmit_timestamp[0];
		ntpSentMsg.orignate_timestamp[1]  = ntpRecvMsg.transmit_timestamp[1];
		ntpSentMsg.receive_timestamp[0]   = ntohl(DS3231_Get_Time_Base1900());
		ntpSentMsg.transmit_timestamp[0]  = ntohl(DS3231_Get_Time_Base1900());

        if (sendto(fd, &ntpSentMsg, sizeof(NTP), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            printf("sendto failed\r\n");
		}

		/*=============  for print  ==================*/
#if 1
		inet_ntop(AF_INET, &addr.sin_addr, pbuf, 64);
		printf("\r\n====MESSAGE SENDTO:%s port:%d %d bytes====\r\n", 
				pbuf,ntohs(addr.sin_port), nbytes);	

		time_t ref_time = ntohl(ntpSentMsg.reference_timestamp[0]) - FROM1900TO1970 + SECONDS_8_HOUS;
		time_t ori_time = ntohl(ntpSentMsg.orignate_timestamp[0]) -FROM1900TO1970 + SECONDS_8_HOUS;
		time_t rec_time = ntohl(ntpSentMsg.receive_timestamp[0]) - FROM1900TO1970 + SECONDS_8_HOUS;
		time_t tra_time = ntohl(ntpSentMsg.transmit_timestamp[0])-FROM1900TO1970 + SECONDS_8_HOUS;

		printf("reference_time:%s", ctime(&ref_time));
		printf("orignate_time :%s", ctime(&ori_time));
		printf("receive_time  :%s", ctime(&rec_time));
		printf("transmit_time :%s", ctime(&tra_time));
		//drv_print_buff("ntpSentMsg", (char *)&ntpSentMsg, sizeof(ntpSentMsg));
#endif
	}
}


