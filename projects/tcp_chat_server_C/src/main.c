#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "server.h"

int main(void)
{
    st_Server server;
    int result = EXIT_FAILURE;

    if (server_init(&server, 12345) < 0)
    {
        fprintf(stderr, "Failed to initialize server.\n");
        return EXIT_FAILURE;
    }

    if (server_start(&server) < 0)
    {
        perror("server_start");
        goto cleanup;
    }

    int client_socket = server_accept_client(&server);
    if (client_socket < 0)
    {
        perror("server_accept_client");
        goto cleanup;
    }

    if (server_add_client(&server, client_socket) < 0)
    {
        /* Registration failed: the caller still owns this socket. */
        close(client_socket);
        fprintf(stderr, "Failed to register client.\n");
        goto cleanup;
    }

    /* Registration transferred ownership to the server. */
    server_echo_client(server.clients_list[0]);
    result = EXIT_SUCCESS;

cleanup:
    server_cleanup(&server);
    return result;
}
