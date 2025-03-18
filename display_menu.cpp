#include <ncurses.h>

#define MAIN_MENU_CHOICES 5

void display_menu(WINDOW *menu_win, const int highlight, const char *choices[], int menu_size) {
    werase(menu_win); // Clear window
    box(menu_win, 0, 0);
    for (int i = 0; i < menu_size; i++) {
        if (i == highlight) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, i + 2, 2, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, i + 2, 2, "%s", choices[i]);
        }
    }
    wrefresh(menu_win);
}