#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "protocol.h"

int protocol_send(int socket_fd, const char *message)
{
    size_t message_length = strlen(message);
    if (message_length > MAX_MESSAGE_LENGTH)
    {
        return -1; // Message too long
    }

    size_t total_sent = 0;
    while (total_sent < message_length)
    {
        ssize_t bytes_sent = send(socket_fd, message + total_sent, message_length - total_sent, 0);
        if (bytes_sent < 0)
        {
            if (errno == EINTR)
                continue; // interrupted, retry
#ifdef EAGAIN
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue; // would block, retry
#endif
            return -1; // real error
        }

        total_sent += (size_t)bytes_sent;
    }

    return 0; // Success
}
int protocol_recv(int socket_fd, char *buffer, size_t buffer_size)
{
    ssize_t bytes_received = recv(socket_fd, buffer, buffer_size - 1, 0);
    if (bytes_received < 0)
    {
        return -1; // Error receiving message
    }

    buffer[bytes_received] = '\0';
    return bytes_received; // Success
}
char *protocol_format_text(const char *username, const char *message)
{
    // Implementation for formatting text messages
    char *buffer = malloc(MAX_MESSAGE_LENGTH);
    if (!buffer) 
    {
        return NULL;
    }

    snprintf(buffer, MAX_MESSAGE_LENGTH, "[%s]: %s", username, message);

    return buffer;
}
char *protocol_format_system(const char *message)
{
    // Implementation for formatting system messages
    char *buffer = malloc(MAX_MESSAGE_LENGTH);
    if (!buffer)
    {
        return NULL;
    }  

    snprintf(buffer, MAX_MESSAGE_LENGTH, "[SYSTEM]: %s", message);
    return buffer;
}
char *protocol_format_error(const char *message)
{
    // Implementation for formatting error messages
    char *buffer = malloc(MAX_MESSAGE_LENGTH);
    if (!buffer)
    {
        return NULL;
    }  

    snprintf(buffer, MAX_MESSAGE_LENGTH, "[ERROR]: %s", message);
    return buffer;
}