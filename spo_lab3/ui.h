#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <bits/types/time_t.h>
#include <time.h>
#include <stdatomic.h>
#include <bits/pthreadtypes.h>
#include <stdint-gcc.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#ifndef UI_UI_H
#define UI_UI_H

char *error_message;
file_t **root_files;
char *client_directory;
size_t directory_size;


void init_terminal_window();
void draw_files(file_t **root_files, size_t size, size_t selected, size_t page);
void draw_help_message();
void draw_upload_help_message();
void draw_error_message();
void remove_last_dir(char *path);
void set_err_msg(char *);
void process_err_msg();
void main_ui_loop(int socket, char* path);
void upload_ui_loop(int socket, char *remote_dir);
void draw_cat_help_message();
void cat_main_loop(int socket, char *file_path);
//void get_file(int socket, char *path, char *filename, char *host_path);
//void get_dir(int socket, char *path, char *dirname, char *host_dirname);

#endif //UI_UI_H


