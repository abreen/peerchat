/*
 * peer.h - def. of peer struct, peer list struct, and associated functions
 */

#ifndef PEER_H
#define PEER_H

#include <stddef.h>		// for size_t

typedef struct peer {
	char *username;		// username of connected peer
	int fd;			// socket over which to write to this peer
	struct peer *next;	// next peer in linked list
} peer_t;

typedef struct peer_list {
	peer_t *head;
	size_t num_peers;
} peer_list_t;

void init_peer_list(peer_list_t *);

void add_peer(peer_list_t *, peer_t *);

void remove_peer(peer_list_t *, peer_t *);

peer_t **get_peers(peer_list_t *);

#endif
