#include "server.h"

int server_init(st_Server *server, int port)
{
    if (server == NULL || port <= 0)
    {
        return -1;
    }

    server->server_socket = -1;
    server->port = port;
    server->num_clients = 0;
    server->max_clients = MAX_CLIENTS;
    server->clients_list = malloc(sizeof(st_Client) * server->max_clients);
    if (server->clients == NULL)
    {
        return -1;
    }

    pthread_mutex_init(&server->clients_mutex, NULL);

    return 0;
}
void server_cleanup(st_Server *server)
{
    server_stop(server);

    if (server == NULL)
    {
        return;
    }

    pthread_mutex_destroy(&server->clients_mutex);

    if (server->clients_list != NULL)
    {
        free(server->clients_list);
        server->clients_list = NULL;
    }
}
int server_start(st_Server *server)
{
    if (server == NULL)
    {
        return -1;
    }

    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_socket < 0)
    {
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server->port);

    if (bind(server->server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        close(server->server_socket);
        return -1;
    }

    if (listen(server->server_socket, server->max_clients) < 0)
    {
        close(server->server_socket);
        return -1;
    }

    return 0;
}
void server_stop(st_Server *server)
{
    if (server == NULL)
    {
        return;
    }

    if (server->server_socket >= 0)
    {
        close(server->server_socket);
        server->server_socket = -1;
    }
}
int server_accept_client(st_Server *server)
{
    if (server == NULL)
    {
        return -1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(server->server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0)
    {
        return -1;
    }
    else
    {
        return client_socket;
    }
}
int server_add_client(st_Server *server, int client_socket)
{
    if (server == NULL || client_socket < 0)
    {
        return -1;
    }

    pthread_mutex_lock(&server->clients_mutex);

    if (server->num_clients >= server->max_clients)
    {
        pthread_mutex_unlock(&server->clients_mutex);
        return -1;
    }

    st_Client *new_client = &server->clients_list[server->num_clients];
    new_client->socket_fd = client_socket;
    new_client->next = NULL;

    server->num_clients++;

    pthread_mutex_unlock(&server->clients_mutex);

    return 0;
}
int server_remove_client(st_Server *server, int client_socket);
int server_broadcast(st_Server *server, const char *message, int sender_socket);
int server_send_to_client(st_Server *server, int client_socket, const char *message);
void *server_client_handler(void *arg);