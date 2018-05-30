
#ifndef DS3231_h
#define DS3231_h


#define DS3231_WriteAddress  0xD0
#define DS3231_ReadAddress   0xD1

#define DS3231_SECOND       0x00 
#define DS3231_MINUTE       0x01
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年
#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器

#define TIME_24H	0
#define TIME_12H	1
#define TIME_PM (1)
#define TIME_AM (0)


#endif
