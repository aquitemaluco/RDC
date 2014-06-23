#include "hub.h"
#include "host.h"
#include <stdlib.h>

static int _registerObserver(sinet_observable_t * sinet_observable,
		sinet_observer_t * sinet_observer)
{
	sinet_hub_t * this = sinet_observable->object;

	int i;
	for(i = 0; i < this->max_ports; i++) {
		if (NULL == this->ports[i]) {
			this->ports[i] = sinet_observer;
			return 1;
		}
	}
	return 0;
}

static int _unregisterObserver(sinet_observable_t * sinet_observable,
		sinet_observer_t * sinet_observer)
{
	sinet_hub_t * this = sinet_observable->object;

	int i;
	for(i = 0; i < this->max_ports; i++) {
		void * observer = this->ports[i];
		if (observer == sinet_observer) {
			observer = NULL;
			return 1;
		}
	}
	return 0;
}

static void _notifyObservers(sinet_observable_t * sinet_observable,
		void * object)
{
	printf("Hello from HUB\n");
	sinet_hub_t * this = sinet_observable->object;

	int i;
	sinet_observer_t * observer;
	for(i = 0; i < this->max_ports; i++) {
		observer = this->ports[i];
		if (NULL != observer) {
			observer->notify(observer, object);
		}
	}
}

static void _notify(sinet_observer_t * sinet_observer, void * object)
{
	sinet_hub_t * this = sinet_observer->object;
	_notifyObservers(this->observable, object);
}

sinet_hub_t * sinet_new_HUB(unsigned char max_ports)
{
	sinet_hub_t * this;
	this = (sinet_hub_t *) calloc(1, sizeof(* this));
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
	for (i=0; i< max_ports; i++) {
		this->ports[i] = malloc(sizeof(void *));
		this->ports[i] = NULL;
	}

	return this;
}
