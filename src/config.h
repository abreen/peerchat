/*
 * config.h - Configuration of the peerchat binary
 *
 * Note: a default configuration is included with the source code
 *       (located in the "config.h.default" file) whose values have been
 *       reproduced here to start. If you decide you want to undo changes
 *       you may have made here, just copy that file.
 */

/*
 * The directory (on the shared system) to which peerchat looks for active
 * rooms. Correspondingly it is the directory in which a socket will be
 * created by the first peer in a chat room.
 */
#define	SOCKETS_DIRECTORY	"/home/ugrad/abreen/share/peerchat/rooms/"


/*
 * When acting as a host, deny any attempts to join the room if the number
 * of peers currently in the room equals MAX_PEERS.
 */
#define	MAX_PEERS	64


/*
 * Hosts whose binaries have this set to zero (false) will not attempt to
 * migrate the chat room to another host when it exits. Instead, all other
 * peers will be disconnected without warning. This feature is set to 0
 * while is not yet implemented.
 */
#define	MIGRATION_ENABLED	0


/*
 * This feature is not yet implemented.
 */
#define	USE_COLOR		0

