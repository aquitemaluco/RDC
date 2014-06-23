#include "hub.h"
#include "host.h"
#include "sinet.h"
#include <stdio.h>
#include <stdlib.h>

void run_19216801(sinet_host_t *);
void run_19216802(sinet_host_t *);
void run_19216803(sinet_host_t *);
void run_19216804(sinet_host_t *);

int main(int argc, char *argv[])
{
	sinet_host_t *hosts[MAX_HOST];
	hosts[0] = sinet_new_host("192.168.0.1", 24, "aa:bb:cc:dd:ee:ff");
	hosts[1] = sinet_new_host("192.168.0.2", 24, "11:22:33:44:55:66");
	hosts[2] = sinet_new_host("192.168.0.3", 24, "ff:ee:dd:cc:bb:aa");
	hosts[3] = sinet_new_host("192.168.0.4", 24, "66:55:44:33:22:11");

	#ifdef HC
	sinet_init(HUB);
	#endif
	#ifdef SC
	sinet_init(SWITCH);
	#endif

	sinet_connect(hosts[0]);
	sinet_set_run(hosts[0], run_19216801);
	sinet_connect(hosts[1]);
	sinet_set_run(hosts[1], run_19216802);
	sinet_connect(hosts[2]);
	sinet_set_run(hosts[2], run_19216803);
	sinet_connect(hosts[3]);
	sinet_set_run(hosts[3], run_19216804);

	sinet_start_execution(hosts);

	sinet_free();
	return 0;
}

void show(const char * ip, const char * msg)
{
	printf("%s %s\n", ip, msg);
	fflush(stdout);
}

void run_19216801(sinet_host_t * sinet_host)
{
	sinet_IP_t to = sinet_new_IP("192.168.0.2");
	sinet_MAC_t macto = 0;

	sinet_message_t * msg;
	sinet_message_t * pmsg;
	arp_message_t * arpreq;
	sinet_message_t * marpreq; //message arp request
	sinet_ethernet_t * ethr; //recv
	sinet_ethernet_t * eths; //resp

	int i;
	for (i=0; i<10; i++) {
		pthread_mutex_lock(&isARPGetEntry);
		macto = get_arp_entry(sinet_host->arp_table, to);
		pthread_mutex_unlock(&isARPGetEntry);
		if (!macto) {
			pthread_mutex_lock(&isNewMSG);
			pmsg = sinet_new_message(sinet_host->interface->ip, to,
					NULL);
			pthread_mutex_unlock(&isNewMSG);

			pthread_mutex_lock(&isNewETH);
			eths = sinet_new_ethernet(sinet_host->interface->mac, 0,
					T_ARP, marpreq);
			pthread_mutex_unlock(&isNewETH);

			arpreq = malloc(sizeof(* arpreq));
			arpreq->message = ARP_REQUEST;

			pmsg->payload = arpreq;

			eths->to = sinet_MAC_bcast();
			eths->payload = pmsg;
			pthread_mutex_lock(&isHostSend);
			sinet_host->send(sinet_host, eths);
			pthread_mutex_unlock(&isHostSend);
			sleep(5);//whait response
			free(arpreq);
		}

		pthread_mutex_lock(&isHostRecv);
		while (NULL != (ethr = sinet_host->recv(sinet_host))) {
			if ((NULL == (msg = ethr->payload)) ||
					(sinet_host->interface->ip != msg->to)) {
				continue;
			}


			if (ethr->type == T_ARP) {
				if (NULL != (arpreq = msg->payload)) {
					if (arpreq->message == ARP_RESPONSE) {
						//do something
					}

					pthread_mutex_lock(&isARPAddEntry);
					add_arp_entry(sinet_host->arp_table, msg->from,
						ethr->from);
					pthread_mutex_unlock(&isARPAddEntry);
					macto = ethr->from;
				}
				break;
			} else {
				if (NULL != msg->payload) {
					show("192.168.0.1 r", (char *) msg->payload);
				}
			}
		}
		pthread_mutex_unlock(&isHostRecv);
		sleep(1);

		//check arp table
		pthread_mutex_lock(&isARPGetEntry);
		macto = get_arp_entry(sinet_host->arp_table, to);
		pthread_mutex_unlock(&isARPGetEntry);

		if (macto) {
			pthread_mutex_lock(&isNewMSG);
			pmsg = sinet_new_message(sinet_host->interface->ip, to,
					NULL);
			pthread_mutex_unlock(&isNewMSG);

			pthread_mutex_lock(&isNewETH);
			eths = sinet_new_ethernet(sinet_host->interface->mac, 0,
					T_IPv4, NULL);
			pthread_mutex_unlock(&isNewETH);

			pmsg->payload = "teste";
			eths->to = macto;
			eths->payload = pmsg;
			pthread_mutex_lock(&isHostSend);
			sinet_host->send(sinet_host, eths);
			pthread_mutex_unlock(&isHostSend);
			show("192.168.0.1 s", "teste");
			sleep(1);
		} else {
			show("192.168.0.1 e", "Host [192.168.0.2] not found");
		}
	}
}

void run_19216802(sinet_host_t * sinet_host)
{
	int aresp = 0;
	sinet_IP_t to = 0;
	sinet_MAC_t macto = 0;

	sinet_message_t * msg;
	sinet_message_t * pmsg;
	arp_message_t * arpreq;
	sinet_message_t * marpreq; //message arp request
	sinet_ethernet_t * ethr; //recv
	sinet_ethernet_t * eths; //resp

	int i;
	for (i=0; i<10; i++) {
		pthread_mutex_lock(&isHostRecv);
		while (NULL != (ethr = sinet_host->recv(sinet_host))) {
			if ((NULL == (msg = ethr->payload)) ||
					(sinet_host->interface->ip != msg->to)) {
				continue;
			}

			to = msg->from;
			macto = ethr->from;
			if (ethr->type == T_ARP) {
				if (NULL != (arpreq = msg->payload)) {
					if (arpreq->message == ARP_REQUEST) {
						aresp = 1;
						show("192.168.0.2 arp", "ARP_REQUEST");
					} else if (arpreq->message == ARP_RESPONSE) {
						//do something
					}

					pthread_mutex_lock(&isARPAddEntry);
					add_arp_entry(sinet_host->arp_table, msg->from,
						ethr->from);
					pthread_mutex_unlock(&isARPAddEntry);
				}
				break;
			} else {
				if (NULL != msg->payload) {
					show("192.168.0.2 r", (char *) msg->payload);
				}
			}
		}
		pthread_mutex_unlock(&isHostRecv);

		sleep(1);
		if (aresp) {
			if ((!macto) && (!to)) {
				show("192.168.0.2 arp", "bad request!");
				continue;
			}

			pthread_mutex_lock(&isNewMSG);
			pmsg = sinet_new_message(sinet_host->interface->ip, to,
					NULL);
			pthread_mutex_unlock(&isNewMSG);

			pthread_mutex_lock(&isNewETH);
			eths = sinet_new_ethernet(sinet_host->interface->mac, 0,
					T_ARP, marpreq);
			pthread_mutex_unlock(&isNewETH);

			arpreq = malloc(sizeof(* arpreq));
			arpreq->message = ARP_RESPONSE;

			pmsg->payload = arpreq;
			pmsg->to;

			eths->type = T_ARP;
			eths->to = macto;
			eths->payload = pmsg;
			pthread_mutex_lock(&isHostSend);
			sinet_host->send(sinet_host, eths);
			pthread_mutex_unlock(&isHostSend);

		} else {
			if (macto) {
				pthread_mutex_lock(&isNewMSG);
				pmsg = sinet_new_message(sinet_host->interface->ip, to,
						NULL);
				pthread_mutex_unlock(&isNewMSG);

				pthread_mutex_lock(&isNewETH);
				eths = sinet_new_ethernet(sinet_host->interface->mac, 0,
						T_IPv4, NULL);
				pthread_mutex_unlock(&isNewETH);

				pmsg->payload = "yahoo";
				pmsg->to;

				eths->type = T_IPv4;
				eths->to = macto;
				eths->payload = pmsg;
				pthread_mutex_lock(&isHostSend);
				sinet_host->send(sinet_host, eths);
				pthread_mutex_unlock(&isHostSend);
				show("192.168.0.2 s", "yahoo");

			} else {
				show("192.168.0.2 m", "nothing to do");
			}
		}

		to = 0;
		macto = 0;
		aresp = 0;
		sleep(1);
	}
}

void run_19216803(sinet_host_t * sinet_host)
{
	sinet_IP_t to = sinet_new_IP("192.168.0.4");
	sinet_MAC_t macto = 0;

	sinet_message_t * msg;
	sinet_message_t * pmsg;
	arp_message_t * arpreq;
	sinet_message_t * marpreq; //message arp request
	sinet_ethernet_t * ethr; //recv
	sinet_ethernet_t * eths; //resp

	int i;
	for (i=0; i<10; i++) {
		pthread_mutex_lock(&isARPGetEntry);
		macto = get_arp_entry(sinet_host->arp_table, to);
		pthread_mutex_unlock(&isARPGetEntry);
		if (!macto) {
			pthread_mutex_lock(&isNewMSG);
			pmsg = sinet_new_message(sinet_host->interface->ip, to,
					NULL);
			pthread_mutex_unlock(&isNewMSG);

			pthread_mutex_lock(&isNewETH);
			eths = sinet_new_ethernet(sinet_host->interface->mac, 0,
					T_ARP, marpreq);
			pthread_mutex_unlock(&isNewETH);

			arpreq = malloc(sizeof(* arpreq));
			arpreq->message = ARP_REQUEST;

			pmsg->payload = arpreq;

			eths->to = sinet_MAC_bcast();
			eths->payload = pmsg;
			pthread_mutex_lock(&isHostSend);
			sinet_host->send(sinet_host, eths);
			pthread_mutex_unlock(&isHostSend);
			sleep(5);//whait response
			free(arpreq);
		}

		pthread_mutex_lock(&isHostRecv);
		while (NULL != (ethr = sinet_host->recv(sinet_host))) {
			if ((NULL == (msg = ethr->payload)) ||
					(sinet_host->interface->ip != msg->to)) {
				continue;
			}


			if (ethr->type == T_ARP) {
				if (NULL != (arpreq = msg->payload)) {
					if (arpreq->message == ARP_RESPONSE) {
						//do something
					}

					pthread_mutex_lock(&isARPAddEntry);
					add_arp_entry(sinet_host->arp_table, msg->from,
						ethr->from);
					pthread_mutex_unlock(&isARPAddEntry);
					macto = ethr->from;
				}
				break;
			} else {
				if (NULL != msg->payload) {
					show("192.168.0.3 r", (char *) msg->payload);
				}
			}
		}
		pthread_mutex_unlock(&isHostRecv);
		sleep(1);

		//check arp table
		pthread_mutex_lock(&isARPGetEntry);
		macto = get_arp_entry(sinet_host->arp_table, to);
		pthread_mutex_unlock(&isARPGetEntry);

		if (macto) {
			pthread_mutex_lock(&isNewMSG);
			pmsg = sinet_new_message(sinet_host->interface->ip, to,
					NULL);
			pthread_mutex_unlock(&isNewMSG);

			pthread_mutex_lock(&isNewETH);
			eths = sinet_new_ethernet(sinet_host->interface->mac, 0,
					T_IPv4, NULL);
			pthread_mutex_unlock(&isNewETH);

			pmsg->payload = "ping";
			eths->to = macto;
			eths->payload = pmsg;
			pthread_mutex_lock(&isHostSend);
			sinet_host->send(sinet_host, eths);
			pthread_mutex_unlock(&isHostSend);
			show("192.168.0.3 s", "ping");
			sleep(1);
		} else {
			show("192.168.0.3 e", "Host [192.168.0.4] not found");
		}
	}
}

void run_19216804(sinet_host_t * sinet_host)
{
	int aresp = 0;
	sinet_IP_t to = 0;
	sinet_MAC_t macto = 0;

	sinet_message_t * msg;
	sinet_message_t * pmsg;
	arp_message_t * arpreq;
	sinet_message_t * marpreq; //message arp request
	sinet_ethernet_t * ethr; //recv
	sinet_ethernet_t * eths; //resp

	int i;
	for (i=0; i<10; i++) {
		pthread_mutex_lock(&isHostRecv);
		while (NULL != (ethr = sinet_host->recv(sinet_host))) {
			if ((NULL == (msg = ethr->payload)) ||
					(sinet_host->interface->ip != msg->to)) {
				continue;
			}

			to = msg->from;
			macto = ethr->from;
			if (ethr->type == T_ARP) {
				if (NULL != (arpreq = msg->payload)) {
					if (arpreq->message == ARP_REQUEST) {
						aresp = 1;
						show("192.168.0.4 arp", "ARP_REQUEST");
					} else if (arpreq->message == ARP_RESPONSE) {
						//do something
					}

					pthread_mutex_lock(&isARPAddEntry);
					add_arp_entry(sinet_host->arp_table, msg->from,
						ethr->from);
					pthread_mutex_unlock(&isARPAddEntry);
				}
				break;
			} else {
				if (NULL != msg->payload) {
					show("192.168.0.4 r", (char *) msg->payload);
				}
			}
		}
		pthread_mutex_unlock(&isHostRecv);

		sleep(1);
		if (aresp) {
			if ((!macto) && (!to)) {
				show("192.168.0.4 arp", "bad request!");
				continue;
			}

			pthread_mutex_lock(&isNewMSG);
			pmsg = sinet_new_message(sinet_host->interface->ip, to,
					NULL);
			pthread_mutex_unlock(&isNewMSG);

			pthread_mutex_lock(&isNewETH);
			eths = sinet_new_ethernet(sinet_host->interface->mac, 0,
					T_ARP, marpreq);
			pthread_mutex_unlock(&isNewETH);

			arpreq = malloc(sizeof(* arpreq));
			arpreq->message = ARP_RESPONSE;

			pmsg->payload = arpreq;
			pmsg->to;

			eths->type = T_ARP;
			eths->to = macto;
			eths->payload = pmsg;
			pthread_mutex_lock(&isHostSend);
			sinet_host->send(sinet_host, eths);
			pthread_mutex_unlock(&isHostSend);

		} else {
			if (macto) {
				pthread_mutex_lock(&isNewMSG);
				pmsg = sinet_new_message(sinet_host->interface->ip, to,
						NULL);
				pthread_mutex_unlock(&isNewMSG);

				pthread_mutex_lock(&isNewETH);
				eths = sinet_new_ethernet(sinet_host->interface->mac, 0,
						T_IPv4, NULL);
				pthread_mutex_unlock(&isNewETH);

				pmsg->payload = "pong";
				pmsg->to;

				eths->type = T_IPv4;
				eths->to = macto;
				eths->payload = pmsg;
				pthread_mutex_lock(&isHostSend);
				sinet_host->send(sinet_host, eths);
				pthread_mutex_unlock(&isHostSend);
				show("192.168.0.4 s", "pong");

			} else {
				show("192.168.0.4 m", "nothing to do");
			}
		}

		to = 0;
		macto = 0;
		aresp = 0;
		sleep(1);
	}
}
