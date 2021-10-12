#include "utils.h"

file_t **update_current_dir(int socket, size_t *size, char* path) {
    message_t message = {
            .command = "ls",
            .size = 128 + 4096 + sizeof(size_t) + 1,
            .data = NULL
    };
    strcpy(message.arg, path);
    send_command(socket, message);

    message_t *response = recv_command(socket);
    file_t **files = malloc(response->size * sizeof(file_t *));
    *size = 0;
    do{
        file_t *file = recv_file(socket);
        files[*size] = file;
        printf("File received %s, %d\n", files[*size]->name, *size);
        (*size)++;
    }while(*size < response->size);
    return files;
}

file_t **list(char *path, size_t *files_count){
    DIR *dir = opendir(path);
    struct dirent *dirent;
    file_t **files = malloc(256 * sizeof(file_t));
    if (dir) {
        *files_count = 0;
        while ((dirent = readdir(dir)) != NULL) {
            char full_path[256] = {};
            strcat(full_path, path);
            strcat(full_path, dirent->d_name);

            struct stat st;
            stat(full_path, &st);
            file_t *file = malloc(sizeof(file_t));
            strcpy(file->name, dirent->d_name);
            if (S_ISREG(st.st_mode)) {
                file->type = FILE_T;
            } else if (S_ISDIR(st.st_mode)) {
                file->type = DIR_T;
            }
            file->size = st.st_size;
            strcpy(file->change_date, ctime(&st.st_mtim.tv_sec));

            files[*files_count] = file;
            (*files_count)++;
        }
        closedir(dir);
    } return files;
}

void send_command(void *sock, message_t message){
//    printf("sizeof(message)=%lu;\n", sizeof(message));
    char *request = malloc(sizeof(message));
    char *response = malloc(BUFSIZ);
    bzero(response, BUFSIZ);

    memcpy(request, &message, sizeof(message));

    if (send(sock, request, BUFSIZ, 0) <= 0) {
        printf("Could not send data");
    }
}

message_t *recv_command(void *sock) {
    char *response, client_message[BUFSIZ] = {0};

    size_t read_size;
    message_t *message = malloc(sizeof(message_t));
    read_size = recv(sock, client_message, BUFSIZ, 0);
    if (read_size <= 0) {
        free(message);
        return NULL;
    }
    memcpy(message, client_message, sizeof(message_t));
//    printf("New message: Command=%s; Arg=%s, Size=%lu from %d\n", message->command, message->arg, message->size, sock);
    return message;
}

void send_file(void *sock, file_t file, message_t message) {
//    printf("sizeof(message)=%lu; sizeof (file)=%lu; size %d\n", sizeof(message), sizeof(file), message.size);
    char *request = malloc(sizeof(message));
    char *response = malloc(BUFSIZ);
    bzero(response, BUFSIZ);

    memcpy(request, &message, sizeof(message));

    if (send(sock, request, BUFSIZ,0) <= 0) {
        perror("Could not send data");
        exit(1);
    }
    if (strcmp(message.command, "ls") == 0 || strcmp(message.command, "upload") == 0) {
        char *data = malloc(message.size);
        memcpy(data, &file, message.size);

        if (send(sock, data, message.size, 0) <= 0) {
            perror("Could not send data");
            exit(1);
        }

        free(data);
    }
    free(request);
}

void send_file_content(void *sock, char *file_path) {
    printf("Send file, path: %s\n", file_path);
    FILE *file = fopen(file_path, "r");
    if (file != NULL) {
        struct stat filestat;
        stat(file_path, &filestat);
        char *buffer = malloc(filestat.st_size);
//        bzero(buffer, filestat.st_size);
        fread(buffer, filestat.st_size, 1, file);

        message_t message = {
                .command = NULL,
                .size = filestat.st_size,
        };

        send_command(sock, message);

        char *data = malloc(filestat.st_size);
        memcpy(data, buffer, filestat.st_size);
        if (send(sock, data, filestat.st_size, 0) <= 0) {
            perror("Could not send data");
            exit(1);
        }
        free(data);
        free(buffer);
    }
    fclose(file);
}

void send_dir_content(void *sock, char *dir_path) {
    size_t size;
    file_t **dir_files = update_current_dir(socket, &size, dir_path);

    for (int i = 0; i < size; ++i) {
        char file_name[255];
        strcpy(file_name, dir_path);
        strcat(file_name, dir_files[i]->name);
        printf("Path to downloading dir: %s", file_name);
        if (dir_files[i]->type == FILE_T) {
            send_file_content(socket, file_name);
        } else if (strcmp(dir_files[i]->name, ".") != 0 && strcmp(dir_files[i]->name, "..") != 0) {
            send_dir_content(socket, file_name);
        }
        free(dir_files[i]);
    }
    free(dir_files);
}

file_t *recv_file(int server_socket) {
    char *response, client_message[BUFSIZ] = {0};

    size_t read_size;
    while ((read_size = recv(server_socket, client_message, BUFSIZ, 0)) > 0) {
        message_t *message = malloc(sizeof(message_t));
        memcpy(message, client_message, sizeof(message_t));

//        printf("New message: Command=%s; Arg=%s, Size=%lu from %d\n", message->command, message->arg, message->size, server_socket);
        file_t *file;
        if(strcmp(message->command, "ls") == 0 || strcmp(message->command, "upload")){
//            printf("\nThis message contains data...\n");
            bzero(client_message, BUFSIZ);
            size_t data_read_size = recv(server_socket, client_message, message->size, 0);
            if (data_read_size <= 0) printf("DIAMETR OCHKA UVELICHEN");

            message->data = malloc(message->size);
            memcpy(message->data, client_message, message->size);

            file = message->data;

            free(message);
            bzero(client_message, BUFSIZ);
            return file;
        }
    }
}

message_t *recv_message(int server_socket) {
    char *response, client_message[BUFSIZ] = {0};

    size_t read_size;
    read_size = recv(server_socket, client_message, BUFSIZ, MSG_PEEK);
    message_t *message = malloc(sizeof(message_t));
    memcpy(message, client_message, sizeof(message_t));

//    printf("New message: Command=%s; Arg=%s, Size=%lu from %d\n", message->command, message->arg, message->size, server_socket);
    return message;

}

void get_file(int socket, char *path, char *filename, char *host_path) {
    char file_path[256];
    strcpy(file_path, path);
    strcat(file_path, filename);

    char host_filepath[256];
    strcpy(host_filepath, host_path);
    strcat(host_filepath, filename);

    message_t message = {
            .command = "download",
            .size = 128 + 4096 + sizeof(size_t) + 1,
            .data = NULL,
            };
    strcpy(message.arg, file_path);
    send_command(socket, message);

    message_t *response = recv_command(socket);
    message_t *data = malloc(response->size);
    size_t data_read_size = recv(socket, data, response->size, 0);
    if (data_read_size <= 0) printf("DIAMETR OCHKA UVELICHEN");

    response->data = malloc(response->size);
    memcpy(response->data, data, response->size);

    FILE *file = fopen(host_filepath, "w+");
    if (file == NULL) {
        printf("Could not receive the file!");
        return;
    }
    fwrite(response->data, response->size, 1, file);
//    free(response);
//    free(&message);
    free(data);
    fclose(file);
    printf("File downloaded");
}

void get_dir(int socket, char *path, char *dirname, char *host_dirname) {
    char dir_path[256];
    strcpy(dir_path, path);
    strcat(dir_path, dirname);
    strcat(dir_path, "/");

    char host_dir[256];
    strcpy(host_dir, host_dirname);
    strcat(host_dir, dirname);

    struct stat st = {0};
    if (stat(host_dir, &st) != -1) {
        printf("Could not create directory.");
        return;
    }

    mkdir(host_dir, 0777);
    size_t size;
    file_t **dir_files = update_current_dir(socket, &size, dir_path);

    if (dir_files == NULL) {
        printf("Error of reading subdir info!");
        return;
    }
    strcat(host_dir, "/");

    for (int i = 0; i < size; ++i) {
        if (dir_files[i]->type == FILE_T) {
            get_file(socket, dir_path, dir_files[i]->name, host_dir);
        } else if (strcmp(dir_files[i]->name, ".") != 0 && strcmp(dir_files[i]->name, "..") != 0) {
            get_dir(socket, dir_path, dir_files[i]->name, host_dir);
        }
        free(dir_files[i]);
    }
    free(dir_files);
    printf("Dir downloaded");
}



