/*
 * peer.c - functions to manipulate a linked list of peers
 *
 * These functions are used by a host to maintain a list of peers.
 * When the host needs to exit, it signals one of these peers to take
 * over as host.
 */

#include "peer.h"

#include <stdlib.h>		// for malloc
#include <string.h>		// for memset


/*
 * Initialize the peer list struct by simply setting all bytes of
 * the struct to zero.
 */
void init_peer_list(peer_list_t *list)
{
	memset(list, 0, sizeof(*list));
}

/*
 * Add the peer_t object pointed to by "new_peer" to the peer list
 * pointed to by "list".
 */
void add_peer(peer_list_t *list, peer_t *new_peer)
{
	peer_t *ptr = list->head;

	if (ptr == NULL) {
		list->head = new_peer;
		return;
	}

	while (ptr->next != NULL) ptr = ptr->next;
	
	ptr->next = new_peer;

	list->num_peers++;
}

/*
 * Remove the peer pointed to by "to_remove" from the peer list pointed to
 * by "list". Does nothing if the specified peer is not actually in the
 * list.
 */
void remove_peer(peer_list_t *list, peer_t *to_remove)
{
	peer_t *ptr = list->head;

	if (ptr == to_remove) {
		list->head = list->head->next;
		return;
	}

	// invariant: the node to remove is not the head

	while (ptr->next != NULL) {
		if (ptr->next == to_remove) {
			ptr->next = ptr->next->next;
			list->num_peers--;
			return;
		}

		ptr = ptr->next;
	}
}

/*
 * Transform the peer list pointed to by "list" into an array of peer_t
 * objects. Return NULL if the peer list is empty.
 *
 * This function allocates memory. Use free() when finished with the
 * resultant array.
 */
peer_t **get_peers(peer_list_t *list)
{
	if (list->num_peers == 0) return NULL;

	peer_t **arr = malloc(sizeof(peer_t) * list->num_peers);
	peer_t *ptr = list->head;

	int i = 0;
	while (ptr != NULL) {
		arr[i] = ptr;
		i++;
		ptr = ptr->next;
	}

	return arr;
}
