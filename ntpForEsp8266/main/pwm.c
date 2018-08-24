#include "user_def.h"
#include "wifi_state_machine.h"

extern uint32_t g_wifi_stat;


//PIN SETTINGS FOR PWM
#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_0_OUT_IO_NUM 12
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO12

#define PWM_1_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_1_OUT_IO_NUM 13
#define PWM_1_OUT_IO_FUNC  FUNC_GPIO13

#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_MTMS_U
#define PWM_2_OUT_IO_NUM 14
#define PWM_2_OUT_IO_FUNC  FUNC_GPIO14

#define PWM_3_OUT_IO_MUX PERIPHS_IO_MUX_MTDO_U
#define PWM_3_OUT_IO_NUM 15
#define PWM_3_OUT_IO_FUNC  FUNC_GPIO15

#define PWM_4_OUT_IO_MUX PERIPHS_IO_MUX_GPIO4_U
#define PWM_4_OUT_IO_NUM 4
#define PWM_4_OUT_IO_FUNC  FUNC_GPIO4

#define PWM_NUM_CHANNEL_NUM    5  //number of PWM Channels

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void pwm_led_on(void)
{
    int i = 0, flag = 1;
    printf("SDK version:%s\n", system_get_sdk_version());
    printf("PWM TEST !!!!\r\n");
    uint32 io_info[][3] = {
            { PWM_0_OUT_IO_MUX, PWM_0_OUT_IO_FUNC, PWM_0_OUT_IO_NUM }, //Channel 0
            { PWM_1_OUT_IO_MUX, PWM_1_OUT_IO_FUNC, PWM_1_OUT_IO_NUM }, //Channel 1
            { PWM_2_OUT_IO_MUX, PWM_2_OUT_IO_FUNC, PWM_2_OUT_IO_NUM }, //Channel 2
            { PWM_3_OUT_IO_MUX, PWM_3_OUT_IO_FUNC, PWM_3_OUT_IO_NUM }, //Channel 3
            { PWM_4_OUT_IO_MUX, PWM_4_OUT_IO_FUNC, PWM_4_OUT_IO_NUM }, //Channel 4
    };

	u32 duty[5] = {512, 512, 512, 512, 512}; //Max duty cycle is 1023
	pwm_init(1000, duty ,PWM_NUM_CHANNEL_NUM,io_info);
#if 1
	printf("pwm connect stat:>>>>>>>>>  %d\n", wifi_station_get_connect_status());
	while(wifi_station_get_connect_status() == 5){
		flag? i++:i--;
		//pwm_set_duty(i, 0); //Set duty to specific channel
		//pwm_set_duty(i, 1);
		pwm_set_duty(i, 2);
		//pwm_set_duty(i, 3);
		//pwm_set_duty(i, 4);
		pwm_start();   //Call this: every time you change duty/period
		vTaskDelay(2); //400 milliSec Delay

		if (i > 50) 
			flag = 0;
		 else if(i <= 4){
			flag = 1;
			vTaskDelay(5); 
		}
	}
	pwm_set_duty(0, 2);
	pwm_start();   //Call this: every time you change duty/period
	printf("pwm wifi disconnected\n");
	system_restart();

	vTaskDelete(NULL);
#endif
} 

void pwm_led_off(void)
{
    int i = 0, flag = 1;
    printf("SDK version:%s\n", system_get_sdk_version());
    printf("PWM TEST !!!!\r\n");
    uint32 io_info[][3] = {
            { PWM_0_OUT_IO_MUX, PWM_0_OUT_IO_FUNC, PWM_0_OUT_IO_NUM }, //Channel 0
            { PWM_1_OUT_IO_MUX, PWM_1_OUT_IO_FUNC, PWM_1_OUT_IO_NUM }, //Channel 1
            { PWM_2_OUT_IO_MUX, PWM_2_OUT_IO_FUNC, PWM_2_OUT_IO_NUM }, //Channel 2
            { PWM_3_OUT_IO_MUX, PWM_3_OUT_IO_FUNC, PWM_3_OUT_IO_NUM }, //Channel 3
            { PWM_4_OUT_IO_MUX, PWM_4_OUT_IO_FUNC, PWM_4_OUT_IO_NUM }, //Channel 4
    };

	u32 duty[5] = {512, 512, 512, 512, 512}; //Max duty cycle is 1023
	pwm_init(1000, duty ,PWM_NUM_CHANNEL_NUM,io_info);
#if 1
	do{
		//pwm_set_duty(i, 0); //Set duty to specific channel
		//pwm_set_duty(i, 1);
		pwm_set_duty(0, 2);
		//pwm_set_duty(i, 3);
		//pwm_set_duty(i, 4);
		pwm_start();   //Call this: every time you change duty/period
		vTaskDelay(2); //400 milliSec Delay
	}while(0);
	vTaskDelete(NULL);
#endif
} 

 
