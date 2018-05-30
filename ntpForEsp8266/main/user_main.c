#include "esp_common.h"
#include "lwip/mem.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/i2c_master.h"
#include "time.h"
#include "proto.h"

#define DEMO_SERVER_PORT (123) 

#define DEMO_WIFI_SSID     "hhhh"
#define DEMO_WIFI_PASSWORD  "11111111"
#define OTA_TIMEOUT 120000  //120000 ms

/*********************global param define start ******************************/
NTP ntpRecvMsg, ntpSentMsg;
LOCAL xTaskHandle *ntp_task_handle = NULL;
LOCAL os_timer_t upgrade_timer;
/*********************global param define end *******************************/

void drv_print_buff(uint8_t *str, uint8_t *buf, uint32_t len)
{
	uint32_t i = 0;
	printf("=======show %s sram======", str);
	for(i=0; i<len; i++) {
		if (0 == (i%16)) {
			printf("\n[%04X] ", i);
		}
		printf("%02X ", buf[i]);
	}
	printf("\n");
}


/******************************************************************************
 * FunctionName : ntp_begin
 * Description  : ntp_task function
 * Parameters   : task param
 * Returns      : none
 *******************************************************************************/
void ntp_begin(void *pvParameters)
{
    uint32_t fd, nbytes, addrlen;
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
		addr.sin_port = htons(DEMO_SERVER_PORT);

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
		// int settimeofday(const struct timeval *tv, const struct timezone *tz);
		struct timeval tv;

		tv.tv_sec = DS3231_getTime_t() - FROM1900TO1970 + SECONDS_8_HOUS;
		tv.tv_usec = 0;
		if (settimeofday(&tv, NULL))
			printf("settimeofday fail!\n");

		if(gettimeofday(&tv, NULL))
			printf("gettimeofday fail!!!\n");
		//printf("gettimeofday  :%s", ctime(&tv.tv_sec));
		/******* test ********/
#endif

    printf("Hello, welcome to NTP server. Current time: %s", ctime(&tv.tv_sec));

	while (1) {
		memset(&ntpRecvMsg, '0', sizeof(NTP));
		memset(&ntpSentMsg, '0', sizeof(NTP));

		if ((nbytes=recvfrom(fd, &ntpRecvMsg, sizeof(NTP), 0, (struct sockaddr *) &addr, (socklen_t *)&addrlen)) < 0) {
			printf("recvfrom failed\r\n");
		}

		inet_ntop(AF_INET, &addr.sin_addr, pbuf, 64);
		printf("\r\n====MESSAGE FROM:%s port:%d %d bytes====\r\n", 
				pbuf,ntohs(addr.sin_port), nbytes);	

		drv_print_buff("ntpRecvMsg", (char *)&ntpRecvMsg, 48);

		time_t ref_time = ntohl(ntpRecvMsg.reference_timestamp[0]);
		time_t ori_time = ntohl(ntpRecvMsg.orignate_timestamp[0]);
		time_t rec_time = ntohl(ntpRecvMsg.receive_timestamp[0]);
		time_t tra_time = ntohl(ntpRecvMsg.transmit_timestamp[0])-FROM1900TO1970 + SECONDS_8_HOUS;

		printf("reference_time:%s", ctime(&ref_time));
		printf("orignate_time :%s", ctime(&ori_time));
		printf("receive_time  :%s", ctime(&rec_time));
		printf("transmit_time :%s", ctime(&tra_time));

		memset(&ntpSentMsg, '0', sizeof(NTP));
		ntpSentMsg.LI_VN_Mode = htonl((LI_00<<30) | (VN<<27) | (MODE4<<24));
		ntpSentMsg.stratum    = 4;	/* 对本地时钟级别的整体识别 */
		ntpSentMsg.poll	= 1;	/* 测试间隔 */
		ntpSentMsg.precision	= 0;	/* 本地时钟精度 */

		
		ntpSentMsg.reference_timestamp[0] = htonl(ntohl(ntpRecvMsg.transmit_timestamp[0])-1);
		ntpSentMsg.orignate_timestamp[0]  = ntpRecvMsg.transmit_timestamp[0];
		ntpSentMsg.orignate_timestamp[1]  = ntpRecvMsg.transmit_timestamp[1];
		ntpSentMsg.receive_timestamp[0]   = ntohl(DS3231_getTime_t());
		ntpSentMsg.transmit_timestamp[0]  = ntohl(DS3231_getTime_t());

		drv_print_buff("ntpSentMsg", (char *)&ntpSentMsg, sizeof(ntpSentMsg));


        if (sendto(fd, &ntpSentMsg, sizeof(NTP), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            printf("sendto failed\r\n");
		}
	}
    vTaskDelay(1);
}

/******************************************************************************
 * FunctionName : ntp_begin
 * Description  : ntp_task function
 * Parameters   : task param
 * Returns      : none
 *******************************************************************************/
void ntp_event_cb(System_Event_t *event)
{
    if (event == NULL) {
        return;
    }

    switch (event->event_id) {
        case EVENT_STAMODE_GOT_IP:
            os_printf("sta got ip , creat fota task\n");
            if (ntp_task_handle == NULL) {
                xTaskCreate(ntp_begin, "ntp_task", 1024, NULL, 1, ntp_task_handle);
            }
            //os_timer_disarm(&upgrade_timer);
            //os_timer_setfn(&upgrade_timer, (os_timer_func_t *) upgrade_recycle, NULL);
            //os_timer_arm(&upgrade_timer, OTA_TIMEOUT, 0);
            break;
        case EVENT_SOFTAPMODE_STADISCONNECTED:
            printf("sta disconnect from AP\n");
            printf("recyle ota task");
            //system_upgrade_flag_set(UPGRADE_FLAG_IDLE);
            //upgrade_recycle();
        default:
            break;
    }
}

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
 *******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
user_init(void)
{
	UART_SetBaudrate(UART0,115200);
    wifi_set_opmode(STATION_MODE);
    {
        struct station_config config;
        bzero(&config, sizeof(struct station_config));
        sprintf(config.ssid, DEMO_WIFI_SSID);
        sprintf(config.password, DEMO_WIFI_PASSWORD);
        wifi_station_set_config(&config);
        wifi_station_connect();
    }
    wifi_set_event_handler_cb(ntp_event_cb);
}

