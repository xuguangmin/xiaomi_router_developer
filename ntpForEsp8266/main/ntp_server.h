#ifndef NTP_PROTO_H_
#define NTP_PROTO_H__

#include "esp_common.h"

#define NTP_SERVER_PORT (123) 

#define LI_00	0	/* 无预告 */
#define LI_01	1	/* 最近一分钟有61秒 */
#define LI_10	2	/* 最近一分钟有59秒 */
#define LI_11	3	/* 警告状态 */
#define VERSION		4	/* 版本号 */
#define MODE0	0	/* 保留 */	
#define MODE1	1	/* 对称性激活 */	
#define MODE2	2	/* 被动的对称性 */	
#define MODE3	3	/* 客户端 */	
#define MODE4	4	/* 服务端 */	
#define MODE5	5	/* 广播 */	
#define MODE6	6	/* 为NTP控制性系保留 */	
#define MODE7	7	/* 为自用保留 */	

/* 
 *  NTP的时间从 1900 1月1日 开始计时
 *
 *  3600s*24h*(365days*70years+17days)从1900年1月1日至1970年1月1日的秒数
*/

enum {
	__LITTLE_ENDIAN,
	__BIG_ENDIAN,
};
#define __BYTE_ORDER 	(__LITTLE_ENDIAN)

#define FROM1900TO1970 (0x83aa7e80U)

/* 因为我们在东8区,时间戳比标准时间多了８小时 */
#define SECONDS_8_HOUS	(8*60*60)

typedef struct msg_st{
	//uint8_t LI_VN_Mode;	/* LI 2bit; VM 3bit; Mode 3bit*/
#if __BYTE_ORDER == __LITTLE_ENDIAN
	uint8_t mode:3;
	uint8_t version:3;
	uint8_t li:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
	uint8_t li:2;
	uint8_t version:3;
	uint8_t mode:3;
#endif

	uint8_t stratum;
	uint8_t poll;
	uint8_t precision;

	uint32_t rootdelay;	/* roundtrip delay to primary source */
	uint32_t rootdispersion;	/* dispersion to primary source */
	uint32_t reference_identifier;		/* remote reference ID */

	uint32_t reference_timestamp[2];	/* 参考时间 */
	uint32_t orignate_timestamp[2];	/* 客户端发送时间 */
	uint32_t receive_timestamp[2];		/* 服务端接收时间 */
	uint32_t transmit_timestamp[2];	/* 服务端发送时间 */
}NTP __attribute__((aligned(8)));



#endif





