#ifndef INTERFACE_H
#define INTERFACE_H

	#include <stdint.h>

	typedef enum {
		HUB,
		SWITCH,
		ROUTER
	} sinet_device_type_t;


	void sinet_init(sinet_device_type_t device);
	void sinet_start_execution();
	void sinet_free();

	typedef uint32_t sinet_IP_t;
	sinet_IP_t *sinet_new_IP(const char *ip);
	/* only used 48 bit for MAC. */
	typedef uint64_t sinet_MAC_t;
	sinet_MAC_t *sinet_new_MAC(const char *mac);

	typedef struct {
		sinet_IP_t ip;
		unsigned char mask;
		sinet_IP_t gateway;
		sinet_MAC_t mac;
		sinet_device_t device;
		void (*run)(void);
	} sinet_host_t;
	sinet_host_t *sinet_new_host(const char *ip, unsigned char mask; const char *mac);
	void sinet_set_run(sinet_host_t *host, void (*run)(void));
	int sinet_connect(sinet_host_t *host);
	void sinet_set_gateway(sinet_host_t *host, sinet_IP_t gateway);
	
	/* put an static member in the implementation to use in send/recv functions */
	typedef struct {
		void *device;
	} sinet_device_t;
	void sinet_create_device(sinet_device_type_t device, unsigned char max_ports);
	void sinet_set_max_ports(sinet_device_t *device, unsigned char max_ports);

	typedef struct {
		sinet_IP_t from;
		sinet_IP_t to;
		void *payload;
	} sinet_message_t;
	sinet_message_t *sinet_new_message(const char *from, const char *to, void* payload);

	void sinet_send(sinet_message_t *m);
	sinet_message_t sinet_recv();
	

#endif 

