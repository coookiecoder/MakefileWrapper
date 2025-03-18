#include <cstdlib>
#include <mutex>
#include <string>
#include <thread>

#include <ncurses.h>

#define MAIN_MENU_CHOICES 6

void display_menu(WINDOW *menu_win, int highlight, const char *choices[], int menu_size);

static
int execute_command(WINDOW *command_win, const char *command) {
    const int rows = getmaxy(command_win);
    const int cols = getmaxx(command_win);

    FILE *fp = popen(command, "r");
    if (fp == nullptr)
        return 1;

    char buffer[cols - 1];
    int line = 1;
    werase(command_win);

    while (fgets(buffer, cols - 1, fp) != nullptr) {
        if (line < rows + 3) {
            mvwprintw(command_win, line++, 1, "%s", buffer);
        } else {
            werase(command_win);
            line = 1;
            mvwprintw(command_win, line++, 1, "%s", buffer);
        }
    }
    box(command_win, 0, 0);
    wrefresh(command_win);

    return pclose(fp);
}

static
void init_menu(WINDOW **ptr) {
    const int rows = getmaxy(stdscr);
    const int cols = getmaxx(stdscr);

    const int width_menu = cols / 4;
    const int height_menu = rows;
    constexpr int x_pos_menu = 0;
    constexpr int y_pos_menu = 0;

    *ptr = newwin(height_menu, width_menu, y_pos_menu, x_pos_menu);
    box(*ptr, 0, 0);

    wrefresh(*ptr);
}

static
void init_command(WINDOW **ptr) {
    const int rows = getmaxy(stdscr);
    const int cols = getmaxx(stdscr);

    const int width_command = cols - cols / 4;
    const int height_command = rows;
    const int x_pos_command = cols / 4;
    constexpr int y_pos_command = 0;

    *ptr = newwin(height_command, width_command, y_pos_command, x_pos_command);
    box(*ptr, 0, 0);

    wrefresh(*ptr);
}


void handle_main_menu() {
    int return_code = 0;
    int highlight = 0;
    const char *choices[] = {"make", "make all", "make clean", "make fclean", "make re", "make :"};


    WINDOW *menu_win;
    WINDOW *command_win;

    init_menu(&menu_win);
    init_command(&command_win);

    refresh();

    while (true) {
        display_menu(menu_win, highlight, choices, MAIN_MENU_CHOICES);
        int choice = getch();

        switch (choice) {
            case KEY_UP:
                if (highlight == 0) {
                    highlight = MAIN_MENU_CHOICES - 1;
                } else {
                    highlight--;
                } refresh();
            break;

            case KEY_DOWN:
                if (highlight == MAIN_MENU_CHOICES - 1) {
                    highlight = 0;
                } else {
                    highlight++;
                } refresh();
            break;

            case 10:
                if (highlight == 0) {
                    return_code = execute_command(command_win,"make &");
                } else if (highlight == 1) {
                    return_code = execute_command(command_win,"make all &");
                } else if (highlight == 2) {
                    return_code = execute_command(command_win,"make clean &");
                } else if (highlight == 3) {
                    return_code = execute_command(command_win,"make fclean &");
                } else if (highlight == 4) {
                    return_code = execute_command(command_win,"make re &");
                } else if (highlight == 5) {
                    char input_buffer[256];
                    std::string input_buffer_string;
                    echo();
                    mvwprintw(menu_win, 7, 2, "Enter make command: ");
                    wrefresh(menu_win);
                    wgetnstr(menu_win, input_buffer, sizeof(input_buffer) - 1);
                    noecho();
                    wrefresh(menu_win);
                    input_buffer_string = input_buffer;
                    input_buffer_string += " &";
                    return_code = execute_command(command_win,input_buffer_string.c_str());
                    getch();
                }
                if (return_code) {
                    init_pair(2, COLOR_WHITE, COLOR_RED);
                } else {
                    init_pair(2, COLOR_WHITE, COLOR_GREEN);
                }
            wbkgd(menu_win, COLOR_PAIR(2));
            break;

            case KEY_EXIT:
                return;

            default:
                break;
        }
    }
}
