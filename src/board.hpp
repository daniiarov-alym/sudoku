#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>

class Board
{
public:
    static const int SIZE = 9;
    
private:
    static const int EASY = 27;
    static const int MEDIUM = 45;
    static const int HARD = 72;
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> userBoard;
    void FindMinimal(int &y, int &x);
    std::vector<int> FindCandidates(int y, int x);
    void Hide(int difficulty);
    bool AllFilled();
public:
    Board();
    std::vector<std::vector<int>> GetBoard()
    {
        return board;
    }
    std::vector<std::vector<int>> GetUserBoard()
    {
        return userBoard;
    }
    
    void Generate(int difficulty);
    
    void Set(int y, int x, int val, bool editMode);
    bool Solve(int y, int x);
    bool Valid(int y, int x, int val);
};

#endif