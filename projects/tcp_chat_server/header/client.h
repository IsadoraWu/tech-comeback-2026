#ifndef CLIENT_H
#define CLIENT_H

#define MAX_USERNAME_LENGTH 32

typedef struct
{
    int socket_fd;
    char username[MAX_USERNAME_LENGTH];
} st_Client;

#endif