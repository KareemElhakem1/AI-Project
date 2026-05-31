#include "Controller.h"
#include "GW.h"
#include "CMW.h"
#include <queue>
Controller::Controller() {
    Win = nullptr;
    Game = nullptr;
    p1Turn = true; 
    AI_Mode = false;
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
    Switch_Window(Win->get_button(true), Win->get_button(false), Win, Game);
} 
void Controller::Switch_Window(Button* a , Button * b ,  widget* first, widget* second){
 QObject::connect(a, &Button::clicked, [=]() {
    if (first != nullptr && second != nullptr) {
        AI_Mode = false;
        cout <<"False";
        first->hide();
        second->show();
    }
   }); 
 QObject::connect(b, &Button::clicked, [=]() {
    if (first != nullptr && second != nullptr) {
        AI_Mode = true;
        if (AI_Mode) cout <<"aaa9";
        first->hide();
        second->show();
    }
});
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
    if(!(BFS(p1).first && BFS(p2).first)) 
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
    if(Pawns::Turn == true && AI_Mode) AI_Algorithm( p1 , p2);
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
pair<bool , int> Controller :: BFS( Pawns * P  )
{
    vector <pair<int , int >> neighbours ;
    pair <Place *, int> Current ;
    int Finish = P->Get_Finish();
    pair <Place * , int> start = {P->Get_Position() , 0};
    int Row;
    int Col ;
    
    for(int i =0 ; i < 9 ; i++)
    {
        for(int j =0 ; j < 9 ; j++)
        {
            boardData[i][j]->Set_Mark(false);
        }
    }
   
    queue <pair<Place *, int>> To_Visit;
    To_Visit.push(start);
    start.first->Set_Mark(true);
    
    int a , b;
    while(!To_Visit.empty())
    {
        Current = To_Visit.front();
        if(Current.first->getRow() == Finish) return {true ,Current.second }; 
        Row = Current.first->getRow();
        Col = Current.first->getCol(); 
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
                     To_Visit.push({boardData[a][b], Current.second +1});
                }
            }
    }
    return {false , -1};
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
void Controller::AI_Algorithm(Pawns * P1 , Pawns * P2)
{
    int Minimum_Steps = BFS(P2).second ;
    if(Minimum_Steps > BFS(P1).second&& p2FencesLeft > 0)
    {
        int max_P1_path = -1;
        int best_fence_r = -1;
        int best_fence_c = -1;
        bool best_is_horizontal = true;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (canPlaceFence(r, c, true)) {
                    hFences[r][c] = true; 
                    if (BFS(P1).first == true && BFS(P2).first == true) {
                        int p1_distance = BFS(P1).second;
                        if (p1_distance > max_P1_path) {
                            max_P1_path = p1_distance;
                            best_fence_r = r;
                            best_fence_c = c;
                            best_is_horizontal = true;
                        }
                    }
                    hFences[r][c] = false; 
                }
                if (canPlaceFence(r, c, false)) {
                    vFences[r][c] = true; 
                    
                    if (BFS(P1).first == true && BFS(P2).first == true) {
                        int p1_distance = BFS(P1).second;
                        
                        if (p1_distance > max_P1_path) {
                            max_P1_path = p1_distance;
                            best_fence_r = r;
                            best_fence_c = c;
                            best_is_horizontal = false;
                        }
                    }
                    vFences[r][c] = false;
                }
            }
        }
        Fences * Choosen_Fence ;
        if(best_is_horizontal)
        {
        Choosen_Fence= hboardFences[best_fence_r][best_fence_c];
        emit  Choosen_Fence -> fenceClicked(Choosen_Fence , true) ;
        }
        else
        {
            Choosen_Fence= vboardFences[best_fence_r][best_fence_c];
            emit Choosen_Fence->fenceClicked(Choosen_Fence, true);
        }
    }
    else {
    Place * Current_Location = P2->Get_Position();
    Pawns * Doll_Pawn = new Pawns(nullptr);
    Doll_Pawn->Set_Position(Current_Location); 
    Doll_Pawn->Set_Finish( P2->Get_Finish() );
    Current_Location->Set_Has_Pawn(false);
    Place * Step;
    Place * Suggested;
    int Minimum_Suggested=0;
    vector<pair<int, int>> Available_Moves =  getValidMoves(Current_Location->getRow(), Current_Location-> getCol());
    for(int i=0; i < Available_Moves.size(); i++)
    {
        Suggested = boardData[Available_Moves[i].first][Available_Moves[i].second];
        Doll_Pawn->Set_Position(Suggested);
        Minimum_Suggested = BFS(Doll_Pawn).second;
        
        if(Minimum_Suggested== Minimum_Steps-1 || Minimum_Suggested== 0)
        {
            Current_Location->Set_Has_Pawn(true);
               emit Suggested->Move_Pawn(Suggested , true);
               emit Suggested -> Clean();
              break;
        }
    }
    delete Doll_Pawn;
}
}
