#include "GW.h"
#include "Pawns.h"
#include "Place.h"
#include "Controller.h"
#include "Fences.h" 
#include <QShowEvent>
#include <QTimer> 

bool Started = false;

Game_Window::Game_Window(Controller* Master_Controller, QWidget *parent)
    : QWidget(parent)
{
    myController = Master_Controller;
    this->setWindowTitle("Game Window");
    this->setWindowIcon(QIcon(":/Images/Icon.png"));
    this->setFixedSize(1200, 600);
    this->setObjectName("Game");
    Game_Style(this);

    P1 = new Pawns(this);
    P2 = new Pawns(this);
   
    P1->setAttribute(Qt::WA_StyledBackground, true);
    P2->setAttribute(Qt::WA_StyledBackground, true);
    P1->setFixedSize(40, 40);
    P2->setFixedSize(40, 40);
    P1->setStyleSheet("background-color: #048eeb; border: 1px solid black; border-radius: 20px;");
    P2->setStyleSheet("background-color: #f54803; border: 1px solid black; border-radius: 20px;");

    Board_Layout = new Grid();
    Master_Layout = new hbox();

    Board_Layout->setVerticalSpacing(0);
    Board_Layout->setHorizontalSpacing(0); 

    auto Pawn_Connection = [this](Pawns * Pawn ){connect(Pawn, &Pawns::pawnClicked, this, [this ](Pawns* p ) {
        this->Valid_Moves(p);
    });}; 

    auto Pawn_Motion_Connection = [this](Place * a , Pawns * pawn ){connect (a, &Place::Move_Pawn , pawn , &Pawns::movepawn);};
    auto Clean_Valid = [this] (Place * a){connect(a ,&Place::Clean , this , &Game_Window::Clean_V);};
    auto Fence_Connection = [this](Fences * f){
        connect(f, &Fences::fenceClicked, this, [this](Fences* clickedFence) {
            bool success = myController->placeFence(clickedFence->getRow(), clickedFence->getCol(), clickedFence->getIsHorizontal());
            if (success) {
                clickedFence->placeVisually(); 
            }
        });
    };
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            Place* square = new Place(row, col, this);
            Board_Layout->addWidget(square, row * 2, col * 2); 
            boardData[row][col] = square; 
            Pawn_Motion_Connection(square, P1);
            Pawn_Motion_Connection(square, P2);
            Clean_Valid(square);
            if (row < 8 && col < 8) {
                Fences* hFence = new Fences(row, col, true, this);
                Board_Layout->addWidget(hFence, row * 2 + 1, col * 2, 1, 3);
                hFence->raise(); 
                Fence_Connection(hFence);
                Fences* vFence = new Fences(row, col, false, this);
                Board_Layout->addWidget(vFence, row * 2, col * 2 + 1, 3, 1);
                vFence->raise(); 
                Fence_Connection(vFence);
                widget * spacer = new widget(this);
                spacer->setFixedSize(10, 10);
                spacer->setStyleSheet("background-color: transparent;");
                Board_Layout->addWidget(spacer, row * 2 + 1, col * 2 + 1);
            }
        }
    }
    
    widget* Left = new widget(this);
    Left->setFixedSize(300, 600);
    Left->setStyleSheet("background: transparent;");

    widget* Right = new widget(this);
    Right->setFixedSize(300, 600);
    Right->setStyleSheet("background: transparent;");

    Master_Layout->addWidget(Left);
    Master_Layout->addLayout(Board_Layout);
    Master_Layout->addWidget(Right);

    Pawn_Connection(P1);
    Pawn_Connection(P2);
    this->setLayout(Master_Layout);
}

void Game_Window::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    QTimer::singleShot(0, this, [this]() {
        if (boardData[8][4] != nullptr) {
            P1->Set_Choosen(true);
            P1->movepawn(boardData[8][4]);
            P1->raise();
            P2->Set_Id(true);
        }
        if(boardData[0][4] != nullptr)
        {
            P2->Set_Choosen(true);
            P2->movepawn(boardData[0][4]);
            P2->raise();
        }
    });
}

void Game_Window::Game_Style(widget * home)
{
    home->setStyleSheet("QWidget#Game { "
                "background-image: url(':/Images/Background2.png'); "
                "background-position: center; "
                "} "
                );
}

void Game_Window::Valid_Moves(Pawns * P)
{
    Place * Current = P->Get_Position();
    int row = Current->getRow();
    int col = Current->getCol();
    
    vector<pair<int, int>> valid_coords = myController->getValidMoves(row, col);
    for (auto coord : valid_coords) {
        int r = coord.first;
        int c = coord.second;
        if(boardData[r][c]->Get_Has_Pawn()) continue; 
        boardData[r][c]->Change_Color(true);
        boardData[r][c]->Set_Available(true);
    }
}

void Game_Window::Clean_V()
{
    for(int i= 0; i < 9 ; i++)
    {
        for(int j =0 ; j < 9 ; j++)
        {
            if(boardData[i][j]->Get_Available())
            {
                boardData[i][j]->Set_Available(false);
                boardData[i][j]->Change_Color(false);
            }
        }
    }
}