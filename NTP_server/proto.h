#ifndef PROTO_H_
#define PROTO_H__

/* 
 *  NTP的时间从 1900 1月1日 开始计时
 *
 *  3600s*24h*(365days*70years+17days)从1900年1月1日至1970年1月1日的秒数
*/
#define FROM1900TO1970 (0x83aa7e80U)

typedef struct {
	uint32_t ui;
	uint32_t uf;
} l_fp;

struct msg_st
{
	int8_t	version;
	int8_t leap;
	int8_t stratum;
	int8_t precision;

	int32_t rootdelay;	/* roundtrip delay to primary source */
	int32_t rootdisp;	/* dispersion to primary source */
	int32_t refid;		/* remote reference ID */

	//l_fp	reftime;	/* update epoch */
	//l_fp	origtime;
	//l_fp	recetime;
	//l_fp	transtime;

	uint32_t reftime_hi;
	uint32_t reftime_lo;

	uint32_t origtime_hi;
	uint32_t origtime_lo;

	uint32_t recetime_hi;
	uint32_t recetime_lo;

	uint32_t transtime_hi;
	uint32_t transtime_lo;
}__attribute__((packed));




#endif





