#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "scan_ap.h"

uint32_t find_ap_status = 0;
void scan_done(void *arg, STATUS status)
{
    printf("now doing the scan_done... \n");
    uint8 ssid[33];
    char temp[128];
    if (status == OK) {
        struct bss_info *bss_link = (struct bss_info *) arg;
        while (bss_link != NULL) {
            memset(ssid, 0, 33);
            if (strlen(bss_link->ssid) <= 32)
                memcpy(ssid, bss_link->ssid, strlen(bss_link->ssid));
            else
                memcpy(ssid, bss_link->ssid, 32);
            //printf("(%d,\"%s\",%d,\""MACSTR"\",%d)\r\n", bss_link->authmode, ssid, bss_link->rssi,
            //        MAC2STR(bss_link->bssid), bss_link->channel);
			if(strcmp("Xiaomi001", ssid) == 0)
			{
				find_ap_status= 1;
            	printf(">>>>>>>  %s\n", ssid);
				return ;
			}
            bss_link = bss_link->next.stqe_next;
        }
    } else {
        printf("scan fail !!!\r\n");
    }
}

void scan_ap_init(void)
{
	wifi_set_opmode(STATIONAP_MODE);
	wifi_station_scan(NULL,scan_done);
	vTaskDelay(800);
	os_printf("now entering the scan AP test..... \n");
	printf("\n>>>>>>>>  RUN SCAN AP TEST.  %d \n", find_ap_status);
}

