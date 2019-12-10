#ifndef _XT_PCC_H
#define _XT_PCC_H

enum xt_pcc_flags {
	XT_PCC_INVERT  = 1 << 0,//00000001
	XT_SRC_ADDR    = 1 << 1,//00000010
	XT_DST_ADDR    = 1 << 2,//00000100
	XT_SRC_PORT    = 1 << 3,//00001000
	XT_DST_PORT    = 1 << 4,//00010000
	XT_PCC_MOD     = 1 << 5,//00100000
	XT_PCC_VALUE   = 1 << 6,//01000000
};

struct xt_pcc_mtinfo {
	u_int8_t flags;
	u_int32_t mod, value;
};

#endif
