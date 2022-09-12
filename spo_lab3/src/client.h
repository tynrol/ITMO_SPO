#ifndef SPO_LAB3_CLIENT_H
#define SPO_LAB3_CLIENT_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "utils.h"
#include "ui.h"

int client(char *address, uint32_t port);
void *dir_changes_listener();
#endif
