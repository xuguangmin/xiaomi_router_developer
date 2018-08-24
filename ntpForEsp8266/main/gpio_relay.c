#include "user_def.h"

/**********************************SAMPLE CODE*****************************/
#define ETS_GPIO_INTR_ENABLE()  _xt_isr_unmask(1 << ETS_GPIO_INUM)  //ENABLE INTERRUPTS
#define ETS_GPIO_INTR_DISABLE() _xt_isr_mask(1 << ETS_GPIO_INUM)    //DISABLE INTERRUPTS

//GPIO OUTPUT SETTINGS
#define  LED_IO_MUX  PERIPHS_IO_MUX_MTDO_U
#define  LED_IO_NUM  5
#define  LED_IO_FUNC FUNC_GPIO5
#define  LED_IO_PIN  GPIO_Pin_5

//GPIO INPUT SETTINGS
#define  BUTTON_IO_MUC  PERIPHS_IO_MUX_MTCK_U
#define  BUTTON_IO_NUM   13
#define  BUTTON_IO_FUNC  FUNC_GPIO13
#define  BUTTON_IO_PIN   GPIO_Pin_13

void io_intr_handler(void)
{
    uint32 status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);          //READ STATUS OF INTERRUPT
    static uint8 val = 0;
    if (status & BUTTON_IO_PIN) {
        if (val == 0) {
            GPIO_OUTPUT_SET(LED_IO_NUM, 1);
            //gpio16_output_set(0);
            val = 1;
        } else {
            GPIO_OUTPUT_SET(LED_IO_NUM, 0);
            //gpio16_output_set(1);
            val = 0;
        }
    }

    //should not add print in interruption, except that we want to debug something
    //printf("in io intr: 0X%08x\r\n",status);                    //WRITE ON SERIAL UART0
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, status);       //CLEAR THE STATUS IN THE W1 INTERRUPT REGISTER

}

void gpio_task(void)
{
    printf("TEST TOGGLE ON GPIO15,YOU WILL SEE THE LED BLINKING ON IO15\n");
    GPIO_ConfigTypeDef io_out_conf;
    io_out_conf.GPIO_IntrType = GPIO_PIN_INTR_DISABLE;
    io_out_conf.GPIO_Mode = GPIO_Mode_Output;
    io_out_conf.GPIO_Pin = LED_IO_PIN;
    io_out_conf.GPIO_Pullup = GPIO_PullUp_DIS;
    gpio_config(&io_out_conf);

    GPIO_OUTPUT_SET(LED_IO_NUM, 0);
    gpio16_output_conf();


	//uint32_t delay = MINUTE;
	uint32_t delay = 120*MINUTE;
	while (1) {
		//gpio16_output_set(0);
		GPIO_OUTPUT_SET(LED_IO_NUM, 1);
		vTaskDelay(delay);
		//gpio16_output_set(1);
		GPIO_OUTPUT_SET(LED_IO_NUM, 0);

		system_restart();
		vTaskDelete(NULL);
	}

    printf("SETUP GPIO13 INTERRUPT CONFIGURE..\r\n");
    GPIO_ConfigTypeDef io_in_conf;
    io_in_conf.GPIO_IntrType = GPIO_PIN_INTR_NEGEDGE;
    io_in_conf.GPIO_Mode = GPIO_Mode_Input;
    io_in_conf.GPIO_Pin = BUTTON_IO_PIN;
    io_in_conf.GPIO_Pullup = GPIO_PullUp_EN;
    gpio_config(&io_in_conf);

    gpio_intr_handler_register(io_intr_handler, NULL);
    gpio16_output_set(1);
    ETS_GPIO_INTR_ENABLE();
}

