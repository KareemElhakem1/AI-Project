#include "Pawns.h"
Pawns :: Pawns(widget * parent) : QWidget(parent)
{
    Parent = parent;
    this->raise();
}
void Pawns :: mousePressEvent(mevent * event)
{
    if(Parent == nullptr) return;
    if (event->button() == LeftButton && Press_Id == Turn) {
        
        Is_Pressed = true;    
        emit pawnClicked(this);
    }
}

void Pawns::movepawn(Place* On , bool undo)
{
    if (!Is_Pressed && !undo) return;
    point center = On->getCenter();
    int x = center.x() - (this->width() / 2);
    int y = center.y() - (this->height() / 2);
    if (position != nullptr)         
        position->Set_Has_Pawn(false);
    position = On;
    position->Set_Has_Pawn(true);
    this->move(x, y);
    Is_Pressed = false;
    
}
Place * Pawns::Get_Position()
{
    return position;
}
void Pawns :: Set_Choosen(bool a )
{
    Is_Pressed = a;
}
void Pawns :: Set_Id(bool x)
{
    Press_Id = x;
}
int Pawns::Get_Id()
{
    return Press_Id;
}
int Pawns::Get_Finish()
{
    return Finish_Row;
}
void Pawns::Set_Finish(int x)
{
    Finish_Row = x;
}
void Pawns :: Set_Position(Place * a)
{
    position = a;
}