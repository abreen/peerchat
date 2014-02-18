/*
 * host.h - Functions used by peerchat when acting as host
 */

#ifndef HOST_H
#define HOST_H

#include "peer.h"	// for peer_t

#include <sys/un.h>	// for sockaddr_un struct


int setup_host(int, struct sockaddr_un, peer_t *);
void request_migrate();

#endif
