#ifndef MAC_H
#define MAC_H
#include <stdint.h>

/* only used 48 bit for MAC. */
typedef uint64_t sinet_MAC_t;
sinet_MAC_t sinet_new_MAC(const char *mac);
sinet_MAC_t sinet_MAC_bcast();

#endif
