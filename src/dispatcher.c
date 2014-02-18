/*
 * dispatcher.c - thread to handle dispatching strings to peers
 */

#include "dispatcher.h"
#include "queue.h"		// for queue_item_t, queue_t, etc.
#include "peer.h"		// for peer_t, peer_list_t, etc.

/*
 * Should be called by the server after having initialized all the
 * necessary structures (peer list and output queue). This function
 * will wait until the buffer contains an item and dispatch it immediately
 * to the client listening.
 */
void dispatch(peer_list_t *plist, queue_t *q)
{

}


/*
 * This dequeue function is defined here for protection. Only the dispatcher
 * should know how to remove items from the queue, and it should only do it
 * when noticing the queue is not empty.
 */
static entry_t *dequeue(queue_t *q)
{
	size_t i = q->start;

	queue_item_t *tmp = q->entries[i];

	q->entries[i] = NULL;

	q->start = (q->start + 1) % QUEUE_SIZE;

	q->num_items--;

	return tmp;
}

