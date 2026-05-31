#include "Fences.h"

Fences::Fences(int r, int c, bool horizontal, QWidget* parent) 
    : QLabel(parent), row(r), col(c), isHorizontal(horizontal) 
{
    if (isHorizontal) this->setFixedSize(90, 10); 
    else this->setFixedSize(10, 90);              
    
    this->setStyleSheet("background-color: transparent; border: 1px dotted gray;");
}

void Fences::mousePressEvent(QMouseEvent* event) {
    
    
    emit fenceClicked(this); 
}

void Fences::placeVisually() {
    this->setStyleSheet("background-color: #8B4513;"); 
     this->raise(); 
}
int Fences::getRow() const {
    return row;
}

int Fences::getCol() const {
    return col;
}

bool Fences::getIsHorizontal() const {
    return isHorizontal;
}
void Fences::resetVisually()
{
    this->setStyleSheet("background-color: transparent;"); 
}