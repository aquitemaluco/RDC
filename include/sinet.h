#ifndef SINET_H
#define SINET_H
#include "device.h"
#include "host.h"
#include "message.h"

#define DEFLT_MAX_PORT 4

void sinet_create_device(sinet_device_type_t device,
		unsigned char max_ports);
void sinet_set_max_ports(unsigned char max_ports);

int sinet_connect(sinet_host_t * sinet_host);

void sinet_init(sinet_device_type_t device);
void sinet_start_execution(sinet_host_t **);
void sinet_free();

#endif
