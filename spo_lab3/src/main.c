#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"

void help(){
    printf("Usage:\n"
           "./main server [port] [dir] - server start on [port] in [dir]\n"
           "./main client [port] [address] - client start on [address]:[port]\n");

}
int main(int argc, char *argv[]) {
    if (argc == 4){
        int port = atoi(argv[2]);
        if (port < 0 || port > 65535) {
            printf("Port must be in borders [0:65535]\n");
            exit(1);
        }
        if (strcmp(argv[1],"server")==0) {
            struct stat st;
            if (stat(argv[3], &st)){
                printf("[dir] is not available\n");
                exit(1);
            }
            if (!S_ISDIR(st.st_mode)) {
                printf("[dir] is not a directory\n");
                exit(1);
            }
            return server(port, argv[3]);
        }
        if (strcmp(argv[1],"client")==0) {
            return client(argv[3], port);
        }
    }
    help();
    return 0;
}
