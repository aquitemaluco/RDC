#ifndef MESSAGE_H
#define MESSAGE_H
#include "ip.h"
#include <pthread.h>

pthread_mutex_t isNewMSG;

typedef struct {
	sinet_IP_t from;
	sinet_IP_t to;
	void *payload;
} sinet_message_t;

sinet_message_t *sinet_new_message(sinet_IP_t from, sinet_IP_t to,
		void * payload);

#endif
