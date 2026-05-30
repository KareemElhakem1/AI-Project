#include "Place.h"

Place::Place(int r, int c, widget *parent) 
    : QWidget(parent), Has_Pawn(false), row(r), col(c) 
{
    this->setFixedSize(50, 50);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("background-color: #bdc3c7; border: 1px solid black; border-radius: 1px");
}
Place :: Place(Place * a)
{

}
bool Place::hasPawn() const {
    return Has_Pawn;
}

void Place::setPawn(bool status) {
    Has_Pawn = status;
}

point  Place::getCenter() const {
    return this->geometry().center();
}

int Place::getRow() const { return row; }
int Place::getCol() const { return col; }
void Place::Change_Color(bool a){
    if(a)
    this->setStyleSheet("background-color: #26e40d; border: 1px solid black; border-radius: 1px");
    else
    this->setStyleSheet("background-color: #bdc3c7; border: 1px solid black; border-radius: 1px");

}
void Place::Set_Available( bool b)
{
    Available = b;
}
void Place :: mousePressEvent(mevent * event) 
{
    if(this->Available == true )
    {
    
        emit Move_Pawn(this);
        emit Clean();
    }
}
bool Place::Get_Available()
{
    return Available;
}
void Place:: Set_Has_Pawn( bool x){
 Has_Pawn = x;
}
bool Place::Get_Has_Pawn()
{
    return Has_Pawn;
}