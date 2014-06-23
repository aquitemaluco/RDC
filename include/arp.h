#ifndef ARP_H
#define ARP_H
#include "ip.h"
#include "mac.h"
#include <pthread.h>

#define ARP_REQUEST 1
#define ARP_RESPONSE 2
#define ARP_TABLE_SIZE 255

pthread_mutex_t isARPGetEntry;
pthread_mutex_t isARPAddEntry;

typedef struct {
	int message;
} arp_message_t;

typedef struct {
	sinet_IP_t ip;
	sinet_MAC_t mac;
	unsigned char ifaceid;
	unsigned char expires;
} arp_entry_t;

sinet_MAC_t get_arp_entry(arp_entry_t **arp_table, sinet_IP_t ip);
int add_arp_entry(arp_entry_t **arp_table, sinet_IP_t ip,
		sinet_MAC_t mac);


#endif
