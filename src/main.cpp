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
    mousemask(ALL_MOUSE_EVENTS);
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