#ifndef HUB_H
#define HUB_H
#include "observer.h"
#include "observable.h"

typedef struct __sinet_hub {
	void **ports;
	unsigned char max_ports;

	sinet_observable_t *observable;
	sinet_observer_t *observer;
} sinet_hub_t;

sinet_hub_t * sinet_new_HUB(unsigned char max_ports);

#endif

