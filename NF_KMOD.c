/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Coompiled on Artix 20210101-x86_64
 * Init system - runit
 * Kernel version - 5.10.90-1-lts
 * Date : 22 Feb 2021
 * Author : Ritik Malik
 * BRIEF ; The netfilter is designed to -
 * -> DROP all UPD packets except on Port 53
 * -> ACCEPT all TCP & detect TCP-SYN ACK & FIN scans
 * -> ACCEPT all ICMP
 * -> ACCEPT everything else
 * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/netfilter.h>
#include<linux/netfilter_ipv4.h>
#include<linux/ip.h>
#include<linux/tcp.h>
#include<linux/udp.h>

// module information :
// use $ modinfo NF_KMOD
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ritik Malik");
MODULE_DESCRIPTION("A netfilter kernel module to identify all TCP, UDP, ICMP packets");
MODULE_VERSION("0.1");

static struct nf_hook_ops *nfho = NULL;

// the main function
static unsigned int hfunc(void *priv, struct sk_buff *skb, const struct nf_hook_state *state){
	
	// define IP + UDP + TCP headers
	struct iphdr *iph;
	struct udphdr *udph;
	struct tcphdr *tcph;

	// check empty socket buffer
	if (!skb)
		return NF_ACCEPT;

	iph = ip_hdr(skb);

	// detect & drop UPD packet (except Port 53)
	if (iph->protocol == IPPROTO_UDP){

		udph = udp_hdr(skb);	// store the UDP header

		// log in dmesg
		printk(KERN_INFO "NF_KMOD: Detected UDP packet on Port %d\n", ntohs(udph->dest));

		// accept UDP only on Port 53
		if (ntohs(udph->dest) == 53){
			printk(KERN_INFO "NF_KMOD: ACCEPTING the above UDP packet\n");
			return NF_ACCEPT;
		}

		printk(KERN_INFO "NF_KMOD: DROPPING the above UDP packet\n");

		// drop the packet
		return NF_DROP;
	}

	// detect TCP packet
	else if (iph->protocol == IPPROTO_TCP){
		
		tcph = tcp_hdr(skb);	// store TCP header

		// detect & log TCP-SYN scan
		if (tcph->syn == 1 && tcph->ack == 0 && tcph->urg == 0 &&
			 tcph->rst == 0 && tcph->fin == 0 && tcph->psh == 0) {

			printk(KERN_INFO "NF_KMOD: Detected TCP-SYN scan on Port %d\n", ntohs(tcph->dest));
		}

		// detect & log TCP-ACK scan
		else if (tcph->syn == 0 && tcph->ack == 1 && tcph->urg == 0 &&
			 tcph->rst == 0 && tcph->fin == 0 && tcph->psh == 0) {
			
			printk(KERN_INFO "NF_KMOD: Detected TCP-ACK scan on Port %d\n", ntohs(tcph->dest));
		}
	
		// detect & log TCP-FIN scan
		else if (tcph->syn == 0 && tcph->ack == 0 && tcph->urg == 0 &&
			 tcph->rst == 0 && tcph->fin == 1 && tcph->psh == 0) {
			
			printk(KERN_INFO "NF_KMOD: Detected TCP-FIN scan on Port %d\n", ntohs(tcph->dest));
		}

		// detect all other TCP scan
		else{
		printk(KERN_INFO "NF_KMOD: Detected TCP packet on Port %d\n", ntohs(tcph->dest));
		}

		// log final message & drop the packet
		printk(KERN_INFO "NF_KMOD: ACCEPTING the above TCP packet\n");
		return NF_ACCEPT;

	}

	// detect & ACCEPT ICMP packet
	else if (iph->protocol == IPPROTO_ICMP){
		printk(KERN_INFO "NF_KMOD: Detected ICMP packet\n");
		printk(KERN_INFO "NF_KMOD: ACCEPTING the above ICMP packet\n");
		return NF_ACCEPT;
	}

	// accept all other
	return NF_ACCEPT;
}

// std init function
static int __init NF_MOD_START(void){

	nfho = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);

	nfho->hook	= (nf_hookfn*)hfunc;
	nfho->hooknum	= NF_INET_PRE_ROUTING;
	nfho->pf	= PF_INET;
	nfho->priority	= NF_IP_PRI_FIRST;

	// register the above hooks
	nf_register_net_hook(&init_net, nfho);
	printk(KERN_INFO "NF_KMOD: -----------------------------------------\n");
	printk(KERN_INFO "NF_KMOD: Inserted the netfilter kernel module\n");
	printk(KERN_INFO "NF_KMOD: -----------------------------------------\n");
	return 0;

}

// std exit function
static void __exit NF_MOD_END(void){
	
	// unregister the hooks
	nf_unregister_net_hook(&init_net, nfho);
	kfree(nfho);
	printk(KERN_INFO "NF_KMOD: -----------------------------------------\n");
	printk(KERN_INFO "NF_KMOD: Removed the netfilter kernel module\n");
	printk(KERN_INFO "NF_KMOD: -----------------------------------------\n");
}

module_init(NF_MOD_START);
module_exit(NF_MOD_END);

