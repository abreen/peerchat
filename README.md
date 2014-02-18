peerchat
========

`peerchat` is a small C program that uses UNIX sockets to set up lightweight
chat rooms. `peerchat` is designed for use on a multi-user Linux system, in
which any user can open sockets in an agreed-upon directory, to which
other peers running `peerchat` can look to connect.

If there are no users chatting, the first user actually acts like a server
for the chatroom and opens a socket. Each connected user maintains a list
of peers, so that if the host disconnects, the socket is re-opened by
another peer and those connections are migrated to the new host.

This configuration is ideal for a multi-user system on which peers want to
communicate in chat rooms but do not have superuser access and do not want
to waste system resources if there are no chatters (i.e., csa{2,3}.bu.edu).


## Usage

You can copy the source into your home directory, compile it, and run it
with the name of the chat room you wish to join. For example,

    cp -r /home/ugrad/abreen/share/peerchat .
    cd peerchat/src
    make
    ./peerchat general

The `peerchat` program will inform you if you are the first user in the
chatroom (i.e., if you are the first host). As peers join, they become
eligible to be the new host, such that if a host disconnects while the
room is not empty, one of the remaining peers will be selected to host.

All possible command line arguments are summarized by the following:

    peerchat [-v] room_name
    peerchat [-v] -l

Specifying `-v` (verbose) will induce `peerchat` to narrate connection and
migration in detail, as well as provide more descriptive reasons for program
failures. The first form, in which only a room name is provided, will cause
`peerchat` to attempt to join or start the specified chat room. The second
form will cause `peerchat` to return all active chat rooms (i.e., all chat
rooms in which there is currently at least one peer).


### Commands

`peerchat` will prepend your messages with your login username. Simply type
a message and press return to send it to the other peers in the chat room.

Commands begin with an exclamation mark and are interpreted by the `peerchat`
process you are running. In the case of the `!who` command, a request is 
made to the host to send back a string containing a list of users in the
chat.

    !exit               exits the chat room
    !who                prints list of peers in chat room

Issuing `!exit` when you are the host automatically initiates migration.
When responsibility is transferred, `peerchat` will exit.


## Rooms

Although you can use `peerchat` in any way you wish, I propose that we use
the following rooms at BU:

    general             general discussion
    cs\d\d\d            discussion specific to a CS course (e.g., cs552)
    research            discussion about undergrad research at BU
    gossip              [redacted]


## Configuration

The lack of command line arguments available for use with `peerchat` was
not an oversight (cf. "the UNIX philosopy"), but customization is still
allowed through the modification of the `config.h` file (included with the
source code). Various macros can be altered there, and the binary
can be re-compiled with different options.


## Author

`peerchat` was written by Alexander Breen (abreen@bu.edu). This code is
provided to you with no license, and the author expressly abandons any
legal responsibility for misuse of the code or damages resulting from it.

### Notice

In the `config.h` file, the macro `SOCKET_DIRECTORY` is defined as
"/home/ugrad/abreen/share/peerchat/rooms". This directory is located in
my home directory and I volunteer it for use with `peerchat` on BU's
servers. This constant may be changed to create future `peerchat`
communities on the same multi-user system.

