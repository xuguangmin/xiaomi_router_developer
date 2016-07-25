#ifndef PROTO_H_
#define PROTO_H__

#define RCVPORT		"1989"

#define NAMESIZE 	13

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

	l_fp	reftime;	/* update epoch */
	l_fp	origtime;
	l_fp	recetime;
	//l_fp	transtime;

	//uint64_t reftime;
	//uint64_t origtime;
	//uint64_t recetime;
	int64_t transtime;
}__attribute__((packed));




#endif





