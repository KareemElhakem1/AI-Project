#pragma once
#include "Definitions.h"
#include "Place.h"
class Pawns : public widget{
Q_OBJECT
private: 
bool Press_Id = false;  
bool Is_Pressed = false; 
Place * position;
void mousePressEvent(mevent * event) override;
bool Pawn_Id = false;
public:
Pawns(widget * parent);
void movepawn(Place * On);
Place * Get_Position();
void Set_Choosen(bool a );
static inline bool Turn;
void Set_Id(bool x);
int Get_Id();
signals: 
void pawnClicked(Pawns* clickedPawn);
};