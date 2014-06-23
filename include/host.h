#ifndef HOST_H
#define HOST_H
#include "ip.h"
#include "mac.h"
#include "arp.h"
#include "device.h"
#include "message.h"
#include "ethernet.h"
#include "observer.h"
#include "observable.h"
#include <pthread.h>

#define MAX_HOST 4

pthread_mutex_t isHostSend;
pthread_mutex_t isHostRecv;

typedef struct {
	sinet_IP_t ip;
	unsigned char mask;
	sinet_IP_t gateway;
	sinet_IP_t bcast;
	sinet_MAC_t mac;
	void *txlink;
} interface_t;

typedef struct __sinet_host {
	interface_t *interface;
	arp_entry_t *arp_table[ARP_TABLE_SIZE];
	sinet_IP_t arp_response_to;

	//Message buffer
	unsigned char h; //Head
	unsigned char t; //Tail
	void *buffer[255];

	sinet_device_t device;
	void (*run)(struct __sinet_host *);
	void (*send)(struct __sinet_host *, void *m);
	void * (*recv)(struct __sinet_host *);

	sinet_observable_t *observable;
	sinet_observer_t *observer;
} sinet_host_t;

sinet_host_t *sinet_new_host(const char *ip, unsigned char mask,
		const char *mac);
void sinet_set_run(sinet_host_t *host, void (*run)(sinet_host_t *));
void sinet_set_gateway(sinet_host_t *host, sinet_IP_t gateway);

#endif
