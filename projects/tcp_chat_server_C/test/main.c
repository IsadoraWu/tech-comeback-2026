#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "client.h"
#include "protocol.h"

#define MAX_COMMAND_LENGTH 512

static int echo_segment(st_Client *client, char *buffer, size_t length)
{
    char reply[MAX_MESSAGE_LENGTH + 1];
    buffer[length] = '\0';

    if (client_send_message(client, buffer) < 0)
    {
        return -1;
    }
    int received = client_recv_message(client, reply, sizeof(reply));
    if (received < 0)
    {
        return -1;
    }

    if (fwrite(reply, 1, (size_t)received, stdout) != (size_t)received ||
        fflush(stdout) == EOF)
    {
        return -1;
    }
    return 0;
}

static int run_message_loop(st_Client *client)
{
    char buffer[MAX_MESSAGE_LENGTH + 1];
    size_t length = 0;
    char command_buffer[MAX_COMMAND_LENGTH];
    size_t command_length = 0;
    bool command_overflow = false;
    bool line_start = true;
    bool command = false;
    int ch;

    for (;;)
    {
        ch = fgetc(stdin);
        if (ch == EOF && ferror(stdin))
        {
            return -1;
        }

        if (ch == '\n' || ch == EOF)
        {
            if (command && !command_overflow && command_length == 2 &&
                command_buffer[0] == '/' && command_buffer[1] == 'q')
            {
                return 0;
            }
            if (!line_start && !command)
            {
                if (length > 0 && echo_segment(client, buffer, length) < 0)
                {
                    return -1;
                }
                if (putchar('\n') == EOF || fflush(stdout) == EOF)
                {
                    return -1;
                }
            }
            if (ch == EOF)
            {
                return 0;
            }
            length = 0;
            line_start = true;
            command = false;
            command_length = 0;
            command_overflow = false;
            continue;
        }

        if (line_start)
        {
            command = (ch == '/');
            line_start = false;
        }
        /* Never send commands; discard excess bytes until the line ends. */
        if (command)
        {
            if (command_length < MAX_COMMAND_LENGTH)
            {
                command_buffer[command_length++] = (char)ch;
            }
            else
            {
                command_overflow = true;
            }
            continue;
        }

        buffer[length++] = (char)ch;
        if (length == MAX_MESSAGE_LENGTH)
        {
            if (echo_segment(client, buffer, length) < 0)
            {
                return -1;
            }
            length = 0;
        }
    }
}

int main(void)
{
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        printf("Unexpected error\n");
        return EXIT_FAILURE;
    }

    int socket_fd = client_connect_to_server("127.0.0.1", 12345);
    if (socket_fd < 0)
    {
        printf("Connected Failed...\n");
        return EXIT_FAILURE;
    }

    st_Client *client = client_create(socket_fd, "");
    if (client == NULL)
    {
        close(socket_fd);
        printf("Connected Failed...\n");
        return EXIT_FAILURE;
    }

    printf("Connected\n");

    int result = EXIT_SUCCESS;
    if (fflush(stdout) == EOF || run_message_loop(client) < 0)
    {
        printf("Unexpected error\n");
        result = EXIT_FAILURE;
    }
    if (client_destroy(client) != 0)
    {
        result = EXIT_FAILURE;
    }
    else
    {
        printf("Disconnected\n");
    }
    return result;
}
