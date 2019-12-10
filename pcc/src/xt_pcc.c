#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/netfilter/x_tables.h>

#include "xt_pcc.h"

MODULE_AUTHOR("SZLZNET <admin@szlznet.com>");
MODULE_DESCRIPTION("Xtables: Per Connection Classifier match");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_pcc");

static bool pcc_mt_v4(const struct sk_buff *skb, struct xt_action_param *par){
	const struct xt_pcc_mtinfo *info = par->matchinfo;
	const struct iphdr *iph = ip_hdr(skb);
	const struct tcphdr *tcph = NULL;
	const struct udphdr *udph = NULL;
	struct tcphdr tcpbuf;
	struct udphdr udpbuf;
	u_int32_t hash = 0;
	if (info->flags & XT_SRC_ADDR)
		hash ^= ntohl(iph->saddr);
	if (info->flags & XT_DST_ADDR)
		hash ^= ntohl(iph->daddr);
	if (iph->protocol == 6){
		tcph=skb_header_pointer(skb,skb_network_offset(skb)+(iph->ihl<<2),sizeof(tcpbuf),&tcpbuf);
		if (info->flags & XT_SRC_PORT)
			hash ^= ntohs(tcph->source);
		if (info->flags & XT_DST_PORT)
			hash ^= ntohs(tcph->dest);
	} else if (iph->protocol == 17){
		udph=skb_header_pointer(skb,skb_network_offset(skb)+(iph->ihl<<2),sizeof(udpbuf),&udpbuf);
		if (info->flags & XT_SRC_PORT)
			hash ^= ntohs(udph->source);
		if (info->flags & XT_DST_PORT)
			hash ^= ntohs(udph->dest);
	}
	return ((hash%info->mod)==info->value) ^ !!(info->flags & XT_PCC_INVERT);
}

static int pcc_mt_check(const struct xt_mtchk_param *par){
	const struct xt_pcc_mtinfo *info = par->matchinfo;
	if (!info->mod) {
		printk(KERN_INFO KBUILD_MODNAME ": --pcc-mod can't be zero\n");
		return -EINVAL;
	}
	if (info->value >= info->mod || !info->value || info->value < 0) {
		printk(KERN_INFO KBUILD_MODNAME ": --pcc-value must be less than --pcc-mod\n");
		return -EINVAL;
	}
	if (!(info->flags & (XT_SRC_ADDR | XT_DST_ADDR | XT_SRC_PORT | XT_DST_PORT))) {
		printk(KERN_INFO KBUILD_MODNAME ": You must specify one of --src-addr, --dst-addr, --src-port, --dst-port or both.\n");
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
};

static int __init pcc_mt_init(void){
	return xt_register_matches(pcc_mt_reg, ARRAY_SIZE(pcc_mt_reg));
}

static void __exit pcc_mt_exit(void){
	xt_unregister_matches(pcc_mt_reg, ARRAY_SIZE(pcc_mt_reg));
}

module_init(pcc_mt_init);
module_exit(pcc_mt_exit);
