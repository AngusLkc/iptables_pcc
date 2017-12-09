/*
 *	"pcc" match extension for iptables
 *	2015 Tiago Marques <tmarques@viaconnect.com.br>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License; either
 *	version 2 of the License, or any later version, as published by the
 *	Free Software Foundation.
 */
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtables.h>
#include "xt_pcc.h"
#include "compat_user.h"

static const struct option pcc_mt_opts[] = {
	{.name = "pcc-src",   .has_arg = false, .val = 's'},
	{.name = "pcc-dst",   .has_arg = false, .val = 'd'},
	{.name = "pcc-mod",   .has_arg = true,  .val = 'm'},
	{.name = "pcc-value", .has_arg = true,  .val = 'v'},
	{NULL},
};

static void pcc_mt_help(void)
{
	printf(
	"pcc match options:\n"
	"    --pcc-src           Include IPv4/IPv6 source address to hash calculation\n"
	"    --pcc-dst           Include IPv4/IPv6 destination address to hash calculation\n"
	"    --pcc-mod modulus   Modulus for hash calculation (to limit the range of possible values)\n"
	"[!] --pcc-value value   Value to compare\n"
	);
}

static int
pcc_mt_parse(int c, char **argv, int invert, unsigned int *flags,
	        const void *entry, struct xt_entry_match **match)
{
	struct xt_pcc_mtinfo *info = (void *)(*match)->data;
	char *endptr;

	switch (c) {
	case 's':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--pcc-src", *flags & XT_PCC_SRC);
		*flags      |= XT_PCC_SRC;
		info->flags |= XT_PCC_SRC;
		return true;
	case 'd':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--pcc-dst", *flags & XT_PCC_DST);
		*flags      |= XT_PCC_DST;
		info->flags |= XT_PCC_DST;
		return true;
	case 'm':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--pcc-mod", *flags & XT_PCC_MOD);
		*flags      |= XT_PCC_MOD;
		info->flags |= XT_PCC_MOD;
		info->mod = strtoull(optarg, &endptr, 0);
		if (*endptr != '\0' || !info->mod)
			xtables_error(PARAMETER_PROBLEM, "pcc match: "
			           "invalid value for --pcc-mod");
		return true;
	case 'v':
		xtables_param_act(XTF_ONLY_ONCE, "pcc", "--pcc-value", *flags & XT_PCC_VALUE);
		*flags      |= XT_PCC_VALUE;
		info->flags |= XT_PCC_VALUE;
		if (invert)
			info->flags |= XT_PCC_INVERT;
		info->value = strtoul(optarg, &endptr, 0);
		if (*endptr != '\0')
			xtables_error(PARAMETER_PROBLEM, "pcc match: "
			           "invalid value for --pcc-value");
		return true;
	}
	return false;
}

static void
pcc_mt_save(const void *ip, const struct xt_entry_match *match)
{
	const struct xt_pcc_mtinfo *info = (void *)match->data;

	if (info->flags & XT_PCC_SRC)
		printf(" --pcc-src");
	if (info->flags & XT_PCC_DST)
		printf(" --pcc-dst");
	if (info->flags & XT_PCC_MOD)
		printf(" --pcc-mod %lu", (unsigned long)info->mod);
	if (info->flags & XT_PCC_INVERT)
		printf(" !");
	if (info->flags & XT_PCC_VALUE)
		printf(" --pcc-value %lu", (unsigned long)info->value);
}

static void pcc_mt_print(const void *ip, const struct xt_entry_match *match,
                            int numeric)
{
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

static __attribute__((constructor)) void pcc_mt_ldr(void)
{
	xtables_register_match(&pcc_mt_reg);
}
