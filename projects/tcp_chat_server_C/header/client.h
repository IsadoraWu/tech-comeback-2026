#ifndef CLIENT_H
#define CLIENT_H

#include <stddef.h>
#include <pthread.h>

#define MAX_USERNAME_LENGTH 32

typedef struct
{
    int socket_fd;
    char username[MAX_USERNAME_LENGTH];
    int connected;  // 0 for disconnected, 1 for connected
    pthread_t thread;   // Thread for receiving messages
} st_Client;

st_Client *client_create(int socket_fd, const char *username);
int client_destroy(st_Client *client);
int client_set_username(st_Client *client, const char *username);
int client_send_message(st_Client *client, const char *message);
int client_recv_message(st_Client *client, char *buffer, size_t buffer_size);
int client_connect_to_server(const char *host, int port);
void *client_receive_loop(void *arg);

#endif