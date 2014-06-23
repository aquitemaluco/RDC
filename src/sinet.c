#include "sinet.h"
#include "hub.h"
#include "switch.h"
#include "host.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static sinet_device_t * _sinet_device;

void sinet_init(sinet_device_type_t device)
{
	sinet_create_device(device, DEFLT_MAX_PORT);
}

void * host_run(void * object)
{
	sinet_host_t * sinet_host = object;
	sinet_host->run(sinet_host);

	return NULL;
}

void sinet_start_execution(sinet_host_t **sinet_hosts)
{
	pthread_t threads[MAX_HOST];
	pthread_mutex_init(&isHostSend, NULL);
	pthread_mutex_init(&isHostRecv, NULL);
	pthread_mutex_init(&isNewMSG, NULL);
	pthread_mutex_init(&isNewETH, NULL);
	pthread_mutex_init(&isARPGetEntry, NULL);
	pthread_mutex_init(&isARPAddEntry, NULL);

	int i, rc;
	sinet_host_t * sinet_host;
	for (i=0; i < MAX_HOST; i++) {
		sinet_host = sinet_hosts[i];
		rc = pthread_create(&threads[i], NULL, host_run, sinet_host);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n",
					rc);

			exit(-1);
		}
	}

	for (i=0; i < MAX_HOST; i++) {
		pthread_join(threads[i], NULL);
	}
}

void sinet_free()
{
}

void sinet_create_device(sinet_device_type_t device,
		unsigned char max_ports)
{
	_sinet_device = sinet_new_device(device);
	switch (device) {
	case HUB:
		_sinet_device->device = sinet_new_HUB(max_ports);
		break;
	case SWITCH:
		_sinet_device->device = sinet_new_SWITCH(max_ports);
		break;
	default:
		break;
	}
}

int sinet_connect(sinet_host_t * sinet_host)
{
	if (HUB == _sinet_device->device_type) {
		sinet_hub_t * sinet_hub = _sinet_device->device;
		sinet_hub->observable->registerObserver(sinet_hub->observable,
				sinet_host->observer);
		sinet_host->observable->registerObserver(sinet_host->observable,
				sinet_hub->observer);
	}

	if (SWITCH == _sinet_device->device_type) {
		sinet_switch_t * sinet_switch = _sinet_device->device;
		sinet_switch->observable->registerObserver(
				sinet_switch->observable, sinet_host->observer);
		sinet_host->observable->registerObserver(sinet_host->observable,
				sinet_switch->observer);
	}

	return 1;
}
