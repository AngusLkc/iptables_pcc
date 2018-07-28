#ifndef _XT_PCC_H
#define _XT_PCC_H

enum xt_pcc_flags {
	XT_PCC_INVERT  = 1 << 0,
	XT_SRC_ADDR    = 1 << 1,
	XT_DST_ADDR    = 1 << 2,
	XT_SRC_PORT    = 1 << 3,
	XT_DST_PORT    = 1 << 4,
	XT_PCC_MOD     = 1 << 5,
	XT_PCC_VALUE   = 1 << 6,
};

struct xt_pcc_mtinfo {
	u_int8_t flags;
	u_int32_t mod, value;
};

#endif
