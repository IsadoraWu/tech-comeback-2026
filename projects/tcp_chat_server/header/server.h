#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>

#include "client.h"
#include "protocol.h"

typedef struct
{
    int server_socket;
    int port;
    st_Client *clients;
    int num_clients;
    int max_clients;
    pthread_mutex_t clients_mutex;
} st_Server;

int server_init(st_Server *server, int port);
void server_cleanup(st_Server *server);
int server_start(st_Server *server);
void server_stop(st_Server *server);
int server_accept_client(st_Server *server);
int server_add_client(st_Server *server, int client_socket);
int server_remove_client(st_Server *server, int client_socket);
int server_broadcast(st_Server *server, const char *message, int sender_socket);
int server_send_to_client(st_Server *server, int client_socket, const char *message);
void *server_client_handler(void *arg);

#endif