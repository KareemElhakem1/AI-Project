#pragma once
#include "Definitions.h"
#include "Pawns.h"
#include "Place.h"
#include "Controller.h"
#include <QPainter>
#include <QStyleOption>
class Game_Window : public widget{
    Q_OBJECT
    private:
    Grid * Board_Layout;
    hbox * Master_Layout;
    Pawns* P1;
    Pawns* P2;
    
    void Game_Style(widget * home);
    Controller* myController;
    widget * Left_Card;
    widget* Right_Card;
    label* P1_Fences_Label;
    label * P2_Fences_Label;
    label* P1_Status_Label;
    label* P2_Status_Label;
    Button* Main_Menu_Btn;
    protected:
    void paintEvent(QPaintEvent* event) override {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

    public:
    explicit Game_Window(Controller*  Master_Controller, QWidget *parent = nullptr);
    void showEvent(QShowEvent* event);
    public slots:
    void Valid_Moves(Pawns * P );
    void Clean_V();
    void Update_UI();
    void Trigger_Win(int winner_player);
};
