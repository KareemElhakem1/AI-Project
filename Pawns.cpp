#include "Pawns.h"
Pawns :: Pawns(widget * parent) : QWidget(parent)
{
    this->raise();
}
void Pawns :: mousePressEvent(mevent * event)
{
    if (event->button() == LeftButton) {
        Is_Pressed = true;     
        this->raise();
        emit pawnClicked(this);
    }
}

void Pawns::movepawn(point center)
{
    int x = center.x() - (this->width() / 2);
    int y = center.y() - (this->height() / 2);
    
    this->move(x, y);
}
void Pawns::Set_Position( Place * On)
{
    this->movepawn(On->getCenter());
    position = On;
}
Place * Pawns::Get_Position()
{
    return position;
}