#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "protocol.h"

static int protocol_send_all(int socket_fd, const void *data, size_t length)
{
    const unsigned char *buffer = (const unsigned char *)data;
    size_t total_sent = 0;

    while (total_sent < length)
    {
        ssize_t bytes_sent = send(socket_fd, buffer + total_sent, length - total_sent, 0);
        if (bytes_sent < 0)
        {
            if (errno == EINTR)
                continue;
#ifdef EAGAIN
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
#endif
            return -1;
        }

        if (bytes_sent == 0)
        {
            return -1;
        }

        total_sent += (size_t)bytes_sent;
    }

    return 0;
}

static int protocol_recv_all(int socket_fd, void *data, size_t length)
{
    unsigned char *buffer = (unsigned char *)data;
    size_t total_received = 0;

    while (total_received < length)
    {
        ssize_t bytes_received = recv(socket_fd, buffer + total_received, length - total_received, 0);
        if (bytes_received < 0)
        {
            if (errno == EINTR)
                continue;
#ifdef EAGAIN
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
#endif
            return -1;
        }

        if (bytes_received == 0)
        {
            return -1;
        }

        total_received += (size_t)bytes_received;
    }

    return 0;
}

int protocol_send(int socket_fd, const char *message)
{
    if (message == NULL)
    {
        return -1;
    }

    size_t message_length = strlen(message);
    if (message_length > MAX_MESSAGE_LENGTH)
    {
        return -1;
    }

    uint32_t header = htonl((uint32_t)message_length);
    if (protocol_send_all(socket_fd, &header, sizeof(header)) < 0)
    {
        return -1;
    }

    if (protocol_send_all(socket_fd, message, message_length) < 0)
    {
        return -1;
    }

    return 0;
}

int protocol_recv(int socket_fd, char *buffer, size_t buffer_size)
{
    uint32_t header = 0;
    size_t payload_length = 0;

    if (buffer == NULL || buffer_size == 0)
    {
        return -1;
    }

    if (protocol_recv_all(socket_fd, &header, sizeof(header)) < 0)
    {
        return -1;
    }

    payload_length = (size_t)ntohl(header);
    if (payload_length >= buffer_size || payload_length > MAX_MESSAGE_LENGTH)
    {
        return -1;
    }

    if (protocol_recv_all(socket_fd, buffer, payload_length) < 0)
    {
        return -1;
    }

    buffer[payload_length] = '\0';
    return (int)payload_length;
}
int protocol_format_text(char *buffer, size_t buffer_size, const char *username, const char *message)
{
    if (buffer == NULL || buffer_size == 0 || username == NULL || message == NULL)
    {
        return -1;
    }

    int written = snprintf(buffer, buffer_size, "[%s]: %s", username, message);
    if (written < 0 || (size_t)written >= buffer_size)
    {
        return -1;
    }

    return 0;
}
int protocol_format_system(char *buffer, size_t buffer_size, const char *message)
{
    if (buffer == NULL || buffer_size == 0 || message == NULL)
    {
        return -1;
    }

    int written = snprintf(buffer, buffer_size, "[SYSTEM]: %s", message);
    if (written < 0 || (size_t)written >= buffer_size)
    {
        return -1;
    }

    return 0;
}
int protocol_format_error(char *buffer, size_t buffer_size, const char *message)
{
    if (buffer == NULL || buffer_size == 0 || message == NULL)
    {
        return -1;
    }

    int written = snprintf(buffer, buffer_size, "[ERROR]: %s", message);
    if (written < 0 || (size_t)written >= buffer_size)
    {
        return -1;
    }

    return 0;
}