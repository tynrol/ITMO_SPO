#include "server.h"
char start_path[4096];
int sockets[128];
int socket_count;

int server(uint32_t port, char *path){
    strcpy(&start_path, path);
    struct sockaddr_in server_addr, client_addr;
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        perror("Could not create socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) perror("setsockopt");

    if (bind(socket_desc, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed. Error");
        exit(1);
    }
    listen(socket_desc, 3);

    printf("Server listen on port %d...\n", port);

    int c = sizeof(struct sockaddr_in), client_sock;
    while ((client_sock = accept(socket_desc, (struct sockaddr *) &client_addr, (socklen_t *) &c))) {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Connection accepted; Client IP: %s\n", client_ip);

        int *new_sock = malloc(1);
        *new_sock = client_sock;
        printf("Client socket added to a array %d\n", client_sock);
        sockets[socket_count] = client_sock;
        socket_count++;


        send(client_sock, start_path, 4096,0);

        pthread_t sniffer_thread;
        pthread_create(&sniffer_thread, NULL, connection_handler, (void *) (new_sock));
    }
    if (client_sock < 0) {
        perror("accept failed");
        exit(1);
    }
    return 0;
}

void *connection_handler(void *socket_desc) {
    int sock = *(int *) socket_desc;

    message_t *message;
    while((message = recv_command(sock)) != NULL) {
        if (strcmp(message->command, "ls") == 0) {
            size_t files_count;
            file_t **files = list(message->arg, &files_count);
            message_t response = {
                    .command = NULL,
                    .arg = NULL,
                    .size = files_count,
                    .data = NULL
            };
            send_command(sock, response);
            for (int i = 0; i < files_count; i++) {
                send_file(sock, *files[i], *message);
                printf("File send %s\n", files[i]->name);
            }
            free(message);
        } else if (strcmp(message->command, "download") == 0) {
            char command_dir[256] = {};
            strcat(command_dir, message->arg);
            struct stat st;
            stat(command_dir, &st);
            if (S_ISREG(st.st_mode)) {
                send_file_content(sock, command_dir);
            } else if (S_ISDIR(st.st_mode)) {
                send_dir_content(sock, command_dir);
            }
            for (int i = 0; i<socket_count; i++){
                send_notification(sockets[i]);
            }
        } else if (strcmp(message->command, "upload") == 0) {
            message_t response = {
                    .command = NULL,
                    .arg = NULL,
                    .size = 1,
                    .data = NULL
            };
            send_command(sock, response);
            file_t *file = recv_file(sock);
            if (strcmp(file->name, "..") == 0) {
                printf("Not nested directory was selected\n");
            } else if (strcmp(file->name, ".") == 0) {
                printf("Not nested directory was selected\n");
            } else if (file->type == FILE_T) {
                get_file(sock, start_path, file->name, "./");
                printf("File uploaded\n");
            } else if (file->type == DIR_T) {
                get_dir(sock, start_path, file->name, "./");
                printf("Dir uploaded\n");
            }
            for (int i = 0; i<socket_count; i++){
                send_notification(sockets[i]);
            }
        }
    }
}

void send_notification(int sock) {
    message_t msg = {
            .command = "notify",
            .size = BUFSIZ,
    };
    send_command(sock, msg);
    printf("Notification send to socket %d\n", sock);
}






