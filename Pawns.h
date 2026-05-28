#pragma once
#include "Definitions.h"
#include "Place.h"
class Pawns : public widget{
    Q_OBJECT
    private: 
bool Is_Pressed = false;
label thisis;   
Place * position;
public:
Pawns(widget * parent);
protected:
void mousePressEvent(mevent * event) override;
public:
void movepawn(point  center);
void Set_Position( Place * On);
Place * Get_Position();
signals: 
void pawnClicked(Pawns* clickedPawn);
};