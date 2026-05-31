#include "GW.h"
#include "Pawns.h"
#include "Place.h"
#include "Controller.h"
#include "Fences.h" 
#include <QShowEvent>
#include <QTimer> 

bool Started = false;

Game_Window::Game_Window(Controller* Master_Controller, widget *parent)
    : widget(parent)
{
    myController = Master_Controller;
    this->setWindowTitle("Game Window");
    this->setWindowIcon(QIcon(":/Images/Icon.png"));
    this->setFixedSize(1200, 600);
    this->setObjectName("Game");
    Game_Style(this);

    P1 = new Pawns(this);
    P2 = new Pawns(this);
    P2->Set_Finish(8);
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

    auto Pawn_Motion_Connection = [this](Place * a , Pawns * pawn ){
        connect(a, &Place::Move_Pawn, this, [this, pawn, a]() {
            Place* oldPos = pawn->Get_Position();
            pawn->movepawn(a , false); 
            if (pawn->Get_Position() == a && oldPos != a) {
                myController->recordPawnMove(pawn, oldPos , a);
                this->Clean_V();          
                if (myController) {
                    myController->switchTurn(); 
                }
                if (pawn == P1 && a->getRow() == 0) {
                    this->Trigger_Win(1);
                } else if (pawn == P2 && a->getRow() == 8) {
                    this->Trigger_Win(2);
                }
            }
        });
    };
    auto Clean_Valid = [this] (Place * a){connect(a ,&Place::Clean , this , &Game_Window::Clean_V);};
    auto Fence_Connection = [this](Fences * f){
        connect(f, &Fences::fenceClicked, this, [this](Fences* clickedFence) {
            if (myController == nullptr || clickedFence == nullptr) return; 
            bool Fence_Owner = Pawns::Turn;

            bool success = myController->placeFence(clickedFence->getRow(), clickedFence->getCol(), clickedFence->getIsHorizontal() , Fence_Owner, P1 , P2);
            
            if (success) {
                myController->recordFenceMove(clickedFence, Fence_Owner);
                clickedFence->placeVisually(); 
                
                this->Update_UI();  
                P1->Set_Choosen(false); 
                P2->Set_Choosen(false); 
       
                this->Clean_V(); 
                myController->switchTurn();       
                
            }
        });
    };
    
    for (int row = 8; row >= 0; row--) {
        for (int col = 8; col >= 0; col--) {
            Place* square = new Place(row, col, this);
            Board_Layout->addWidget(square, row * 2, col * 2); 
            myController->boardData[row][col] = square; 
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
    vbox* Left_VBox = new vbox(Left);

    widget* Left_Top = new widget(Left);
    Left_Top->setStyleSheet("background: transparent;");

    widget* Left_Middle = new widget(Left);
    Left_Middle->setObjectName("LeftMiddle");
    Left_Middle->setStyleSheet("QWidget#LeftMiddle { background-color: rgba(0, 0, 0, 150); border-radius: 20px; }");
    
    vbox* Left_Middle_Layout = new vbox(Left_Middle);
    
    P1_Status_Label = new label("", Left_Middle); 
    P1_Status_Label->setAlignment(Qt::AlignCenter);
    Left_Middle_Layout->addWidget(P1_Status_Label);
    
    label* P1_Name = new label("Player 1", Left_Middle);
    P1_Name->setStyleSheet("color: #048eeb; font-size: 26px; font-weight: bold; background: transparent;");
    P1_Name->setAlignment(Qt::AlignCenter);

    P1_Fences_Label = new label("Fences: 10", Left_Middle);
    P1_Fences_Label->setStyleSheet("color: white; font-size: 20px; background: transparent;");
    P1_Fences_Label->setAlignment(AlignCenter);

    Left_Middle_Layout->addWidget(P1_Name);
    Left_Middle_Layout->addWidget(P1_Fences_Label);

    widget* Left_Bottom = new widget(Left);
    Left_Bottom->setStyleSheet("background: transparent;");
    vbox* Left_Bottom_Container = new vbox(Left_Bottom);
    hbox* Left_Bottom_Layout = new hbox(Left_Bottom); 

    Main_Menu_Btn = new Button("Main Menu", Left_Bottom);
    Main_Menu_Btn->setFixedSize(120, 40);
    Main_Menu_Btn->setStyleSheet("background-color: #333; color: white; font-weight: bold; border-radius: 10px;");

    Undo_Btn = new Button("Undo", Left_Bottom);
    Undo_Btn->setFixedSize(80, 40);
    Undo_Btn->setStyleSheet("background-color: #e6b800; color: black; font-weight: bold; border-radius: 10px;");
    Left_Bottom_Layout->addWidget(Undo_Btn);

    Redo_Btn = new Button("Redo", Left_Bottom);
    Redo_Btn->setFixedSize(80, 40);
    Redo_Btn->setStyleSheet("background-color: #e6b800; color: black; font-weight: bold; border-radius: 10px;");
    Left_Bottom_Layout->addWidget(Redo_Btn);

    Left_Bottom_Container->addLayout(Left_Bottom_Layout);
    Left_Bottom_Container->addWidget(Main_Menu_Btn);

    connect(Undo_Btn, &Button::clicked, this, [this]() {
        if (myController == nullptr || myController->moveStack.empty()) return; 
        Move lastMove = myController->popLastMove();
        myController->redoStack.push(lastMove);
        if (!lastMove.isFence) {
            lastMove.movedPawn->movepawn(lastMove.oldPlace , true);
            P1_Status_Label->setText("");
            P2_Status_Label->setText("");
        } 
        else {
            lastMove.placedFence->resetVisually(); 
            myController->undoFence(lastMove.placedFence->getRow(), 
                                    lastMove.placedFence->getCol(), 
                                    lastMove.placedFence->getIsHorizontal(), 
                                    lastMove.playerTurn);
        }
        myController->switchTurn();
        this->Clean_V();
        this->Update_UI();
    });
    connect(Redo_Btn, &Button::clicked, this, [this]() {
        if (myController == nullptr || myController->redoStack.empty()) return; 
        Move redoMove = myController->redoStack.top();
        myController->redoStack.pop();
        myController->moveStack.push(redoMove);

 
        if (!redoMove.isFence) {
            redoMove.movedPawn->movepawn(redoMove.newPlace, true); 
            redoMove.movedPawn->raise(); 
        } 
        else {
            myController->placeFence(redoMove.placedFence->getRow(), 
                                     redoMove.placedFence->getCol(), 
                                     redoMove.placedFence->getIsHorizontal(),
                                     redoMove.playerTurn , P1 , P2);
            redoMove.placedFence->placeVisually(); 
        }
        myController->switchTurn();
        this->Clean_V();
        this->Update_UI();
    });

    connect(Main_Menu_Btn, &Button::clicked, this, [this]() {
    Main_Menu_Btn->setEnabled(false);
    Pawns::Turn = false;
    this->hide();

    Controller* oldController = myController;
    myController = nullptr;

    Controller* fresh = new Controller();
    fresh->start();

    QTimer::singleShot(200, [oldController]() {
        oldController->detachWindows();
        delete oldController;
    });
});
    Left_VBox->addWidget(Left_Top);
    Left_VBox->addWidget(Left_Middle);
    Left_VBox->addWidget(Left_Bottom);

    widget* Right = new widget(this);
    Right->setFixedSize(300, 600);
    Right->setStyleSheet("background: transparent;");
    vbox* Right_VBox = new vbox(Right);

    widget* Right_Top = new widget(Right);
    Right_Top->setStyleSheet("background: transparent;");

    widget* Right_Middle = new widget(Right);
    Right_Middle->setObjectName("RightMiddle");
    Right_Middle->setStyleSheet("QWidget#RightMiddle { background-color: rgba(0, 0, 0, 150); border-radius: 20px; }");
    
    vbox * Right_Middle_Layout = new vbox(Right_Middle);
    
    label * P2_Name = new label("Player 2", Right_Middle);
    P2_Name->setStyleSheet("color: #f54803; font-size: 26px; font-weight: bold; background: transparent;");
    P2_Name->setAlignment(AlignCenter);

    P2_Fences_Label = new label("Fences: 10", Right_Middle);
    P2_Fences_Label->setStyleSheet("color: white; font-size: 20px; background: transparent;");
    P2_Fences_Label->setAlignment(AlignCenter);
    P2_Status_Label = new label("", Right_Middle); 
    P2_Status_Label->setAlignment(AlignCenter);

    Right_Middle_Layout->addWidget(P2_Status_Label);
    Right_Middle_Layout->addWidget(P2_Name);
    Right_Middle_Layout->addWidget(P2_Fences_Label);

    widget * Right_Bottom = new widget(Right);
    Right_Bottom->setStyleSheet("background: transparent;");

    Right_VBox->addWidget(Right_Top);
    Right_VBox->addWidget(Right_Middle);
    Right_VBox->addWidget(Right_Bottom);

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
        if (myController->boardData[8][4] != nullptr) {
            P1->Set_Choosen(true);
            P1->movepawn(myController->boardData[8][4] , true);
            P1->raise();
            P2->Set_Id(true);
        }
        if(myController->boardData[0][4] != nullptr)
        {
            P2->Set_Choosen(true);
            P2->movepawn(myController->boardData[0][4] , true);
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
        if(myController->boardData[r][c]->Get_Has_Pawn()) continue; 
       myController-> boardData[r][c]->Change_Color(true);
        myController->boardData[r][c]->Set_Available(true);
    }
}

void Game_Window::Clean_V()
{
    for(int i= 0; i < 9 ; i++)
    {
        for(int j =0 ; j < 9 ; j++)
        {
            if(myController->boardData[i][j]->Get_Available())
            {
                myController->boardData[i][j]->Set_Available(false);
                myController->boardData[i][j]->Change_Color(false);
            }
        }
    }
}

void Game_Window::Update_UI()
{
    if (P1_Fences_Label != nullptr && P2_Fences_Label != nullptr && myController != nullptr) 
    {
        P1_Fences_Label->setText("Fences: " + QString::number(myController->getP1Fences()));
        P2_Fences_Label->setText("Fences: " + QString::number(myController->getP2Fences()));
    }
}

void Game_Window::Trigger_Win(int winner_player)
{
    if (P1_Status_Label == nullptr || P2_Status_Label == nullptr) return;

    if (winner_player == 1) {
        P1_Status_Label->setText("WINNER!");
        P1_Status_Label->setStyleSheet("color: #00ff00; font-size: 30px; font-weight: 900; background: transparent;");
        
        P2_Status_Label->setText("LOSER!");
        P2_Status_Label->setStyleSheet("color: #ff0000; font-size: 30px; font-weight: 900; background: transparent;");
    } 
    else if (winner_player == 2) {
        P2_Status_Label->setText("WINNER!");
        P2_Status_Label->setStyleSheet("color: #00ff00; font-size: 30px; font-weight: 900; background: transparent;");
        
        P1_Status_Label->setText("LOSER!");
        P1_Status_Label->setStyleSheet("color: #ff0000; font-size: 30px; font-weight: 900; background: transparent;");
    }

    if (Main_Menu_Btn != nullptr) {
        Main_Menu_Btn->show();
    }
}