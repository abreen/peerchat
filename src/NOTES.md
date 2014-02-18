Notes, known bugs, and to-do list
=================================

To-do list
----------

* Implement migration


Known bugs
----------

* Peers in host mode (or migrated-host mode, for that matter) will not
  initialize migration when interrupted (e.g, SIGINT) --- is this a bug
  or a feature?


Notes on the source code
------------------------

* The peerchat "protocol" (if you want to call it that) is simple. For
  the purposes of these notes, let a "message" be defined as a sequence
  of bytes written over a socket descriptor from a client to the host
  or from the host to any client. Then:

  - A message whose first byte is 0x00 is a special message.
  - A message whose first byte is 0xFF is a chat message.
  - All messages are terminated with 0x01.

  - Chat messages sent contain 0xFF and then a sequence of ASCII bytes
    intended for sending to other peers in the room. (The host will
    automatically prepend the username of the sending peer before resending
    to the other peers.)

    (full example: 0xFF 0x48 0x69 0x21 0x01)
                          H    i    ! 

  - Special messages contain 0x00 and then a variable number of bytes
    describing a special action or request, and may contain ASCII data
    as well. As with a chat message, a special message ends with
    a 0x01 byte.

  - The bytes following 0xFF in a special message are summarized by
    the following table:

  	0x12		CONNECT
			A client sends this special message after a
			connection has been made. The host can send a
			notification string to all other connected
			peers (e.g., "abreen joined the room.").

			The bytes after this byte are reserved for
			a null-terminated string containing the username
			of the user connecting.

	                full example: 0x00 0x12 0x61 0x62 0x01
			(i.e., the user "ab" is connecting)
	
	0x13		DISCONNECT
			A client sends this special message when it
			declares its intent to disconnect. The host can
			send a notification string to all other
			connected peers that the peer has disconnected
			(e.g., "abreen left the room.").

			full example: 0x00 0x13 0x01
	
	0x14		MIGRATE
			A host sends this special message to a client
			when the client has been selected to be
			the new host.

			full example: 0x00 0x14 0x01
	
	0x15		WHO
			A client sends this special message to the host
			when it wants a list of users in the room. The
			host then sends a standard (printable) message to
			the client containing the list.

			full example: 0x00 0x15 0x01


* There are three possible running states for the peerchat program: host mode,
  which the program enters when it opens a room for the first time; client
  mode, which the program enters when it joins a room already hosted; and
  migrated-host mode, which the program enters when it is asked to assume
  host control of a room when the original host needs to exit.

    +--> host mode   ----->     exit
    |    client mode ----->     exit
    |       |
    +-------+
    (migrated-host mode)

* A program in host (or migrated-host) mode will not be demoted to client
  mode.

      main --> setup_client --> accept_migrate --> setup_host --> exit
      main --> setup_host --> request_migrate --> exit
      main --> setup_client --> exit
