#ifndef DEVICE_H
#define DEVICE_H

typedef enum {
	HUB,
	SWITCH,
	ROUTER
} sinet_device_type_t;

typedef struct {
	sinet_device_type_t device_type;
	void *device;
} sinet_device_t;

sinet_device_t * sinet_new_device();

/* put an static member in the implementation to use in send/recv
 * functions */

#endif
