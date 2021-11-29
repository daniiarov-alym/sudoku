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
                mvwaddch(grid, i, j, '+');
            }
            else if (j % 4 == 0)
            {
                mvwaddch(grid, i, j, '|');
            }
            else if (i % 2 == 0)
            {
                mvwaddch(grid, i, j, '-');
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
           
            mvwaddch(grid, 2 * i + 1, 2 + 4 * j, '0' + deck.at(i).at(j));
            if (underDeck.at(i).at(j) > 0)
            {
                mvwchgat(grid, 2 * i + 1, 2 + 4 * j, 1, A_COLOR, COLOR_PAIR(1), NULL);
            }
        }
    }
    Refresh();
    wmove(grid, 2 * cursorY + 1, 2 + 4 * cursorX);
    wrefresh(grid);
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
    int tmpY = event.y-GRID_Y;
    int tmpX = event.x-GRID_X;
    if (tmpY < 0 || tmpY > GRID_LINES-1 || tmpX < 0 || tmpX > GRID_COLUMNS-1)
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
    }
    else if (key == 'f' || key == 'F')
    {
        editMode = false;
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
    else if (key == '0' || key == KEY_BACKSPACE || key == KEY_DC)
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
                    wprintw(info, "Won!\n");
                    wrefresh(info);
                    curs_set(0);
                    getch();
                    board.Reset();
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
    
    keypad(grid, true);
    keypad(info, true);
    DrawGrid();
}