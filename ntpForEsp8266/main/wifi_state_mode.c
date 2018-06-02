#include "user_def.h"

#define STATION_MODE_WIFI_SSID     "Xiaomi001"
#define STATION_MODE_WIFI_PASSWORD  "xuguangmin10"

//void ntp_begin(void *pvParameters);

LOCAL xTaskHandle *ntp_task_handle = NULL;
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
			os_printf("ntp got ip , creat NTP Server task\n");
			if (ntp_task_handle == NULL) {
				xTaskCreate(ntp_server_begin, "ntp_server_task", 1024, NULL, 1, ntp_task_handle);
				//xTaskCreate(ntp_client_begin, "ntp_client_task", 1024, NULL, 1, ntp_task_handle);
			}
			break;
		case EVENT_STAMODE_SCAN_DONE:        
			os_printf("ntp scan done\n");
			break;
		case EVENT_STAMODE_CONNECTED:            
			os_printf("ntp connected\n");
			break;
		case EVENT_STAMODE_DISCONNECTED:         
			os_printf("ntp disconnected\n");
			system_restart();
			break;
		case EVENT_STAMODE_AUTHMODE_CHANGE:      
			os_printf("ntp authmode change\n");
			break;
		case EVENT_STAMODE_DHCP_TIMEOUT:         
			os_printf("ntp dhcp timeout\n");
			break;
		default:
			break;
	}
}

void wifi_init() {
    wifi_set_opmode(STATION_MODE);
    {
        struct station_config config;
        bzero(&config, sizeof(struct station_config));
        sprintf(config.ssid, STATION_MODE_WIFI_SSID);
        sprintf(config.password, STATION_MODE_WIFI_PASSWORD);
        wifi_station_set_config(&config);
        wifi_station_connect();
    }
	wifi_set_event_handler_cb(ntp_event_cb);
}

