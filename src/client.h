/*
 * client.h - Functions used by peerchat when acting as client
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "peer.h"	// for peer_t

#include <sys/un.h>	// for sockaddr_un struct

int setup_client(int, struct sockaddr_un, peer_t *);
void accept_migrate();

#endif
