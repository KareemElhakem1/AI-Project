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
    Place* boardData[9][9];
    void Game_Style(widget * home);
    Controller* myController;
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
};
