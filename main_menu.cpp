#include <cstdlib>
#include <string>

#include <ncurses.h>

#define MAIN_MENU_CHOICES 6

void display_menu(WINDOW *menu_win, int highlight, const char *choices[], int menu_size);

void handle_main_menu() {
    int return_code = 0;
    int highlight = 0;
    const char *choices[] = {"make", "make all", "make clean", "make fclean", "make re", "make :"};

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    WINDOW *menu_win = newwin(rows - 4, cols - 4, 2, 2);
    box(menu_win, 0, 0);

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
                    return_code = system("make 1>/dev/null 2>/dev/null");
                } else if (highlight == 1) {
                    return_code = system("make all 1>/dev/null 2>/dev/null");
                } else if (highlight == 2) {
                    return_code = system("make clean 1>/dev/null 2>/dev/null");
                } else if (highlight == 3) {
                    return_code = system("make fclean 1>/dev/null 2>/dev/null");
                } else if (highlight == 4) {
                    return_code = system("make re 1>/dev/null 2>/dev/null");
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
                    input_buffer_string += " 1>/dev/null 2>/dev/null";
                    return_code = system(input_buffer_string.c_str());
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
