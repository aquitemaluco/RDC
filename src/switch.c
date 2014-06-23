#include "ethernet.h"
#include "switch.h"
#include "host.h"
#include <stdlib.h>

static int _registerObserver(sinet_observable_t * sinet_observable,
		sinet_observer_t * sinet_observer)
{
	sinet_switch_t * this = sinet_observable->object;
	sinet_host_t * host = sinet_observer->object;

	int i;
	for(i = 0; i < this->max_ports; i++) {
		if (NULL == this->ports[i]->link) {
			this->ports[i]->link = sinet_observer;
			this->ports[i]->mac = host->interface->mac;
			return 1;
		}
	}
	return 0;
}

static int _unregisterObserver(sinet_observable_t * sinet_observable,
		sinet_observer_t * sinet_observer)
{
	sinet_switch_t * this = sinet_observable->object;

	int i;
	for(i = 0; i < this->max_ports; i++) {
		void * observer = this->ports[i]->link;
		if (observer == sinet_observer) {
			this->ports[i]->mac = 0;
			observer = NULL;
			return 1;
		}
	}
	return 0;
}

static void _notifyObservers(sinet_observable_t * sinet_observable,
		void * object)
{
	printf("Hello from SWITCH\n");
	sinet_switch_t * this = sinet_observable->object;

	int i;
	sinet_observer_t * observer;
	sinet_ethernet_t * eth = object;
	for(i = 0; i < this->max_ports; i++) {
		observer = this->ports[i]->link;
		if ((NULL != observer) && (NULL != eth)) {
			if ((this->ports[i]->mac == eth->to) ||
					(sinet_MAC_bcast() == eth->to)) {
				observer->notify(observer, object);
			}
		}
	}
}

static void _notify(sinet_observer_t * sinet_observer, void * object)
{
	sinet_switch_t * this = sinet_observer->object;
	_notifyObservers(this->observable, object);
}

sinet_switch_t * sinet_new_SWITCH(unsigned char max_ports)
{
	sinet_switch_t * this;
	this = (sinet_switch_t *) calloc(1, sizeof(* this));
	this->ports = calloc(0, sizeof(* this->ports) * max_ports);
	this->max_ports = max_ports;

	//rx
	this->observer = sinet_new_observer(this,
			(void(*)(sinet_observer_t *, void *)) _notify);

	//tx
	this->observable = sinet_new_observable(this);
	this->observable->registerObserver = _registerObserver;
	this->observable->unregisterObserver = _unregisterObserver;
	this->observable->notifyObservers = _notifyObservers;

	int i;
	for (i=0; i<max_ports; i++) {
		this->ports[i] = malloc(sizeof(port_t *));
		this->ports[i]->mac = 0;
		this->ports[i]->link = NULL;
	}

	return this;
}
