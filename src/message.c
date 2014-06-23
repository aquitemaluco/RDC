#include "message.h"
#include <stdlib.h>

sinet_message_t *sinet_new_message(sinet_IP_t from, sinet_IP_t to,
		void * payload)
{
	sinet_message_t * this;

	this = (sinet_message_t *) calloc(1, sizeof(* this));
	this->from = from;
	this->to = to;
	this->payload = payload;

	return this;
}

