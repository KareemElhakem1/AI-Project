#include "Controller.h"
#include "GW.h"
#include "CMW.h"
#include <queue>
Controller::Controller() {
    Win = nullptr;
    Game = nullptr;
    p1Turn = true; 

    p1FencesLeft = 10;
    p2FencesLeft = 10;
    for(int r = 0; r < 8; r++) {
        for(int c = 0; c < 8; c++) {
            hFences[r][c] = false;
            vFences[r][c] = false;
        }
    }
}

void Controller::start()
{
    Win = new Choose_Mode_Window();
    Game = new Game_Window(this);
    Win->show();
    Switch_Window(Win->get_button(), Win, Game);
} 

bool Controller::canPlaceFence(int r, int c, bool isHorizontal) {
    if (r < 0 || r > 7 || c < 0 || c > 7) return false;
    if (isHorizontal) {
        if (hFences[r][c]) return false; 
        if (c > 0 && hFences[r][c-1]) return false; 
        if (c < 7 && hFences[r][c+1]) return false; 
        if (vFences[r][c]) return false; 
    } else {
        if (vFences[r][c]) return false;
        if (r > 0 && vFences[r-1][c]) return false; 
        if (r < 7 && vFences[r+1][c]) return false; 
        if (hFences[r][c]) return false; 
    }
    
    return true;
}

bool Controller::placeFence(int r, int c, bool isHorizontal, bool playerPlacingFence , Pawns * p1 , Pawns * p2) {
    if (playerPlacingFence == false && p1FencesLeft <= 0) {
        return false; 
    } 
    else if (playerPlacingFence == true && p2FencesLeft <= 0) {
        return false;
    }
    if (!canPlaceFence(r, c, isHorizontal)) return false;
    if (isHorizontal) {
        hFences[r][c] = true;
    } else {
        vFences[r][c] = true;
    }
    if (playerPlacingFence == false) {
        p1FencesLeft--;
    } else {
        p2FencesLeft--;
    }
    if(!(BFS(p1) && BFS(p2))) 
    {
    undoFence(r , c , isHorizontal , playerPlacingFence);
    switchTurn();
    Show_Invalid_window();
    return false;
    }
    return true;
}
vector<pair<int, int>> Controller::getValidMoves(int row, int col) {
    vector<pair<int, int>> valid_moves;
    int Relative_Position =0;
    
    if (row > 0) {
        bool blocked = (col < 8 && hFences[row-1][col]) || (col > 0 && hFences[row-1][col-1]);
        bool pawnAhead = boardData[row-1][col]->Get_Has_Pawn();
    
        if (!blocked && !pawnAhead) valid_moves.push_back({row-1, col});
        if (!blocked && pawnAhead)  Pawn_Meet(row, col, 0, valid_moves);
    }
    if (row < 8) {
        bool blocked = (col < 8 && hFences[row][col]) || (col > 0 && hFences[row][col-1] );
        bool pawnDown = boardData[row+1][col]->Get_Has_Pawn();
    
        if (!blocked && !pawnDown) valid_moves.push_back({row+1, col});
        if (!blocked && pawnDown)  Pawn_Meet(row, col, 1, valid_moves);
    }
    if (col > 0) {
        bool blocked = (row < 8 && vFences[row][col-1]) || (row > 0 && vFences[row-1][col-1] );
        bool pawnLeft = boardData[row][col-1]->Get_Has_Pawn();
    
        if (!blocked && !pawnLeft) valid_moves.push_back({row, col-1});
        if (!blocked && pawnLeft)  Pawn_Meet(row, col, 2, valid_moves);
    }
    if (col < 8) {
        
        bool blocked = (row < 8 && vFences[row][col]) || (row > 0 && vFences[row-1][col]);
        bool pawnRight = boardData[row][col+1]->Get_Has_Pawn();
    
        if (!blocked && !pawnRight) valid_moves.push_back({row, col+1});
        if (!blocked && pawnRight)  Pawn_Meet(row, col, 3, valid_moves);
    }
    
    return valid_moves;
}

bool Controller::isPlayer1Turn() const {
    return p1Turn;
}

void Controller::switchTurn() {
    p1Turn = !p1Turn;
    Pawns::Turn = !Pawns::Turn;
}
int Controller::getP1Fences() const { return p1FencesLeft; }
int Controller::getP2Fences() const { return p2FencesLeft; }
void Controller::Pawn_Meet(int row, int col, int Relative, vector<pair<int, int>>& valid_moves)
{
    switch (Relative) {
        case 0: 
        {
            bool blocked = (col < 8 && hFences[row-2][col]) || (col > 0 && hFences[row-2][col-1]);
            if (!blocked) { valid_moves.push_back({row-2, col}); break; }

            bool rightBlocked = (col >= 8) || (vFences[row-1][col]   || vFences[row][col]);
            bool leftBlocked  = (col <= 0) || (vFences[row-1][col-1] || vFences[row][col-1]);

            if (!rightBlocked) valid_moves.push_back({row-1, col+1});
            if (!leftBlocked)  valid_moves.push_back({row-1, col-1});
            break;
        }
        case 1: 
        {
            bool blocked = (col < 8 && hFences[row+1][col]) || (col > 0 && hFences[row+1][col-1]);
            if (!blocked) { valid_moves.push_back({row+2, col}); break; }

            bool rightBlocked = (col >= 8) || (vFences[row][col]   || vFences[row+1][col]);
            bool leftBlocked  = (col <= 0) || (vFences[row][col-1] || vFences[row+1][col-1]);

            if (!rightBlocked) valid_moves.push_back({row+1, col+1});
            if (!leftBlocked)  valid_moves.push_back({row+1, col-1});
            break;
        }
        case 2: 
        {
            bool blocked = (row < 8 && vFences[row][col-2]) || (row > 0 && vFences[row-1][col-2]);
            if (!blocked) { valid_moves.push_back({row, col-2}); break; }

            bool upBlocked   = (row <= 0) || (hFences[row-1][col-1] || hFences[row-1][col]);
            bool downBlocked = (row >= 8) || (hFences[row][col-1]   || hFences[row][col]);

            if (!upBlocked)   valid_moves.push_back({row-1, col-1});
            if (!downBlocked) valid_moves.push_back({row+1, col-1});
            break;
        }
        case 3: 
        {
            bool blocked = (row < 8 && vFences[row][col+1]) || (row > 0 && vFences[row-1][col+1]);
            if (!blocked) { valid_moves.push_back({row, col+2}); break; }

            bool upBlocked   = (row <= 0) || (hFences[row-1][col+1] || hFences[row-1][col]);
            bool downBlocked = (row >= 8) || (hFences[row][col+1]   || hFences[row][col]);

            if (!upBlocked)   valid_moves.push_back({row-1, col+1});
            if (!downBlocked) valid_moves.push_back({row+1, col+1});
            break;
        }
    }
}
bool Controller :: BFS( Pawns * P  )
{
    
    if(P->Get_Position() == nullptr){qDebug() << "  -> BFS Aborted: Pawn position is nullptr."; return true;}
    
    vector <pair<int , int >> neighbours ;
    Place * Current = nullptr;
    int Finish = P->Get_Finish();
    Place * start = P->Get_Position();
    int Row;
    int Col ;
    
    for(int i =0 ; i < 9 ; i++)
    {
        for(int j =0 ; j < 9 ; j++)
        {
            boardData[i][j]->Set_Mark(false);
        }
    }
   
    queue <Place *> To_Visit;
    To_Visit.push(start);
    start->Set_Mark(true);
    
    int a , b;
    while(!To_Visit.empty())
    {
        if(To_Visit.front()->getRow() == Finish) return true; 
        Current = To_Visit.front();
        Row = Current->getRow();
        Col = Current->getCol(); 
        To_Visit.pop();
        neighbours = getValidMoves(Row , Col);
        for(int i = 0; i < neighbours.size(); i++)
            {
                a = neighbours[i].first;
                b = neighbours[i].second;
                if (a < 0 || a > 8 || b < 0 || b > 8) {
                continue; 
            }
                if(!boardData[a][b]->Get_Mark())
                {
                     boardData[a][b]->Set_Mark(true);
                     To_Visit.push(boardData[a][b]);
                }
            }
    }
    return false;
}
void Controller::recordPawnMove(Pawns* p, Place* oldPlace, Place* newPlace) 
{
    Move newMove(p, oldPlace, newPlace);
    moveStack.push(newMove);
    
 
    while(!redoStack.empty()) redoStack.pop(); 
}

void Controller::recordFenceMove(Fences* f, bool playerTurn) 
{
    Move newMove(f, playerTurn);
    moveStack.push(newMove);
    
    
    while(!redoStack.empty()) redoStack.pop();
}

Move Controller::popLastMove() 
{
    Move last = moveStack.top();
    moveStack.pop();
    return last;
}
void Controller::undoFence(int r, int c, bool isH, bool playerTurn) 
{
    if (isH) {
        hFences[r][c] = false;
    } else {
        vFences[r][c] = false;
    }
    if (playerTurn == false) {
        p1FencesLeft++;
        return;
    } 
    p2FencesLeft++;
    
}
void Controller::Show_Invalid_window()
{
    Invalid_Placement_Widget = new widget();
    Invalid_Placement_Widget->setWindowTitle("Invalid Placement Error");
    Invalid_Placement_Widget->setWindowIcon(QIcon(":/Images/Icon.png"));
    Invalid_Placement_Widget->setFixedSize(400 , 100);
    Invalid_Placemnt_Label = new label( Invalid_Placement_Widget);
    Invalid_Placemnt_Label->setText("  This is placement is invalid  <br> as it prevents a player from reaching the baseline of their opponent");
    Invalid_Placemnt_Label->setAlignment(AlignCenter);
    Invalid_Placemnt_Label->setStyleSheet("font-weight: bold;");
    Invalid_Placement_Widget->show();
}