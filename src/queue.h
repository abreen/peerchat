/*
 * queue.h - defs. for a queue of outgoing strings, for host
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>		// for size_t
#include <stdbool.h>

#define	QUEUE_SIZE	64

typedef struct queue_item {
	int	dest_fd;
	char	*str;
} queue_item_t;

typedef struct queue {
	queue_item_t	*entries[QUEUE_SIZE];
	size_t		num_items;
	size_t		start;			// index of next item to go
	size_t		end;			// index of next free slot
} queue_t;

void init_queue(queue_t *);
bool enqueue(queue_t *, int, char *);

/*
 * Note: the dequeue function is defined locally in dispatcher.c --- the
 * dispatcher should be the only entity allowed to remove items from the
 * queue.
 */

#endif
