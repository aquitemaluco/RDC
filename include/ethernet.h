#ifndef ETHERNET_H
#define ETHERNET_H
#include "mac.h"
#include <pthread.h>

#define T_IPv4 0x0800
#define T_ARP 0x0806

pthread_mutex_t isNewETH;

typedef struct {
	sinet_MAC_t from;
	sinet_MAC_t to;
	int type;
	void *payload;
} sinet_ethernet_t;

sinet_ethernet_t *sinet_new_ethernet(sinet_MAC_t from, sinet_MAC_t to,
		int type, void* payload);

#endif
