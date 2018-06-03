#include "esp_common.h"
#include "gpio.h"
#include "driver/i2c_master.h"
#include "time.h"
#include "ds3231.h"
#include "ntp_server.h"


uint8_t BCD2HEX(uint8_t val){ 	return ((((val/16) - ((val/16)%1)) *10) + (val%16));}
uint8_t DEC2BCD(uint8_t val){ 	return ((((val/10) - ((val/10)%1)) *16) + (val%10));}

uint8_t read_random(uint8_t random_addr);

void DS3231_Write_Byte(uint8_t addr, uint8_t val) {
	i2c_master_start();
    i2c_master_writeByte(DS3231_WriteAddress);
	i2c_master_getAck();

    i2c_master_writeByte(addr);
	i2c_master_getAck();

	i2c_master_writeByte(val);	
	i2c_master_getAck();
    i2c_master_stop();
}

void DS3231_setClockMode(uint8_t h12) {
	// sets the mode to 12-hour (true) or 24-hour (false).
	// One thing that bothers me about how I've written this is that
	// if the read and right happen at the right hourly millisecnd,
	// the clock will be set back an hour. Not sure how to do it better, 
	// though, and as long as one doesn't set the mode frequently it's
	// a very minimal risk. 
	// It's zero risk if you call this BEFORE setting the hour, since
	// the setHour() function doesn't change this mode.
	
	uint8_t temp_buffer;

	// Start by reading byte 0x02.
	temp_buffer = read_random(DS3231_HOUR);

	// Set the flag to the requested value:
	if (h12) {
		temp_buffer = temp_buffer | 0b01000000;
	} else {
		temp_buffer = temp_buffer & 0b10111111;
	}

	// Write the byte
	DS3231_Write_Byte(DS3231_HOUR, temp_buffer);
}


void DS3231_SetHour(uint8_t hour) {
	// Sets the hour, without changing 12/24h mode.
	// The hour must be in 24h format.

	bool h12;

	// Start by figuring out what the 12/24 mode is
	h12 = read_random(DS3231_HOUR) & 0b01000000;
	// if h12 is true, it's 12h mode; false is 24h.
	
	if (h12) {
		// 12 hour
		if (hour > TIME_12H) {
			hour = DEC2BCD(hour-TIME_12H) | 0b01100000;
		} else {
			hour = DEC2BCD(hour) & 0b11011111;
		}
	} else {
		// 24 hour
		hour = DEC2BCD(hour) & 0b10111111;
	}

	DS3231_Write_Byte(DS3231_HOUR, hour);
}

//struct tm *gmtime(const time_t *timep);
void DS3231_Set(const time_t *times) {
	struct tm *timep;

	timep = gmtime(times);
#if 1
	printf("Set DS3231 ====> %d-%02d-%02d   %02d:%02d:%02d  week:%d\n",
			timep->tm_year - 100 + 2000,
			timep->tm_mon?timep->tm_mon+1 : 12,
			timep->tm_mday,
			timep->tm_hour,
			timep->tm_min,
			timep->tm_sec,
			timep->tm_wday? timep->tm_wday : 7);
#endif
	//Sets the minutes 
	DS3231_Write_Byte(DS3231_CONTROL, 0x1C);
	DS3231_Write_Byte(DS3231_STATUS, 0x00);

	DS3231_setClockMode(TIME_24H);

	DS3231_Write_Byte(DS3231_YEAR, DEC2BCD(timep->tm_year-100));
	DS3231_Write_Byte(DS3231_MONTH, 
			DEC2BCD(timep->tm_mon? timep->tm_mon+1 : 12));
	DS3231_Write_Byte(DS3231_DAY, DEC2BCD(timep->tm_mday));

	DS3231_Write_Byte(DS3231_WEEK, 
			DEC2BCD(timep->tm_mon? timep->tm_wday+1:7));

	DS3231_SetHour(timep->tm_hour);
	DS3231_Write_Byte(DS3231_MINUTE, DEC2BCD(timep->tm_min));
	DS3231_Write_Byte(DS3231_SECOND, DEC2BCD(timep->tm_sec));
}

uint8_t read_current()
{
	//uint8_t ack;
	uint8_t read_data;

    i2c_master_start();
    i2c_master_writeByte(DS3231_ReadAddress);
	i2c_master_getAck();

    read_data = i2c_master_readByte();
	//i2c_master_setAck(0);
    i2c_master_stop();
	return read_data;
}

uint8_t read_random(uint8_t random_addr)
{
	//uint8_t ack;

    i2c_master_start();
    i2c_master_writeByte(DS3231_WriteAddress);
	i2c_master_getAck();
    //i2c_master_stop();

    //i2c_master_start();
    i2c_master_writeByte(random_addr);
	i2c_master_getAck();
    //i2c_master_stop();

	return(read_current());
}

uint8_t DS3231_getSecond()	{return BCD2HEX(read_random(DS3231_SECOND));}
uint8_t DS3231_getMinute()	{return BCD2HEX(read_random(DS3231_MINUTE));}
uint8_t DS3231_getDate()	{return BCD2HEX(read_random(DS3231_DAY));}
uint8_t DS3231_getWeek()	{return BCD2HEX(read_random(DS3231_WEEK));}
uint8_t DS3231_getYear()	{return BCD2HEX(read_random(DS3231_YEAR));}

uint8_t DS3231_getMonth()	{
	uint8_t month;

	month = read_random(DS3231_MONTH);
	//uint8_t century = month & 0b10000000;
	return month = BCD2HEX(month & 0b01111111);
}

uint8_t DS3231_getHour(uint8_t * mode)	{
	uint8_t h12, PM, hour;

	hour = read_random(DS3231_HOUR);
	h12 = hour & 0b01000000;

	if (h12) {
		PM = hour & 0b00100000;
		hour = BCD2HEX(hour & 0b00011111);
		if(mode != NULL) *mode = TIME_12H;
	} else {
		hour = BCD2HEX(hour & 0b00111111);
	    if(mode != NULL) *mode = TIME_24H;
	}

	return hour;
}

void DS3231_getTime()
{
    i2c_master_gpio_init();
    i2c_master_start();

    //7 bit Address + LSB=0 for write command
    i2c_master_writeByte(DS3231_WriteAddress);
    i2c_master_writeByte(0x00);
    i2c_master_stop();

	//set(18, 5, 30, 13, 02, 20, 2);
	uint8_t year, month, date, hour, min, sec, week, mode;
	year  = DS3231_getYear();

	month = DS3231_getMonth();

	//date
	date = DS3231_getDate();
	week = DS3231_getWeek();

	hour = DS3231_getHour(&mode);
	min  = DS3231_getMinute(); 
	sec  = DS3231_getSecond(); 

	printf("20%d-%02d-%02d %02d:%02d:%02d/%dh  week:%02d\n", year, month, date, hour, min, sec, mode?12:24, week);

	//vTaskDelay(100); // 1s
}

time_t DS3231_Get_Time_Base1970()
{
	time_t t;
	struct tm tmVal;

    i2c_master_gpio_init();
    i2c_master_start();

    //7 bit Address + LSB=0 for write command
    i2c_master_writeByte(DS3231_WriteAddress);
    i2c_master_writeByte(0x00);
    i2c_master_stop();

	tmVal.tm_sec = DS3231_getSecond();    
	tmVal.tm_min = DS3231_getMinute();    
	tmVal.tm_hour = DS3231_getHour(NULL);   
	tmVal.tm_mday = DS3231_getDate();  
	tmVal.tm_mon  = DS3231_getMonth() - 1;    
	tmVal.tm_year = 2000 + DS3231_getYear() -1900;   
	tmVal.tm_wday = DS3231_getWeek() - 1; 
	//tmVal.tm_yday;
	//tmVal.tm_isdst;  
    
	t = mktime(&tmVal);
#if 0
	drv_print_var("timestamp from DS3231", &t, sizeof(t));
	rintf("time stamp:%d\n", mktime(&tmVal));
#endif
	return  t;
}
time_t DS3231_Get_Time_Base1900()
{
	return DS3231_Get_Time_Base1970() + FROM1900TO1970 - SECONDS_8_HOUS;
}


