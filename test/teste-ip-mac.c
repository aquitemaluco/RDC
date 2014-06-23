#include "ip.h"
#include "mac.h"
#include <stdio.h>
int main(void)
{
	sinet_IP_t *ip = sinet_new_IP("0.0.0.255");
	printf("%d\n", *ip);
	sinet_MAC_t *mac = sinet_new_MAC("00:00:00:00:00:ff");
	printf("%ld\n", *mac);
}
