#pragma once
#include <vector>
#include <utility>
#include "Pawns.h"
#include <stack>
#include "Moves.h" 
class Choose_Mode_Window;
class Game_Window;

class Controller {
private:
    Choose_Mode_Window * Win;
    Game_Window * Game;
    widget * Invalid_Placement_Widget;
    label * Invalid_Placemnt_Label;
    bool p1Turn; 
    bool hFences[8][8]; 
    bool vFences[8][8]; 
    int p1FencesLeft;
    int p2FencesLeft;
    

public:
    Controller();
    void start();
    bool AI_Mode = false; 
    void AI_Algorithm(Pawns * P1 , Pawns * P2);
    vector<pair<int, int>> getValidMoves(int row, int col);
    bool isPlayer1Turn() const;
    void switchTurn();
    bool canPlaceFence(int row, int col, bool isHorizontal);
    bool placeFence(int row, int col, bool isHorizontal , bool playerTurn , Pawns * p1 , Pawns * p2);
    int getP1Fences() const;
    int getP2Fences() const;
    Place* boardData[9][9] = {nullptr , nullptr};
    Fences * hboardFences[8][8] = {nullptr , nullptr};
    Fences * vboardFences[8][8] = {nullptr , nullptr};
    void Pawn_Meet(int row , int col , int Relative , vector<pair<int, int>>& valid_moves);
    void detachWindows() {
    Win = nullptr;
    Game = nullptr;}
    pair<bool , int> BFS( Pawns * P);
    stack<Move> moveStack;
    stack<Move> redoStack;
    void recordPawnMove(Pawns* p, Place* oldPlace, Place* newPlace);
    void recordFenceMove(Fences* f, bool playerTurn);
    Move popLastMove();
    void undoFence(int r, int c, bool isH, bool playerTurn);
    void Show_Invalid_window();
    void Switch_Window(Button* a , Button * b ,  widget* first, widget* second);
    Pawns * p1;
    Pawns * p2;
    
};