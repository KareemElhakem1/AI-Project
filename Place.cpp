#include "Place.h"

Place::Place(int r, int c, QWidget *parent) 
    : QWidget(parent), has_pawn(false), row(r), col(c) 
{
    this->setMinimumSize(50, 50);
    this->setStyleSheet("background-color: #bdc3c7; border: 1px solid black; border-radius: 1px");
}

bool Place::hasPawn() const {
    return has_pawn;
}

void Place::setPawn(bool status) {
    has_pawn = status;
}

point  Place::getCenter() const {
    return this->geometry().center();
}

int Place::getRow() const { return row; }
int Place::getCol() const { return col; }
void Place::Change_Color(){
    this->setStyleSheet("background-color: #26e40d; border: 1px solid black; border-radius: 1px");
}
void Place::Set_Available()
{
    Available = true;
}