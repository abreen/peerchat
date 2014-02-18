/*
 * peerchat.c - A simple chat room program that uses UNIX sockets
 */

#include "host.h"		// for host-like functionality
#include "client.h"		// for client-like functionality
#include "peer.h"		// for peer_t struct

#include <stdio.h>
#include <errno.h>

#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>		// for handling signals (SIGINT, etc.)

#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>

#define UNIX_PATH_MAX		108
#define	PEERCHAT_VERSION	"0.2"

#include "config.h"		// get config file options

enum role { HOST, CLIENT };

enum role this_role;		// set accordingly before listening/connecting

struct program_options {
	char *room;
	bool verbose;
	bool list_mode;
	bool help_mode;
};

struct program_options options;

char *socket_path;		// path to socket handling/connecting to


void print_error(char *str)
{
	fprintf(stderr, "peerchat: ");
	fprintf(stderr, str);
	fprintf(stderr, "\n");
}


static void handle_signal(int signal)
{
	if (this_role == HOST) {
		print_error("got SIGINT as host");

		if (unlink(socket_path) < 0) {
			perror("peerchat: could not remove UNIX socket");
		}

	} else {
		print_error("got SIGINT as client");
	}

	print_error("exiting...");

	exit(1);
}


static bool print_list()
{
	DIR *dir = opendir(SOCKETS_DIRECTORY);
	struct dirent *dirent_ptr;
	int i = 0;

	if (!dir) return false;

	while ((dirent_ptr = readdir(dir)) != NULL) {
		if (dirent_ptr->d_type != DT_SOCK)
			continue;

		printf("%s\n", dirent_ptr->d_name);
		i++;
	}

	closedir(dir);

	if (i == 0) printf("No active rooms.\n");

	return true;
}


static void parse_options(int count, char *args[])
{
	int i;

	for (i = 1; i < count; i++) {
		if (strcmp(args[i], "-v") == 0 ||
		    strcmp(args[i], "--verbose") == 0) {

			options.verbose = true;

		} else if (strcmp(args[i], "-l") == 0 ||
		           strcmp(args[i], "--list") == 0) {

			options.list_mode = true;

		} else if (strcmp(args[i], "-h") == 0 ||
		           strcmp(args[i], "--help") == 0) {

			options.help_mode = true;

		} else {

			options.room = args[i];
		}
	}

	if (options.verbose) {
		printf("peerchat version ");
		printf(PEERCHAT_VERSION);
		printf("\n");

		printf("verbose output is on\n");
		printf("room: %s\n", options.room);
		printf(options.list_mode ? "list mode\n" : "no list mode\n");
		printf(options.help_mode ? "help mode\n" : "no help mode\n");
	}
}


static inline bool check_options()
{
	if (!options.list_mode &&
	    !options.help_mode &&
	    options.room == NULL) {
		print_error("specify a room to join");
		return false;
	}

	return true;
}


static inline void print_usage()
{
	printf("peerchat version ");
	printf(PEERCHAT_VERSION);
	printf("\n");

	printf("usage: peerchat [(-v|--verbose)] <options>\n\n");

	printf("where <options> can be one of the following:\n");
	printf("\troom_name\tto join a room, or start one if it doesn't exist\n");
	printf("\t(-l|--list)\tto list all active rooms\n");
	printf("\t(-h|--help)\tto print this usage message\n\n");

	printf("then, while in a chat room, use:\n");
	printf("\t!exit\t\tto leave the room\n");
	printf("\t!who\t\tto print list of other peers in room\n");
}


int main(int argc, char *argv[])
{
	parse_options(argc, argv);

	if (!check_options())
		return EXIT_FAILURE;
	
	if (options.help_mode) {
		print_usage();
		return EXIT_SUCCESS;
	}

	if (options.list_mode) {
		if (!print_list()) {
			perror("peerchat: listing rooms failed");
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	// set up signal handler
	
	struct sigaction sig_handler;
	sig_handler.sa_handler = handle_signal;
	sigemptyset(&(sig_handler.sa_mask));
	sig_handler.sa_flags = 0;

	sigaction(SIGINT, &sig_handler, NULL);

	peer_t me;

	me.username = getlogin();
	me.fd = 1;			// 1 == stdout file descriptor

	// attempt to get a socket

	struct sockaddr_un address;
	int socket_fd;

	socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);

	if (socket_fd < 0) {
		perror("peerchat: getting socket failed");
		return EXIT_FAILURE;
	}

	// clear memory for UNIX address struct
	memset(&address, 0, sizeof(struct sockaddr_un));

	size_t dir_length = strlen(SOCKETS_DIRECTORY);
	size_t room_name_len = strlen(options.room);

	socket_path = malloc(dir_length + room_name_len + 1);

	strcpy(socket_path, SOCKETS_DIRECTORY);
	strcat(socket_path, options.room);

	// path_name now has the full path to the socket we want

	address.sun_family = AF_UNIX;
	snprintf(address.sun_path, UNIX_PATH_MAX, socket_path);

	int bind_ret = bind(socket_fd, (struct sockaddr *) &address,
				sizeof(struct sockaddr_un));

	if (bind_ret == 0) {
		/* 
		 * We were able to bind. This means we are the first peer
		 * and should set up the room as the host, getting ready
		 * to handle connections from other peers.
		 */

		this_role = HOST;
		
		printf("You seem to be the first peer.\n");
		printf("Opening chat room...\n");

		return setup_host(socket_fd, address, &me);

	} else {
		/*
		 * We were not able to bind. If the address is already
		 * in use, we should infer that there is already a host
		 * managing this chat room and attempt to join.
		 */

		if (errno == EADDRINUSE) {
		
			this_role = CLIENT;

			printf("Joining open chat room...\n");

			return setup_client(socket_fd, address, &me);
		} else {
			perror("peerchat: bind failed, address not in use");
			return EXIT_FAILURE;
		}
	}
}

