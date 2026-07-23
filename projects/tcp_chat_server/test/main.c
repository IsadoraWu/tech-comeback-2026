#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "protocol.h"
#include "client.h"

int main(void)
{
    char buffer_hola[MAX_MESSAGE_LENGTH] = {"Hola"};
    char buffer_user1[MAX_USERNAME_LENGTH] = {"Isadora"};
    char buffer_user2[MAX_USERNAME_LENGTH] = {"Miday"};
    char welcome_buffer[MAX_MESSAGE_LENGTH] = {"\0"};
    char send_buffer[MAX_MESSAGE_LENGTH] = {"\0"};
    char recv_buffer[MAX_MESSAGE_LENGTH] = {"\0"};
    int socket_fd1 = client_connect_to_server("127.0.0.1", 12345);
    int socket_fd2 = client_connect_to_server("127.0.0.1", 12345);

    if(socket_fd1 < 0 || socket_fd2 < 0)
    {
        printf("Failed to connect to server. Exiting...\n");
        return -1;
    }
    else
    {
        printf("Connected to server successfully. Continuing...\n");
    }

    st_Client *client1 = client_create(socket_fd1, buffer_user1);
    st_Client *client2 = client_create(socket_fd2, buffer_user2);
    if(client1 == NULL || client2 == NULL)
    {
        printf("Failed to create clients. Exiting...\n");
        return -1;
    }
    else
    {
        snprintf(welcome_buffer, sizeof(welcome_buffer), "Welcome to the chat, %s!", buffer_user1);
        protocol_format_system(send_buffer, sizeof(send_buffer), welcome_buffer); // Format welcome message for client1
        client_send_message(client1, send_buffer);
        client_recv_message(client1, recv_buffer, sizeof(recv_buffer));
        printf("%s\n", recv_buffer);

        snprintf(welcome_buffer, sizeof(welcome_buffer), "Welcome to the chat, %s!", buffer_user2);
        protocol_format_system(send_buffer, sizeof(send_buffer), welcome_buffer); // Format welcome message for client2
        client_send_message(client2, send_buffer);
        client_recv_message(client2, recv_buffer, sizeof(recv_buffer));
        printf("%s\n", recv_buffer);
    }

    protocol_format_text(send_buffer, sizeof(send_buffer), buffer_user1, buffer_hola);
    client_send_message(client1, send_buffer);
    client_recv_message(client2, recv_buffer, sizeof(recv_buffer));
    printf("%s\n", recv_buffer);

    if(client_destroy(client1) != 0 || client_destroy(client2) != 0)
    {
        printf("Failed to destroy clients. Exiting...\n");
        return -1;
    }
    else
    {
        printf("Clients destroyed successfully. Exiting...\n");
    }

/*    int sockets[2];

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
*/
    return 0;
}