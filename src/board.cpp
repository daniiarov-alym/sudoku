#include "board.hpp"
#include <algorithm>
#include <stdlib.h>
#include <time.h>
Board::Board()
    : board(SIZE, std::vector<int>(SIZE, 0)), userBoard(SIZE, std::vector<int>(SIZE, 0))
{
}

void Board::Set(int y, int x, int val, bool editMode)
{
    if (val < 0 || val > SIZE)
        return;
    if (!Valid(y,x,val) && val != 0)
    {
        return;
    }    
    if (!editMode && board.at(y).at(x) != 0)
    {
        return;
    }
    userBoard.at(y).at(x) = val;
    if (editMode)
    {
        board = userBoard;
    }
}

bool Board::CheckRows()
{
    for (int i = 0; i < 9; i++)
    {
        bool visited[9] = {false, false, false, false, false, false, false, false, false};
        for (int j = 0; j < 9; j++)
        {
            if (visited[userBoard[i][j] - 1])
            {
                return false;
            }
            else
            {
                visited[userBoard[i][j] - 1] = true;
            }
        }
    }
    return true;
}

bool Board::CheckColumns()
{
    for (int j = 0; j < 9; j++)
    {
        bool visited[9] = {false, false, false, false, false, false, false, false, false};
        for (int i = 0; i < 9; i++)
        {
            if (visited[userBoard[i][j] - 1])
            {
                return false;
            }
            else
            {
                visited[userBoard[i][j] - 1] = true;
            }
        }
    }
    return true;
}

bool Board::CheckSquares()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            bool visited[9] = {false, false, false, false, false, false, false, false, false};
            for (int k = i * 3; k < (i + 1) * 3; k++)
            {
                for (int l = j * 3; l < (j + 1) * 3; l++)
                {
                    if (visited[userBoard[k][l] - 1])
                    {
                        return false;
                    }
                    else
                    {
                        visited[userBoard[k][l] - 1] = true;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::CheckSolution()
{
    return CheckColumns() && CheckRows() && CheckSquares();
}

bool Board::AutoSolve()
{
    std::vector<std::vector<int>> tmp = board;
    if (Solve(0,0))
    {
        userBoard = board;
        return true;
    }
    else {
        board = tmp;
        return false;
    }
}

/*
Pseudocode for Solver procedure:
Board -> Solve => (Result, Board)
1. Find minimal free index => Ind
2. Find candidates for guess => Cand(Ind)
3. If Cand(Ind) is empty Board has no solution. Return (False, Empty list)
4. Choose random candidate from Cand(Ind) => Next
5. Insert next at ind
6. Return Board -> Solve
*/

std::vector<int> Board::FindCandidates(int y, int x)
{
    std::vector<int> res;
    bool valid[10] = {false, true, true, true, true, true, true, true, true, true};
    //column
    for (int i = 0; i < 9; i++)
    {
        if (board.at(i).at(x) > 0)
        {
            valid[board.at(i).at(x)] = false;
        }
    }
    //row
    for (int i = 0; i < 9; i++)
    {
        if (board.at(y).at(i) > 0)
        {
            valid[board.at(y).at(i)] = false;
        }
    }
    //squares
    int squareCol = (x / 3) * 3;
    int squareLine = (y / 3) * 3;
    for (int i = squareLine; i < squareLine + 3; i++)
    {
        for (int j = squareCol; j < squareCol + 3; j++)
        {
            if (board.at(i).at(j) > 0)
            {
                valid[board.at(i).at(j)] = false;
            }
        }
    }
    for (int i = 1; i < 10; i++)
    {
        if (valid[i])
        {
            res.push_back(i);
        }
    }
    srand(time(0));
    std::random_shuffle(res.begin(), res.end());
    return res;
}

bool Board::AllFilled()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (!userBoard.at(i).at(j))
            {
                return false;
            }
        }
    }
    return true;
}

//if x, y both equal to -1 then it means that all is filled
void Board::FindMinimal(int &x, int &y)
{
    y = -1;
    x = -1;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board.at(i).at(j) == 0)
            {
                y = i;
                x = j;
                return;
            }
        }
    }
    return;
}

bool Board::Valid(int y, int x, int val)
{
    //column
    for (int i = 0; i < 9; i++)
    {
        if (userBoard.at(i).at(x) == val)
        {
            return false;
        }
    }
    //row
    for (int i = 0; i < 9; i++)
    {
        if (userBoard.at(y).at(i) == val)
        {
            return false;
        }
    }
    //squares
    int squareCol = (x / 3) * 3;
    int squareLine = (y / 3) * 3;
    for (int i = squareLine; i < squareLine + 3; i++)
    {
        for (int j = squareCol; j < squareCol + 3; j++)
        {
            if (board.at(i).at(j) == val)
            {
                return false;
            }
        }
    }
    return true;
}

bool Board::Solve(int y, int x)
{
    if (x == SIZE)
    {
        if (y == SIZE - 1)
        {
            return true;
        }
        else
        {
            y++;
            x = 0;
        }
    }
    if (board.at(y).at(x) > 0)
    {
        return Solve(y, x + 1);
    }
    std::vector<int> candidates = FindCandidates(y, x);
    for (int i = 0; i < candidates.size(); ++i)
    {
        
        board.at(y).at(x) = candidates.at(i);
        if (Solve(y, x + 1))
        {
            return true;
        }
        board.at(y).at(x) = 0;
    }
    return false;
}

void Board::Hide(int difficulty)
{
    int amount = difficulty / SIZE;
    for (int i = 0; i < SIZE; i++)
    {
        std::vector<int> shuffle;
        for (int j = 0; j < SIZE; j++)
        {
            shuffle.push_back(j);
        }
        time_t t = time(nullptr) + rand() % 100 + i;
        srand(t);
        std::random_shuffle(shuffle.begin(), shuffle.end());
        for (int j = 0; j < amount; j++)
        {
            board.at(i).at(shuffle.at(j)) = 0;
        }
    }
}

void Board::Generate(int difficulty)
{
    Solve(0, 0);
    if (difficulty == 1)
    {
        Hide(EASY);
    }
    else if (difficulty == 2)
    {
        Hide(MEDIUM);
    }
    else
    {
        Hide(HARD);
    }
    userBoard = board;
}

void Board::Reset()
{
    std::vector<std::vector<int>> tmp(SIZE, std::vector<int>(SIZE,0));
    board = tmp;
    userBoard = tmp;
}