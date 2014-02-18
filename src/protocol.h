/*
 * protocol.h - description of client/host communication
 */


#define		SPECIAL_START	0x00
#define		CHAT_START	0xFF
#define		MSG_END		0x01


/*
 * Bytes representing special messages.
 */
#define		CONNECT		0x12
#define		DISCONNECT	0x13
#define		MIGRATE		0x14
#define		WHO		0x15

