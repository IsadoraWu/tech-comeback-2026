#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "client.h"
#include "protocol.h"

st_Client *client_create(int socket_fd, const char *username)
{
    if(username == NULL)
    {
        return NULL;
    }
    st_Client *new_client = (st_Client *)malloc(sizeof(*new_client));

    if(new_client == NULL)
    {
        return NULL;
    }
    new_client->socket_fd = socket_fd;
    strncpy(new_client->username, username, MAX_USERNAME_LENGTH - 1);
    new_client->username[MAX_USERNAME_LENGTH - 1] = '\0';
    new_client->connected = 1;
    new_client->thread = 0;

    return new_client;
}
int client_destroy(st_Client *client)
{
    if (client != NULL)
    {
        if (client->socket_fd >= 0)
        {
            close(client->socket_fd);
            client->socket_fd = -1;
        }
        free(client);
        return 0;
    }
    return -1;
}
int client_set_username(st_Client *client, const char *username)
{
    if (client != NULL && username != NULL)
    {
        strncpy(client->username, username, MAX_USERNAME_LENGTH - 1);
        client->username[MAX_USERNAME_LENGTH - 1] = '\0';
        return 0;
    }
    return -1;
}
int client_send_message(st_Client *client, const char *message)
{
    if (client != NULL && message != NULL)
    {
        return protocol_send(client->socket_fd, message);
    }
    return -1;
}
int client_recv_message(st_Client *client, char *buffer, size_t buffer_size)
{
    if (client != NULL && buffer != NULL && buffer_size > 0)
    {
        return protocol_recv(client->socket_fd, buffer, buffer_size);
    }
    return -1;
}
int client_connect_to_server(const char *host, int port)
{
    int sockfd;
    struct sockaddr_in server_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        close(sockfd);
        return -1;
    }

    return sockfd;
}
void *client_receive_loop(void *arg)
{
    if (arg == NULL)
    {
        return NULL;
    }
    st_Client *client = (st_Client *)arg;
    char buffer[MAX_MESSAGE_LENGTH];

    while (client->connected)
    {
        int bytes_received = client_recv_message(client, buffer, sizeof(buffer));
        if (bytes_received <= 0)
        {
            printf("Disconnected from server.\n");
            client->connected = 0;
            break;
        }
        printf("%s\n", buffer);
    }

    return NULL;
}