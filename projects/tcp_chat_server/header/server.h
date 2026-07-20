#ifndef SERVER_H
#define SERVER_H

#include "client.h"
#include "protocol.h"

typedef struct
{
    st_Client *clients;
    int num_clients;
} Server;

#endif