/*
 *      Per Connection Classifier (pcc) match extension for Xtables
 *      Copyright (C) Tiago Marques <tmarques@viaconnect.com.br>, 2015
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License; either
 *      version 2 of the License, or any later version, as published by the
 *      Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter/x_tables.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include "xt_pcc.h"
#include "compat_xtables.h"

MODULE_AUTHOR("Tiago Marques <tmarques@viaconnect.com.br>");
MODULE_DESCRIPTION("Xtables: Per Connection Classifier match");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_pcc");
MODULE_ALIAS("ip6t_pcc");

static bool
pcc_mt_v4(const struct sk_buff *skb, struct xt_action_param *par)
{
	struct xt_pcc_mtinfo *info = (void *)par->matchinfo;
	const struct iphdr *iph = ip_hdr(skb);
	u_int32_t hash = 0;

	if (info->flags & XT_PCC_SRC)
		hash ^= ntohl(iph->saddr);
	if (info->flags & XT_PCC_DST)
		hash ^= ntohl(iph->daddr);

	//printk(KERN_INFO KBUILD_MODNAME ": S(%lu)=" NIPQUAD_FMT " D(%lu)=" NIPQUAD_FMT ""
	//	" %lu %% %lu = %lu %s= %lu Flag(I=%d S=%d D=%d) RET=%d\n",
	//	(long unsigned)ntohl(iph->saddr), NIPQUAD(iph->saddr),
	//	(long unsigned)ntohl(iph->daddr), NIPQUAD(iph->daddr),
	//	(long unsigned)hash, (long unsigned)info->mod, (long unsigned)(hash % info->mod),
	//	(info->flags & XT_PCC_INVERT) ? "!" : "=", (long unsigned)info->value,
	//	(int)(info->flags & XT_PCC_INVERT), (int)(info->flags & XT_PCC_SRC), (int)(info->flags & XT_PCC_DST),
	//	(int)(((hash % info->mod) == info->value) ^ !!(info->flags & XT_PCC_INVERT)));

	return ((hash % info->mod) == info->value) ^
		!!(info->flags & XT_PCC_INVERT);
}

static bool
pcc_mt_v6(const struct sk_buff *skb, struct xt_action_param *par)
{
	struct xt_pcc_mtinfo *info = (void *)par->matchinfo;
	const struct ipv6hdr *iph = ipv6_hdr(skb);
	u_int32_t hash = 0;
	int i;

	if (info->flags & XT_PCC_SRC)
		for(i = 0; i < 4; i++)
			hash ^= ntohl(iph->saddr.s6_addr32[i]);
	if (info->flags & XT_PCC_DST)
		for(i = 0; i < 4; i++)
			hash ^= ntohl(iph->daddr.s6_addr32[i]);

	return ((hash % info->mod) == info->value) ^
		!!(info->flags & XT_PCC_INVERT);
}

static int pcc_mt_check(const struct xt_mtchk_param *par)
{
	const struct xt_pcc_mtinfo *info = par->matchinfo;

	if (!info->mod) {
		printk(KERN_INFO KBUILD_MODNAME ": --pcc-mod can't be zero\n");
		return -EINVAL;
	}

	if (info->value >= info->mod) {
		printk(KERN_INFO KBUILD_MODNAME ": --pcc-value must be less than --pcc-mod\n");
		return -EINVAL;
	}

	if (!(info->flags & (XT_PCC_SRC | XT_PCC_DST))) {
		printk(KERN_INFO KBUILD_MODNAME ": You must specify one of --pcc-src, --pcc-dst or both.\n");
		return -EINVAL;
	}

	return 0;
}

static struct xt_match pcc_mt_reg[] __read_mostly = {
	{
		.name       = "pcc",
		.revision   = 1,
		.family     = NFPROTO_IPV4,
		.match      = pcc_mt_v4,
		.checkentry = pcc_mt_check,
		.matchsize  = sizeof(struct xt_pcc_mtinfo),
		.me         = THIS_MODULE,
	},
	{
		.name       = "pcc",
		.revision   = 1,
		.family     = NFPROTO_IPV6,
		.match      = pcc_mt_v6,
		.checkentry = pcc_mt_check,
		.matchsize  = sizeof(struct xt_pcc_mtinfo),
		.me         = THIS_MODULE,
	},
};

static int __init pcc_mt_init(void)
{
	return xt_register_matches(pcc_mt_reg, ARRAY_SIZE(pcc_mt_reg));
}

static void __exit pcc_mt_exit(void)
{
	xt_unregister_matches(pcc_mt_reg, ARRAY_SIZE(pcc_mt_reg));
}

module_init(pcc_mt_init);
module_exit(pcc_mt_exit);
