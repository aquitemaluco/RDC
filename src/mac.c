#include "mac.h"
#include <stdio.h>
#include <string.h>

static sinet_MAC_t mac_bcast = 0;

sinet_MAC_t sinet_new_MAC(const char *mac)
{
	sinet_MAC_t this;

	int i, count_dot=0;
	unsigned char byte;
	char *tok, lmac[18];
	strcpy(lmac, mac);

	for (i=0; i<strlen(lmac); i++) {
		if (lmac[i] == ':') {
			count_dot++;
		}
	}

	if (count_dot != 5) {
		//invalid ip
		fprintf(stderr, "MAC invalido [%s] abortando execucao\n", lmac);
	}

	lmac[strlen(lmac)] = ':';
	tok = strtok(lmac, ":");
	sscanf(tok, "%2hhx", &byte);
	this = byte;
	this = this << 8;

	tok = strtok(NULL, ":");
	sscanf(tok, "%2hhx", &byte);
	this = this | byte;
	this = this << 8;

	tok = strtok(NULL, ":");
	sscanf(tok, "%2hhx", &byte);
	this = this | byte;
	this = this << 8;

	tok = strtok(NULL, ":");
	sscanf(tok, "%2hhx", &byte);
	this = this | byte;
	this = this << 8;

	tok = strtok(NULL, ":");
	sscanf(tok, "%2hhx", &byte);
	this = this | byte;
	this = this << 8;

	tok = strtok(NULL, ":");
	sscanf(tok, "%2hhx", &byte);
	this = this | byte;

	return this;
}

sinet_MAC_t sinet_MAC_bcast()
{
	if (!mac_bcast) {
		mac_bcast = sinet_new_MAC("ff:ff:ff:ff:ff:ff");
	}

	return mac_bcast;
}
