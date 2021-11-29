#include <ncurses.h>
#include <locale.h>
#include "gameWindow.hpp"

void init_curses()
{
    initscr();
    noecho();
    clear();
    setlocale(LC_ALL, "");
    cbreak();
    keypad(stdscr, true);
    curs_set(0);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    start_color();
    init_pair(1, COLOR_RED, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_BLUE);
}




int main()
{
    init_curses();
    GameWindow game;
    refresh();
    game.Refresh();
    game.GameLoop();
    endwin();
    
}