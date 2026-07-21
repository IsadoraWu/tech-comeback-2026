#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "protocol.h"

int main(void)
{
    char buffer_empty[MAX_MESSAGE_LENGTH] = {"\0"};
    char buffer_hola[MAX_MESSAGE_LENGTH] = {"Hola"};
    char buffer_user[MAX_MESSAGE_LENGTH] = {"Isadora"};
    char recv_buffer[MAX_MESSAGE_LENGTH] = {"\0"};

    int sockets[2];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0)
    {
        perror("socketpair");
        return 1;
    }

    if(protocol_send(sockets[0], buffer_empty) != 0)
    {
        printf("Send failed for empty message. Continuing...\n");
    }
    else
    {
        printf("Empty message sent successfully.\n");
        int received = protocol_recv(sockets[1], recv_buffer, sizeof(recv_buffer));
        if (received < 0)
        {
            fprintf(stderr, "protocol_recv failed\n");
        }
        else
        {
            printf("Received: %s\n", recv_buffer);
        }
    }

    if(protocol_send(sockets[0], buffer_hola) != 0)
    {
        printf("Send failed for 'Hola' message. Continuing...\n");
    }
    else
    {
        printf("'Hola' message sent successfully.\n");
        int received = protocol_recv(sockets[1], recv_buffer, sizeof(recv_buffer));
        if (received < 0)
        {
            fprintf(stderr, "protocol_recv failed\n");
        }
        else
        {
            printf("Received: %s\n", recv_buffer);
        }
    }

    if(protocol_format_text(recv_buffer, sizeof(recv_buffer), buffer_user, buffer_hola) != 0)
    {
        printf("Format text failed. Continuing...\n");
    }
    else
    {
        printf("Formatted text: %s\n", recv_buffer);
    }

    close(sockets[0]);
    close(sockets[1]);

    return 0;
}