#include "ethernet.h"
#include <stdlib.h>

sinet_ethernet_t *sinet_new_ethernet(sinet_MAC_t from, sinet_MAC_t to,
		int type, void* payload)
{
	sinet_ethernet_t * this;

	this = (sinet_ethernet_t *) calloc(1, sizeof(* this));
	this->from = from;
	this->to = to;
	this->type = type;
	this->payload = payload;

	return this;
}
