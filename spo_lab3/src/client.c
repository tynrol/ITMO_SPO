#include "client.h"
int server_socket;

int client(char *address, uint32_t port){
    struct sockaddr_in server;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        return 1;
    }
    server.sin_addr.s_addr = inet_addr(address);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(server_socket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Failed connect to server");
        return 1;
    }

    char path[4096] = {0};
    read(server_socket , path, 4096);
    printf("%s\n", path);

    pthread_t sniffer_thread;
    pthread_create(&sniffer_thread, NULL, dir_changes_listener, NULL);


    init_terminal_window();
    main_ui_loop(server_socket, path);
    close(server_socket);
    return 0;
}

void *dir_changes_listener() {
    char *response = malloc(BUFSIZ);
    memset(response, 0, BUFSIZ);
    message_t *message = malloc(sizeof(message_t));
    while (recv(server_socket, response, BUFSIZ, MSG_PEEK) > 0){

        memcpy(message, response, sizeof(message_t));
        if(strcmp(message->command, "notify") == 0){
            printf("New message: Command=%s; Arg=%s, Size=%lu\n", message->command, message->arg, message->size);
            recv(server_socket, response, BUFSIZ, 0);
            root_files = update_current_dir(server_socket, &directory_size, client_directory);
            set_err_msg("Updated");
        }
    }

    free(response);
    return 0;
}



