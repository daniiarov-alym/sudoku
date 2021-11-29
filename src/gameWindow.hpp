#ifndef GAME_WIN_HPP
#define GAME_WIN_HPP
#include <ncurses.h>
#include "board.hpp"
#include <iostream>

class GameWindow
{
private:
    static const int GRID_LINES = 19;
    static const int GRID_COLUMNS = 37;
    static const int GRID_Y = 3;
    static const int GRID_X = 3;
    static const int INFO_LINES = 11;
    static const int INFO_COLUMNS = 80;
    static const int INFO_Y = 3;
    static const int INFO_X = GRID_X+GRID_COLUMNS+5;
    static const int LOG_X = INFO_X;
    static const int LOG_Y = INFO_Y+INFO_LINES+5;
    static const int LOG_LINES = 1;
    static const int LOG_COLUMNS = INFO_COLUMNS;
    WINDOW* grid;
    WINDOW* info;
    WINDOW* log;
    int cursorY;
    int cursorX;
    Board board;
    bool editMode;
    MEVENT event;
    
    void PrintLog();
    void PrintInfo();
    void FindCoordinate();
    void DrawGrid();
    //returns false on exit
    bool Controle(int key);
    void Delete()
    {
        delwin(grid);
        delwin(info);
    }
    void Normalize(int& x, int& y, int mod = Board::SIZE);
    void Move(int dy, int dx);
    void Tab();
public:
    GameWindow();
    void Refresh()
    {
        wrefresh(info);
        wrefresh(grid);
        refresh();
    }
    void GameLoop();
    void Render();
};


#endif