#pragma once
#include "Definitions.h"
class Place : public widget {
    Q_OBJECT
private:
    bool Has_Pawn =false;
    point center_point = this->geometry().center(); 
    int row;
    int col;
    bool Available = false;
    void mousePressEvent(mevent * event) override;
    bool Marked = false;
public:
    Place(Place * a);
    Place(int r = 0, int c = 0, widget *parent = nullptr);
    bool hasPawn() const;
    point  getCenter() const;
    int getRow() const;
    int getCol() const;
    void Change_Color(bool a);
    void Set_Available(bool b);
    bool Get_Available();
    void Set_Has_Pawn(bool x);
    bool Get_Has_Pawn();
    bool Get_Mark();
    void Set_Mark(bool);
signals:
void Move_Pawn(Place * a , bool x);
void Clean();    
};