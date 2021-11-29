#include "gameWindow.hpp"
#include <vector>

void GameWindow::DrawGrid()
{
    for (int i = 0; i < GRID_LINES; i++)
    {
        for (int j = 0; j < GRID_COLUMNS; j++)
        {
            if (i % 2 == 0 && j % 4 == 0)
            {

                if (i % 6 == 0 && j % 4 == 0 || j % 12 == 0 && i % 2 == 0)
                    wattron(grid, COLOR_PAIR(2));
                mvwaddch(grid, i, j, '+');
                if (i % 6 == 0 && j % 4 == 0 || j % 12 == 0 && i % 2 == 0)
                    wattroff(grid, COLOR_PAIR(2));
            }
            else if (j % 4 == 0)
            {
                if (j % 12 == 0)
                    wattron(grid, COLOR_PAIR(2));
                mvwaddch(grid, i, j, '|');

                if (j % 12 == 0)
                    wattroff(grid, COLOR_PAIR(2));
            }
            else if (i % 2 == 0)
            {
                if (i % 6 == 0)
                    wattron(grid, COLOR_PAIR(2));
                mvwaddch(grid, i, j, '-');

                if (i % 6 == 0)
                    wattroff(grid, COLOR_PAIR(2));
            }
        }
    }
}

void GameWindow::Render()
{
    std::vector<std::vector<int>> deck = board.GetUserBoard();
    std::vector<std::vector<int>> underDeck = board.GetBoard();
    for (int i = 0; i < Board::SIZE; i++)
    {
        for (int j = 0; j < Board::SIZE; j++)
        {
            if (deck.at(i).at(j) == 0)
            {
                mvwaddch(grid, 2 * i + 1, 2 + 4 * j, ' ');
                continue;
            }
            if (underDeck.at(i).at(j) > 0)
            {
                wattron(grid, COLOR_PAIR(1));
            }
            mvwaddch(grid, 2 * i + 1, 2 + 4 * j, '0' + deck.at(i).at(j));
            if (underDeck.at(i).at(j) > 0)
            {
                wattroff(grid, COLOR_PAIR(1));
            }
        }
    }
    Refresh();
    wmove(grid, 2 * cursorY + 1, 2 + 4 * cursorX);
    wrefresh(grid);
}

void GameWindow::PrintInfo()
{
    wprintw(info, "\tSudoku v0.1.0\n");
    wprintw(info, "ESC to leave\n");
    wprintw(info, "Arrows or mouse to move the cursor\n");
    wprintw(info, "E - generate easy problem\n");
    wprintw(info, "M - generate middle problem\n");
    wprintw(info, "H - generate hard problem\n");
    wprintw(info, "S - auto solve problem\n");
    wprintw(info, "I - ented editor mode\n");
    wprintw(info, "F - leave editor mode\n");
    wprintw(info, "Del, BackSpace, Space, 0 - delete number under cursor\n");
    wprintw(info, "C - clear the board\n");
}

void GameWindow::Normalize(int &x, int &y, int mod)
{
    if (x < 0)
    {
        while (x < 0)
        {
            x += mod;
        }
    }
    else
    {
        x = x % mod;
    }
    if (y < 0)
    {
        while (y < 0)
        {
            y += mod;
        }
    }
    else
    {
        y = y % mod;
    }
}

void GameWindow::Move(int dy, int dx)
{
    std::vector<std::vector<int>> deck = board.GetBoard();
    int ny = cursorY + dy;
    int nx = cursorX + dx;
    Normalize(nx, ny);
    if (editMode)
    {
        cursorY = ny;
        cursorX = nx;
        return;
    }
    for (;;)
    {
        if (ny == cursorY && nx == cursorX)
        {
            return;
        }
        else if (deck.at(ny).at(nx) != 0)
        {
            ny += dy;
            nx += dx;
            Normalize(nx, ny);
        }
        else if (deck.at(ny).at(nx) == 0)
        {
            cursorY = ny;
            cursorX = nx;
            return;
        }
    }
}

void GameWindow::Tab()
{
    std::vector<std::vector<int>> deck = board.GetUserBoard();
    if (editMode)
    {
        cursorX++;
        if (cursorX >= Board::SIZE)
        {
            cursorX = 0;
            cursorY = (cursorY + 1) % Board::SIZE;
        }
        return;
    }
    int ny = cursorY;
    int nx = cursorX + 1;
    for (;;)
    {
        if (nx >= Board::SIZE)
        {
            nx = 0;
            ny = (ny + 1) % Board::SIZE;
        }
        if (ny == cursorY && nx == cursorX)
        {
            return;
        }
        else if (deck.at(ny).at(nx) != 0)
        {
            nx++;
        }
        else if (deck.at(ny).at(nx) == 0)
        {
            cursorY = ny;
            cursorX = nx;
            return;
        }
    }
}

void GameWindow::FindCoordinate()
{
    int tmpY = event.y - GRID_Y;
    int tmpX = event.x - GRID_X;
    if (tmpY < 0 || tmpY > GRID_LINES - 1 || tmpX < 0 || tmpX > GRID_COLUMNS - 1)
    {
        curs_set(0);
        return;
    }
    tmpY /= 2;
    tmpX /= 4;
    if (tmpY == Board::SIZE)
        tmpY--;
    if (tmpX == Board::SIZE)
        tmpX--;
    if(!editMode && board.GetBoard().at(tmpY).at(tmpX) > 0)
    {
        return;
    }    
    cursorY = tmpY;
    cursorX = tmpX;
}

bool GameWindow::Controle(int key)
{
    if (key == KEY_MOUSE)
    {
        if (getmouse(&event) == OK)
        {
            if (event.bstate & BUTTON1_CLICKED || event.bstate & BUTTON1_PRESSED)
            {
                curs_set(1);
                FindCoordinate();
            }
        }
    }
    else if (key == 27)
    {
        return false;
    }
    else if (key == 'E' || key == 'e')
    {
        board.Generate(1);
    }
    else if (key == 'M' || key == 'm')
    {
        board.Generate(2);
    }
    else if (key == 'H' || key == 'h')
    {
        board.Generate(3);
    }
    else if (key == 'i' || key == 'I')
    {
        editMode = true;
        board.Sync();
        Render();
    }
    else if (key == 'f' || key == 'F')
    {
        editMode = false;
        if (!board.Solvable())
        {
            wclear(log);
            wprintw(log, "This problem does not have solution\n");
            wrefresh(log);
            curs_set(0);
        }
        Render();
    }
    else if (key == KEY_LEFT)
    {
        Move(0, -1);
        curs_set(1);
    }
    else if (key == KEY_RIGHT)
    {
        Move(0, 1);
        curs_set(1);
    }
    else if (key == KEY_UP)
    {
        Move(-1, 0);
        curs_set(1);
    }
    else if (key == KEY_DOWN)
    {
        Move(1, 0);
        curs_set(1);
    }
    else if (key == '\t')
    {
        Tab();
        curs_set(1);
    }
    else if (isdigit(key) && key > '0')
    {
        board.Set(cursorY, cursorX, key - '0', editMode);
    }
    else if (key == '0' || key == KEY_BACKSPACE || key == KEY_DC || key == ' ')
    {
        board.Set(cursorY, cursorX, 0, editMode);
    }
    else if (key == 'S' || key == 's')
    {
        board.AutoSolve();
        Render();
    }
    else if (key == 'C' || key == 'c')
    {
        board.Reset();
        Render();
    }
    return true;
}

void GameWindow::GameLoop()
{
    for (;;)
    {
        int key = wgetch(grid);
        if (!Controle(key))
        {
            break;
        }

        Render();
        if (!editMode)
        {
            if (board.AllFilled())
            {
                Render();
                if (board.CheckSolution())
                {
                    wclear(log);
                    wprintw(log, "You won the game, congratulations! Press any key to continue\n");
                    wrefresh(log);
                    curs_set(0);
                    int ch = getch();
                    if (!(ch == 27))
                    {
                        board.Reset();
                    }
                    else {
                        break;
                    }
                }
            }
        }
    }
    Delete();
}

GameWindow::GameWindow() : cursorY(0), cursorX(0), editMode(false)
{
    grid = newwin(GRID_LINES, GRID_COLUMNS, GRID_Y, GRID_Y);
    info = newwin(INFO_LINES, INFO_COLUMNS, INFO_Y, INFO_X);
    log = newwin(LOG_LINES, LOG_COLUMNS, LOG_Y, LOG_X);
    keypad(grid, true);
    keypad(info, true);
    DrawGrid();
    PrintInfo();
}