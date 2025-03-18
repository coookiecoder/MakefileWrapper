#include <ncurses.h>

void handle_main_menu();
void display_menu(WINDOW *menu_win, int highlight, const char *choices[]);

int main() {
    initscr();            // Start ncurses mode
    cbreak();             // Disable line buffering
    noecho();             // Don't echo user input
    keypad(stdscr, true); // Enable special keys (arrows, function keys)
    nodelay(stdscr, true);
    curs_set(0);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    wbkgd(stdscr, COLOR_PAIR(1));

    handle_main_menu();

    endwin();
    return 0;
}
