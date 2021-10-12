#ifndef SPO_LAB3_SERVER_H
#define SPO_LAB3_SERVER_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include "utils.h"

int server(unsigned int port, char *path);
void *connection_handler(void *socket_desc);
void send_notification(int sock);
#endif