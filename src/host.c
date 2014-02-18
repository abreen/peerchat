/*
 * host.c
 */

#include "host.h"
#include "peer.h"		// for peer list functions
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>

#define	BUFFER_SIZE	256	// connection thread incoming data buffer

size_t process_connect(char *, size_t, char **);

/*
 * fork()'d connection handlers execute this function, which reads raw
 * data from the connected client, and updates the peer_t object in the
 * peer list when neccessary. This function does not add nor remove the
 * peer_t object from the peer list --- the peer should be added before
 * this function is called and removed after it returns.
 */
bool handle_connection(int fd, peer_t *this_peer)
{
	int num_bytes;
	size_t start, end;
	char buf[BUFFER_SIZE];
	char *ptr;

	num_bytes = read(fd, buf, BUFFER_SIZE);

	if (num_bytes < 0) {
		perror("peerchat: failed reading new client");
		close(fd);
		return false;
	}

	if (num_bytes == 0) {
		perror("peerchat: got no bytes from new client");
		close(fd);
		return false;
	}

	/*
	 * Expect the first message to be the CONNECT message. If not,
	 * close the connection now.
	 */
	end = process_connect(buf, BUFFER_SIZE, &this_peer->username);

	if (end == -1) {
		print_error("got invalid first message from client");
		close(fd);
		return false;
	}

	this_peer->fd = fd;

	// TODO queue a connection message to all peers
	printf("%s connected.\n", this_peer->username);

	/*
	 * There may be one or more whole messages still in the buffer
	 * (thanks to lack of framing in TCP), so we will attempt to
	 * process them first before calling read() again. Note that
	 * the call to process_connect() returned the index of the final
	 * byte in the previous message.
	 */
	do {


	} while ((num_bytes = read(fd, buf, BUFFER_SIZE)) != 0);

	/*
	 * If we get here, we exited the loop on the while condition.
	 * This means we did not get a disconnect message, attempted a
	 * read, and got end of file.
	 */

	// TODO queue a lost connection message to all peers
	printf("Lost connection to peer %s.\n", this_peer->username);

done:
	free(this_peer->username);
	
	close(fd);

	return true;
}


/*
 * Assume the role of host and start the server, listening for connections
 * and coordinating the sending of messages to every peer.
 *
 * The peer_t pointer here is passed in from main() in peerchat.c, so that
 * the host itself can be added to the peer list.
 */
int setup_host(int fd, struct sockaddr_un address, peer_t *host)
{
	int incoming_fd;
	socklen_t address_len = sizeof(address);

	peer_list_t plist;
	init_peer_list(&plist);

	if (listen(fd, 5) != 0) {
		perror("peerchat: listen as host failed");
		return EXIT_FAILURE;
	}

	printf("Chat room open.\n");

	add_peer(&plist, host);

	while ((incoming_fd = accept(fd, (struct sockaddr *) &address,
		&address_len)) > -1)
	{
		pid_t child = fork();

		if (child == 0) {
			peer_t *this_peer = malloc(sizeof(peer_t));

			add_peer(&plist, this_peer);

			handle_connection(incoming_fd, this_peer);

			remove_peer(&plist, this_peer);

			free(this_peer);
		}
	}
}


/*
 * Given a pointer to a string of bytes, try to interpret it as a CONNECT
 * special message and change the "username" pointer to point to an area on
 * the heap containing the peer's username. Return -1 if the message is
 * malformed, or return the index of the MSG_END byte of the CONNECT message.
 * This function is atomic in the sense that, if the message is
 * malformed, the char pointer will not be changed.
 */
size_t process_connect(char *buf, size_t max, char **username)
{
	size_t start = 0, end = 2;
	size_t len;
	char *ptr;

	while (buf[end] != MSG_END) {
		if (end + 1 == max)
			return -1;

		end++;
	}

	/*
	 * The client tried to connect with an empty username.
	 */
	if (end == 2)
		return -1;

	/*
	 * The first byte of the message is not SPECIAL_MSG_START.
	 */
	if (buf[start] != SPECIAL_START)
		return -1;
	
	start++;

	/*
	 * The second byte of the message is not CONNECT.
	 */
	if (buf[start] != CONNECT)
		return -1;
	
	start++;

	/*
	 * Now copy the string from the buffer and set the pointer.
	 */
	len = end - start;
	ptr = malloc(len + 1);
	memcpy(ptr, (buf + start), len);
	ptr[len] = '\0';

	*username = ptr;

	return end;
}
