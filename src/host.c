#include "host.h"
#include <unistd.h>
#include <stdlib.h>

static void _notify(sinet_observer_t * sinet_observer, void * object)
{
	sinet_host_t * this = sinet_observer->object;
	sinet_ethernet_t * eth = object;
	if (NULL != eth) {
		if ((eth->to == this->interface->mac) ||
				(eth->to == sinet_MAC_bcast())) {
			this->buffer[this->h++] = eth;
		}
	}
}

static int _registerObserver(sinet_observable_t * sinet_observable,
		sinet_observer_t * sinet_observer)
{
	sinet_host_t * this = sinet_observable->object;
	if (NULL == this->interface->txlink) {
		this->interface->txlink = sinet_observer;
		return 1;
	}

	return 0;
}

static int _unregisterObserver(sinet_observable_t * sinet_observable,
		sinet_observer_t * sinet_observer)
{
	sinet_host_t * this = sinet_observable->object;
	void * observer = this->interface->txlink;
	if (observer == sinet_observer) {
		observer = NULL;
		return 1;
	}

	return 0;
}

static void _notifyObservers(sinet_observable_t * sinet_observable,
		void * object)
{
	sinet_host_t * this = sinet_observable->object;
	sinet_observer_t * observer = this->interface->txlink;
	if (NULL != observer) {
		observer->notify(observer, object);
	}
}

static void _send(sinet_host_t * this, void *m)
{
	this->observable->notifyObservers(this->observable, m);
}

static void * _recv(sinet_host_t * this)
{
	void * msg;

	if (this->t == this->h)
		msg = this->buffer[this->t];
	else
		msg = this->buffer[this->t++];

	return msg;
}

sinet_host_t * sinet_new_host(const char *ip, unsigned char mask,
		const char *mac)
{
	sinet_host_t * this;
	this = (sinet_host_t *) calloc(1, sizeof(* this));

	this->h = 0;
	this->t = 0;

	this->send = _send;
	this->recv = _recv;

	this->interface = malloc(sizeof(interface_t *));
	this->interface->ip = sinet_new_IP(ip);
	this->interface->mask = mask;
	this->interface->bcast = sinet_new_IP("192.158.0.255");
	this->interface->mac = sinet_new_MAC(mac);
	this->interface->txlink = malloc(sizeof(void *));
	this->interface->txlink = NULL;

	this->arp_response_to = 0;
	int i;
	//init arp table
	for (i=0; i<ARP_TABLE_SIZE; i++) {
		this->arp_table[i] = malloc(sizeof(arp_entry_t *));
		this->arp_table[i]->ip = 0;
		this->arp_table[i]->mac = 0;
		this->arp_table[i]->ifaceid = 0;
		this->arp_table[i]->expires = 0;
	}

	for (i=0; i<255; i++) {
		this->buffer[i] = NULL;
	}

	//rx
	this->observer = sinet_new_observer(this,
			(void(*)(sinet_observer_t *, void *)) _notify);

	//tx
	this->observable = sinet_new_observable(this);
	this->observable->registerObserver = _registerObserver;
	this->observable->unregisterObserver = _unregisterObserver;
	this->observable->notifyObservers = _notifyObservers;

	return this;
}

void sinet_set_run(sinet_host_t *host, void (*run)(sinet_host_t *))
{
	host->run = run;
}

void sinet_set_gateway(sinet_host_t *host, sinet_IP_t gateway)
{
	// TODO tratar se o gatway e da mesma rede que o host
	host->interface->gateway = gateway;
}
