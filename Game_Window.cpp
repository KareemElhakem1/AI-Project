#include "GW.h"
#include "Pawns.h"
#include "Place.h"
#include <QShowEvent>
#include <QTimer> 
Game_Window::Game_Window()
{
    this->setWindowTitle("Game Window");
    this->setWindowIcon(QIcon(":/Images/Icon.png"));
    this->setFixedSize(1200, 600);
    this->setObjectName("Game");
    Game_Style(this);
    // Initialize Pawns as member pointers
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
    Board_Layout->setVerticalSpacing(10);
    Board_Layout->setHorizontalSpacing(10); 

    // Build the Grid
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            Place* square = new Place(row, col, this);
            Board_Layout->addWidget(square, row, col);
            boardData[row][col] = square; // Store in member array
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
    connect(P1, &Pawns::pawnClicked, this, [this](Pawns* p) {
    this->Valid_Moves(p);
    });
    connect(P2, &Pawns::pawnClicked, this, [this](Pawns* p) {
    this->Valid_Moves(p);
    });
    this->setLayout(Master_Layout);
}
void Game_Window::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    QTimer::singleShot(0, this, [this]() {
        if (boardData[8][4] != nullptr) {
            P1->Set_Position(boardData[8][4]);
            P1->raise();
        }
        if(boardData[0][4] != nullptr)
        {
            P2->Set_Position(boardData[0][4]);
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
void Game_Window :: Valid_Moves(Pawns * P)
{
    Place * Current = P->Get_Position();
    int row = Current->getRow();
    int col = Current->getCol();
    for(int i= 0; i < 9 ; i++)
    {
        for(int j =0 ; j < 9 ; j++)
        {
            if((j == col-1 && i == row ) || (j == col+1 && i == row )
               || (j == col && i == row -1) || (j == col && i == row +1))
               {
                boardData[i][j]->Change_Color();
                boardData[i][j]->Set_Available();
               }
        }
    }
    
}