#ifndef USER_DEF_H
#define USER_DEF_H

#include "esp_common.h"
#include "lwip/mem.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/i2c_master.h"
#include "time.h"

#include "ntp_server.h"

extern void ntp_client_begin(void * para);
extern void ntp_server_begin(void * para);
extern void DS3231_getTime();
extern time_t DS3231_Get_Time_Base1970();
extern time_t DS3231_Get_Time_Base1900();

void drv_print_var(uint8_t *str, uint8_t *addr, uint8_t size);
void drv_print_buff(uint8_t *str, uint8_t *buf, uint32_t len);

void wifi_init();


#endif
