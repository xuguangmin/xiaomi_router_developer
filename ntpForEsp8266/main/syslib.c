#include "user_def.h"


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

void drv_print_var(uint8_t *str, uint8_t *addr, uint8_t size)
{
	uint32_t i = 0;

	printf("[%s] ", str);
	for(i=0; i<size; i++) {
		printf("%02X ", addr[i]);
	}
	printf("\n");
}


double Ntp_Get_rrt(NTP ntpPkg, time_t timeVal)
{

}

double Ntp_Get_Offset(NTP ntpPkg, time_t timeVal)
{

}
