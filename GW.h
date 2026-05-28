#pragma once
#include "Definitions.h"
#include "Pawns.h"
#include "Place.h"
#include <QPainter>
#include <QStyleOption>
class Game_Window : public widget{
    Q_OBJECT
    private:
    Grid * Board_Layout;
    hbox * Master_Layout;
    Pawns* P1;
    Pawns* P2;
    Place* boardData[9][9];
    void Game_Style(widget * home);
    protected:
    void paintEvent(QPaintEvent* event) override {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    public:
    Game_Window();
    void showEvent(QShowEvent* event);
    public slots:
    void Valid_Moves(Pawns * P);
    
};
