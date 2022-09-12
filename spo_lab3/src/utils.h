#ifndef SPO_LAB3_UTILS_H
#define SPO_LAB3_UTILS_H

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <netinet/in.h>
#include <time.h>



enum FILE_TYPE {
    DIR_T,
    FILE_T
};

typedef struct file_t {
    char name[255];
    enum FILE_TYPE type;
    uint size;
    char change_date[255];
} file_t;

typedef struct message_t {
    size_t size;
    char command[128];
    char arg[4096];
    void* data;
} message_t ;

file_t **list(char *path, size_t *files_count);

//typedef struct socket_t {
//    int socket;
//    struct socket_t *next;
//} socket_t;
//
//socket_t *socket_list_create(int socket, socket_t *next);
//socket_t *socket_list_append(socket_t *head, int socket);
//socket_t *socket_list_remove(socket_t *head, int socket);
//void *socket_list_foreach(socket_t *head, void (*func)(int));
//
//char *trim(char* str);

file_t **update_current_dir(int socket, size_t *size, char *path);
void send_command(void *sock, message_t message);
message_t *recv_command(void *sock);
message_t *recv_message(int server_socket);
void send_file(void *sock, file_t file, message_t message);
file_t *recv_file(int server_socket);
void send_file_content(void *sock, char *file_path);
void send_dir_content(void *sock, char *dir_path);
void get_file(int socket, char *path, char *filename, char *host_path);
void get_dir(int socket, char *path, char *dirname, char *host_dirname);
#endif

