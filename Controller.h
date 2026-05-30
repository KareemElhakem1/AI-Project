#pragma once
#include <vector>
#include <utility>
class Choose_Mode_Window;
class Game_Window;

class Controller {
private:
    Choose_Mode_Window * Win;
    Game_Window * Game;
    bool p1Turn; 
    bool hFences[8][8]; // Horizontal fences
    bool vFences[8][8]; // Vertical fences
    int p1FencesLeft;
    int p2FencesLeft;

public:
    Controller();
    void start(); 
    std::vector<std::pair<int, int>> getValidMoves(int row, int col);
    bool isPlayer1Turn() const;
    void switchTurn();
    bool canPlaceFence(int row, int col, bool isHorizontal);
    bool placeFence(int row, int col, bool isHorizontal);
   // void playAITurn();
};