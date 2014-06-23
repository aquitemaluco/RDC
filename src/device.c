#include "device.h"
#include <stdlib.h>

sinet_device_t * sinet_new_device(sinet_device_type_t device_type)
{
	sinet_device_t * this;
	this = (sinet_device_t *) calloc(1, sizeof(* this));
	this->device_type = device_type;
	return this;
}

