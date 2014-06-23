#ifndef SWITCH_H
#define SWITCH_H
#include "observer.h"
#include "observable.h"

typedef struct {
	sinet_MAC_t mac;
	void * link;
} port_t;

typedef struct __sinet_switch {
	port_t **ports;
	unsigned char max_ports;

	sinet_observable_t *observable;
	sinet_observer_t *observer;
} sinet_switch_t;

sinet_switch_t * sinet_new_SWITCH(unsigned char max_ports);

#endif

