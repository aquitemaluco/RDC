#include "ip.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

sinet_IP_t sinet_new_IP(const char *ip)
{
	sinet_IP_t this;

	int i, count_dot=0;
	char *tok, lip[16];
	strcpy(lip, ip);

	for (i=0; i<strlen(lip); i++) {
		if (lip[i] == '.') {
			count_dot++;
		}
	}

	if (count_dot != 3) {
		//invalid ip
		fprintf(stderr, "IP invalido [%s] abortando execucao\n", lip);
		exit(1);
	}

	lip[strlen(lip)] = '.';
	tok = strtok(lip, ".");
	this = atoi(tok);
	this = this << 8;

	tok = strtok(NULL, ".");
	this = this | atoi(tok);;
	this = this << 8;

	tok = strtok(NULL, ".");
	this = this | atoi(tok);;
	this = this << 8;

	tok = strtok(NULL, ".");
	this = this | atoi(tok);;

	return this;
}
