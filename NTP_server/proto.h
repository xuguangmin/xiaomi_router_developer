#ifndef PROTO_H_
#define PROTO_H__

#define LI_00	0	/* 无预告 */
#define LI_01	1	/* 最近一分钟有61秒 */
#define LI_10	2	/* 最近一分钟有59秒 */
#define LI_11	3	/* 警告状态 */
#define VN	4	/* 版本号 */
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
#define FROM1900TO1970 (0x83aa7e80U)

/* 因为我们在东8区,时间戳比标准时间多了８小时 */
#define SECONDS_8_HOUS	(8*60*60)

struct msg_st
{
	int8_t LI_VN_Mode;
	int8_t stratum;
	int8_t poll;
	int8_t precision;

	int32_t rootdelay;	/* roundtrip delay to primary source */
	int32_t rootdisp;	/* dispersion to primary source */
	int32_t refid;		/* remote reference ID */

	//l_fp	reftime;	/* update epoch */
	//l_fp	origtime;
	//l_fp	recetime;
	//l_fp	transtime;

	//uint32_t reftime_hi;
	//uint32_t reftime_lo;
	uint64_t reftime;

	//uint32_t origtime_hi;
	//uint32_t origtime_lo;
	uint64_t origtime;

	//uint32_t recetime_hi;
	//uint32_t recetime_lo;
	uint64_t recetime;

	//uint32_t transtime_hi;
	//uint32_t transtime_lo;
	uint64_t transtime;
}__attribute__((packed));



#endif





