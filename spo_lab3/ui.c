#include "ui.h"

#define DELAY 50000
#define ERROR_DELAY 3
#define PAGE_SIZE 40

void init_terminal_window() {
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(FALSE);
}

void main_ui_loop(int socket, char* path) {


    root_files = update_current_dir(socket, &directory_size, path);
    size_t selected_row = 0;

    size_t slider = 0;

    client_directory = malloc(4096);
    strcpy(client_directory, path);

    int current_char = getch();
    set_err_msg("");
    while (current_char != 'q') {
        clear();
        process_err_msg();
        size_t page = selected_row / PAGE_SIZE;
        if (current_char == KEY_DOWN) {
            if (selected_row < directory_size - 1) {
                selected_row++;
            }
        } else if (current_char == KEY_UP) {
            if (selected_row > 0) {
                selected_row--;
            }
        } else if (current_char == "w") {
            if (slider > 0) {
                slider++;
            }
        } else if (current_char == "s") {
            if (slider > 0) {
                slider--;
            }
        } else if (current_char == ' ') {
//            clear();
            if (root_files[selected_row]->type == DIR_T) {
                if (strcmp(root_files[selected_row]->name, "..") == 0) {
                    if (strcmp(client_directory, "/") == 0) {
                        set_err_msg("This is already root! Cannot open parent directory.");
                    } else {
                        remove_last_dir(client_directory);
                    }
                } else if (strcmp(root_files[selected_row]->name, ".") != 0) {
                    strcat(client_directory, root_files[selected_row]->name);
                    strcat(client_directory, "/");
                    printf("\n\n\n%s", client_directory);
                }
                for (int i = 0; i < directory_size; ++i) {
                    free(root_files[i]);
                }
                free(root_files);
                root_files = update_current_dir(socket, &directory_size, client_directory);
                selected_row = 0;

            } else {
                char *name = malloc(1024);
                strcpy(name, client_directory);
                strcat(name, root_files[selected_row]->name);
                cat_main_loop(socket, name);

            }
        } else if (current_char == 'd') {

            if (root_files[selected_row]->type == FILE_T) {
                char msg[255] = {0};
                get_file(socket, client_directory, root_files[selected_row]->name, "./");
                set_err_msg("File Downloaded");
            } else {
                if (strcmp(root_files[selected_row]->name, "..") == 0) {
                    set_err_msg("Please, select nested directory");
                } else if (strcmp(root_files[selected_row]->name, ".") == 0) {
                    set_err_msg("Please, select nested directory");
                } else {
                    char dirname[1024];
                    strcat(dirname, root_files[selected_row]->name);
                    char msg[255] = {0};
                    get_dir(socket, client_directory, dirname, "./");
                    set_err_msg("Dir Download");
                }
            }
        } else if (current_char == 'u') {
            upload_ui_loop(socket, client_directory);
        }

        wprintw(stdscr, "%d", selected_row);
        wmove(stdscr, 0, 30);
        waddstr(stdscr, client_directory);
        draw_files(root_files, directory_size, selected_row, page);
        draw_help_message();
        draw_error_message();
        char buffer_dump[1024];
        getstr(buffer_dump);
        refresh();
        usleep(DELAY);
        current_char = getch();
    }
    endwin();
}

void draw_files(file_t **files, size_t size, size_t selected, size_t page) {
    if (files == NULL) {
        set_err_msg("Could not get data from server. Restart the program.");
        return;
    }
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int start_x = (max_x - 80) / 4;

    for (int i = PAGE_SIZE * page ; i < PAGE_SIZE * (page+1); ++i) {
        if (i >= size) break;
        if (i == selected) {
            attron(A_STANDOUT);
        }
        int row =  i % PAGE_SIZE + 4;
        wmove(stdscr, row, start_x);
        waddstr(stdscr, files[i]->name);
        char *filetype;
        if (files[i]->type == FILE_T) {
            filetype = "<FILE>";
        } else {
            filetype = "<DIRECTORY>";
        }
        wmove(stdscr, row, start_x + 40 - strlen(filetype));
        waddstr(stdscr, filetype);

        mvwaddch(stdscr, row, start_x + 40, ACS_VLINE);
        wprintw(stdscr, "%d", files[i]->size);
        mvwaddch(stdscr, row, start_x + 55, ACS_VLINE);
        waddstr(stdscr, files[i]->change_date);
        attroff (A_STANDOUT);

    }
}

void draw_help_message() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int start_y = max_y / 2 + 5;
    int start_x = max_x - 80;
    wmove(stdscr, start_y, start_x);
    waddstr(stdscr, "space --enter a directory");
    wmove(stdscr, start_y + 1, start_x);
    waddstr(stdscr, "d --download any file/directory to your current directory");
    wmove(stdscr, start_y + 2, start_x);
    waddstr(stdscr, "u --upload any file/directory to the server");
    wmove(stdscr, start_y + 3, start_x);
    waddstr(stdscr, "q --exit program");
}

void remove_last_dir(char *path) {
    while (*path != '\0') { path++; }
    path--;
    while (*(path - 1) != '/') { path--; }
    *path = '\0';
}

void draw_error_message() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int start_y = max_y / 2 - 5;
    int start_x = max_x - 80;

    attron(A_BOLD);
    wmove(stdscr, start_y, start_x);
    waddstr(stdscr, error_message);
    attroff(A_BOLD);
}

void set_err_msg(char *str) {
    free(error_message);
    error_message = malloc(strlen(str) + 1);
    strcpy(error_message, str);
}

void upload_ui_loop(int socket, char *remote_dir) {
    char current_directory[1024] = {};
    getcwd(current_directory, sizeof(current_directory));
    strcat(current_directory, "/");

    size_t size;
    file_t **root_files = list(current_directory, &size);

    if (root_files == NULL) {
        char *buff = malloc(1024);
        sprintf(buff, "Error occurred processing dir '%s'", current_directory);
        set_err_msg(buff);
        free(buff);
        return;
    }

    int current_char = getch();

    set_err_msg("");

    size_t selected_row = 0;
    while (current_char != 'q') {
        clear();
        process_err_msg();
        int page = selected_row / 45;
        if (current_char == KEY_DOWN) {
            if (selected_row < size - 1) {
                selected_row++;
            }
        } else if (current_char == KEY_UP) {
            if (selected_row > 0) {
                selected_row--;
            }
        } else if (current_char == ' ') {
            if (root_files[selected_row]->type == DIR_T) {
                if (strcmp(root_files[selected_row]->name, "..") == 0) {
                    if (strcmp(current_directory, "/") == 0) {
                        set_err_msg("This is already root! Cannot open parent directory.");
                    } else {
                        remove_last_dir(current_directory);
                    }
                } else if (strcmp(root_files[selected_row]->name, ".") != 0) {
                    strcat(current_directory, root_files[selected_row]->name);
                    strcat(current_directory, "/");
                }

                for (int i = 0; i < size; ++i) {
                    free(root_files[i]->name);
                    free(root_files[i]);
                }
                free(root_files);
                root_files = list(current_directory, &directory_size);
                selected_row = 0;
            } else {
                set_err_msg("This is a file. Can't cd into it.");
            }
        } else if (current_char == 'u') {

            char path[256] = {};
//            message_t *message = malloc(sizeof(message_t));
//            strcpy(message->command, "upload");
//            int file_count = 1;
//            memset(message->size, file_count, sizeof(int));
            message_t message = {
                    .command = "upload",
                    .size = 1,
            };
            printf("Sending file: %s\n", root_files[selected_row]->name);
            send_file(socket, *root_files[selected_row], message);
            strcat(path, current_directory);
            strcat(path, root_files[selected_row]->name);
            if (root_files[selected_row]->type == FILE_T) {
                send_file_content(socket, path);
                set_err_msg("File uploaded");
            } else {
                if (strcmp(root_files[selected_row]->name, "..") == 0) {
                    set_err_msg("Please, select nested directory");
                } else if (strcmp(root_files[selected_row]->name, ".") == 0) {
                    set_err_msg("Please, select nested directory");
                } else {
                    send_dir_content(socket, root_files[selected_row]->name);
                    set_err_msg("Dir uploaded");
                }
            }
        }

        wprintw(stdscr, "%d", selected_row);
        wmove(stdscr, 0, 30);
        waddstr(stdscr, current_directory);
        waddstr(stdscr, root_files[selected_row]->name);
        draw_files(root_files, size, selected_row, page);
        draw_upload_help_message();
        draw_error_message();
        refresh();
        char buffer_dump[1024];
        getstr(buffer_dump);
        usleep(DELAY);
        current_char = getch();
    }
}

void process_err_msg() {
    static time_t err_msg_start;
    time_t current_time;
    time(&current_time);
    if (error_message[0] != '\0') {
        if (current_time - err_msg_start > ERROR_DELAY) {
            set_err_msg("");
            time(&err_msg_start);
        }
    } else {
        time(&err_msg_start);
    }
}

void draw_file_content(char *file_content, char *percent, size_t size) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    wmove(stdscr, 2, 0);
    waddstr(stdscr, file_content);
    wmove(stdscr, max_y-2 , 0);
    waddstr(stdscr, percent);
}

void cat_main_loop(int socket, char *name) {
    FILE *file;
    char *buff = 0;
    long fsize;
    if ((file = fopen(name, "r")) == NULL){
        set_err_msg("Cant open file");
    } else {
        fseek(file, 0, SEEK_END);
        fsize = ftell(file);
        fseek(file, 0, SEEK_SET);
        double percent = 1000 * 100 / fsize;
        if (percent > 100) percent = 100;
        buff = malloc (1000);
        bzero(buff, 1000);
        if (buff) fread(buff, 1, 1000, file);

        char *end_value = (char *) malloc(100);
        sprintf(end_value, "\n----- %.2f percent shown -----", percent);

        fclose(file);

        int current_char = getch();
        set_err_msg("");
        while (current_char != 'q') {
            clear();
            process_err_msg();
            waddstr(stdscr, name);
            wmove(stdscr,1,0);
            draw_file_content(buff, end_value, 1000);
            draw_cat_help_message();
            draw_error_message();

            refresh();
            char buffer_dump[1024];
            getstr(buffer_dump);
            usleep(DELAY);
            current_char = getch();
        }
    }
}

void draw_upload_help_message() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int start_y = max_y / 2 + 5;
    int start_x = max_x - 80;
    wmove(stdscr, start_y, start_x);
    waddstr(stdscr, "space --enter a directory");
    wmove(stdscr, start_y + 1, start_x);
    waddstr(stdscr, "u --upload any file/directory to the server");
    wmove(stdscr, start_y + 3, start_x);
    waddstr(stdscr, "q --exit upload mode");
}

void draw_cat_help_message() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int start_y = max_y / 2 + 5;
    int start_x = max_x - 80;
    wmove(stdscr, start_y + 3, start_x);
    waddstr(stdscr, "q --exit cat mode");
}
