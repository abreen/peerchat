/*
 * queue.c - functions to manipulate queue of outgoing strings
 */

#include "queue.h"

#include <stdlib.h>
#include <stdbool.h>

/*
 * Given a pointer to allocated space for a queue, set all initial values
 * of the queue.
 */
void init_queue(queue_t *q)
{
	q->start = 0;
	q->end = 0;
	q->num_items = 0;
}


/*
 * Given a pointer to a queue, an output socket descriptor, and a string,
 * enqueue the string for sending to client at the other end of the
 * socket. Return true on success, and false on failure.
 *
 * Client connection handlers or the host itself should use this function.
 */
bool enqueue(queue_t *q, int fd, char *s)
{
	if (q->num_items == QUEUE_SIZE) return false;
	
	queue_item_t *new = malloc(sizeof(queue_item_t));

	new->str = s;
	new->dest_fd = fd;

	q->entries[q->end] = new;

	q->end = (q->end + 1) % QUEUE_SIZE;

	q->num_items++;

	return true;
}

