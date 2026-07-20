#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>

#define MAX_MESSAGE_LENGTH 512

typedef enum
{
    MSG_TEXT = 1, // normal text message
    MSG_SYSTEM,   // system message (e.g., user joined/left)
    MSG_JOIN,   // user joined the chat
    MSG_LEAVE,  // user left the chat
    MSG_NICK,   // user changed nickname
    MSG_ERROR   // error message
} en_MessageType;

int protocol_send(int socket_fd, const char *message);
int protocol_recv(int socket_fd, char *buffer, size_t buffer_size);
char *protocol_format_text(const char *username, const char *message);
char *protocol_format_system(const char *message);
char *protocol_format_error(const char *message);

#endif