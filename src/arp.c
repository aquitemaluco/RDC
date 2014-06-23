#include "arp.h"

sinet_MAC_t get_arp_entry(arp_entry_t **arp_table, sinet_IP_t ip)
{
	sinet_MAC_t mac = 0;
	int i;
	for (i=0; i<ARP_TABLE_SIZE; i++) {
		if (ip == arp_table[i]->ip) {
			mac = arp_table[i]->mac;
			break;
		}
	}

	return mac;
}

int add_arp_entry(arp_entry_t **arp_table, sinet_IP_t ip,
		sinet_MAC_t mac)
{
	int i;
	for (i=0; i<ARP_TABLE_SIZE; i++) {
		if (0 == arp_table[i]->ip) {
			arp_table[i]->ip = ip;
			arp_table[i]->mac = mac;
			return 1;
		}
	}

	return 0;
}
