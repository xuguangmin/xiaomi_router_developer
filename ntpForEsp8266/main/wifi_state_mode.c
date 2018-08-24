#include "user_def.h"
#include "driver/gpio.h"

#define STATION_MODE_WIFI_SSID     "Xiaomi001"
#define STATION_MODE_WIFI_PASSWORD  "xuguangmin10"

//void ntp_begin(void *pvParameters);

LOCAL xTaskHandle ntp_client_task_handle = 0;
LOCAL xTaskHandle ntp_server_task_handle = 0;
LOCAL xTaskHandle pwm_led_task_handle = 0;
LOCAL xTaskHandle restart_task_handle = 0;


void dete_restart()
{
	while(1)
	{
		vTaskDelay(MINUTE);
		if(wifi_station_get_connect_status() == 5) {
			printf("restart task, wifi stat is reconnect!!!\n");
			system_restart();
		}
	}
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
			os_printf("ntp got ip , creat NTP Server task\n");
			//if (ntp_client_task_handle == 0) 
				xTaskCreate(ntp_client_begin, "ntp_client_task", 1024, NULL, 1, &ntp_client_task_handle);
			vTaskDelay(100);
			//if(ntp_server_task_handle == 0)
				xTaskCreate(ntp_server_begin, "ntp_server_task", 1024, NULL, 1, &ntp_server_task_handle);
			//if (pwm_led_task_handle == 0)
				xTaskCreate(pwm_led_on, "pwm_led", 1024, NULL, 1, &pwm_led_task_handle);

			//xTaskCreate(dete_restart, "restart", 1024, NULL, 1, &restart_task_handle);

			break;
		case EVENT_STAMODE_SCAN_DONE:        
			os_printf("ntp scan done\n");
			break;
		case EVENT_STAMODE_CONNECTED:            
			os_printf("ntp connected\n");
			break;
		case EVENT_STAMODE_DISCONNECTED:         
			os_printf("ntp disconnected\n");
			vTaskDelete(ntp_client_task_handle); 
			vTaskDelete(ntp_server_task_handle); 
			//vTaskDelete(pwm_led_task_handle); 
			//system_restart();
			
			ntp_client_task_handle = 0;
			ntp_server_task_handle = 0;
			pwm_led_task_handle = 0;

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

