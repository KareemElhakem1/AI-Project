#pragma once
#include "Definitions.h"
class Place : public widget {
private:
    bool has_pawn;
    QPoint center_point = this->geometry().center(); 
    int row;
    int col;
    bool Available = false;
public:
    Place(int r = 0, int c = 0, widget *parent = nullptr);
    bool hasPawn() const;
    void setPawn(bool status);
    point  getCenter() const;
    int getRow() const;
    int getCol() const;
    void Change_Color();
    void Set_Available();
    
};