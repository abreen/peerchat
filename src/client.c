/*
 * client.c - functions implementing client features
 */

#include "client.h"
#include "peer.h"
#include "protocol.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

#define	BUFFER_SIZE	256

int setup_client(int fd, struct sockaddr_un address, peer_t *me)
{
	int n;
	char buf[BUFFER_SIZE];

	if (connect(fd, (struct sockaddr *) &address,
		sizeof(struct sockaddr_un)) != 0)
	{
		perror("peerchat: failed to connect to host");
		return EXIT_FAILURE;
	}

	/*
	 * Send the CONNECT special message.
	 */
	n = strlen(me->username);

	buf[0] = SPECIAL_START;
	buf[1] = CONNECT;

	memcpy((buf + 2), me->username, n);

	buf[n + 2] = MSG_END;

	write(fd, buf, n + 3);

	printf("sent CONNECT, now sending DISCONNECT message\n");

	/*
	 * Send the DISCONNECT special message.
	 */
	buf[0] = SPECIAL_START;
	buf[1] = DISCONNECT;
	buf[2] = MSG_END;

	write(fd, buf, 3);

	close(fd);

	return EXIT_SUCCESS;
}
