#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtables.h>

#include "xt_pcc.h"

static const struct option pcc_mt_opts[] = {
	{.name = "src-addr",   .has_arg = false, .val = 's'},
	{.name = "dst-addr",   .has_arg = false, .val = 'd'},
	{.name = "src-port",   .has_arg = false, .val = 'x'},
	{.name = "dst-port",   .has_arg = false, .val = 'y'},
	{.name = "pcc-mod",    .has_arg = true,  .val = 'm'},
	{.name = "pcc-value",  .has_arg = true,  .val = 'v'},
	{NULL},
};

static void pcc_mt_help(void){
	printf(
	"pcc match options:\n"
	"    --src-addr           Include IPv4/IPv6 source address to hash calculation\n"
	"    --dst-addr           Include IPv4/IPv6 destination address to hash calculation\n"
	"    --src-port           Include IPv4/IPv6 source port to hash calculation\n"
	"    --dst-port           Include IPv4/IPv6 destination port to hash calculation\n"
	"    --pcc-mod modulus    Modulus for hash calculation (to limit the range of possible values)\n"
	"[!] --pcc-value value    Value to compare\n"
	);
}

static int pcc_mt_parse(int c, char **argv, int invert, unsigned int *flags, const void *entry, struct xt_entry_match **match){
	struct xt_pcc_mtinfo *info = (void *)(*match)->data;
	char *endptr;
	switch (c) {
	case 's':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--src-addr", *flags & XT_SRC_ADDR);
		*flags      |= XT_SRC_ADDR;
		info->flags |= XT_SRC_ADDR;
		return true;
	case 'd':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--dst-addr", *flags & XT_DST_ADDR);
		*flags      |= XT_DST_ADDR;
		info->flags |= XT_DST_ADDR;
		return true;
	case 'x':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--src-port", *flags & XT_SRC_PORT);
		*flags      |= XT_SRC_PORT;
		info->flags |= XT_SRC_PORT;
		return true;
	case 'y':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--dst-port", *flags & XT_DST_PORT);
		*flags      |= XT_DST_PORT;
		info->flags |= XT_DST_PORT;
		return true;
	case 'm':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--pcc-mod", *flags & XT_PCC_MOD);
		*flags      |= XT_PCC_MOD;
		info->flags |= XT_PCC_MOD;
		info->mod = strtoull(optarg, &endptr, 0);
		if (*endptr != '\0' || !info->mod)
			xtables_error(PARAMETER_PROBLEM, "pcc match: invalid value for --pcc-mod");
		return true;
	case 'v':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--pcc-value", *flags & XT_PCC_VALUE);
		*flags      |= XT_PCC_VALUE;
		info->flags |= XT_PCC_VALUE;
		if (invert)
			info->flags |= XT_PCC_INVERT;
		info->value = strtoul(optarg, &endptr, 0);
		if (*endptr != '\0' || info->value < 0)
			xtables_error(PARAMETER_PROBLEM, "pcc match: invalid value for --pcc-value");
		return true;
	}
	return false;
}

static void pcc_mt_save(const void *ip, const struct xt_entry_match *match){
	const struct xt_pcc_mtinfo *info = (void *)match->data;
	if (info->flags & XT_SRC_ADDR)
		printf(" --src-addr");
	if (info->flags & XT_DST_ADDR)
		printf(" --dst-addr");
	if (info->flags & XT_SRC_PORT)
		printf(" --src-port");
	if (info->flags & XT_DST_PORT)
		printf(" --dst-port");
	if (info->flags & XT_PCC_MOD)
		printf(" --pcc-mod %lu", (unsigned long)info->mod);
	if (info->flags & XT_PCC_INVERT)
		printf(" !");
	if (info->flags & XT_PCC_VALUE)
		printf(" --pcc-value %lu", (unsigned long)info->value);
}

static void pcc_mt_print(const void *ip, const struct xt_entry_match *match, int numeric){
	printf(" -m pcc");
	pcc_mt_save(ip, match);
}

static struct xtables_match pcc_mt_reg = {
	.family        = NFPROTO_UNSPEC,
	.revision      = 1,
	.name          = "pcc",
	.version       = XTABLES_VERSION,
	.size          = XT_ALIGN(sizeof (struct xt_pcc_mtinfo)),
	.userspacesize = XT_ALIGN(sizeof (struct xt_pcc_mtinfo)),
	.help          = pcc_mt_help,
	.parse         = pcc_mt_parse,
	.print         = pcc_mt_print,
	.save          = pcc_mt_save,
	.extra_opts    = pcc_mt_opts,
};

static __attribute__((constructor)) void pcc_mt_ldr(void){
	xtables_register_match(&pcc_mt_reg);
}
