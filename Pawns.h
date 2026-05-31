#pragma once
#include "Definitions.h"
#include "Place.h"
class Pawns : public widget{
Q_OBJECT
private: 
bool Press_Id = false;  
bool Is_Pressed = false; 
Place* position = nullptr;
int Finish_Row = 0;
void mousePressEvent(mevent * event) override;
bool Pawn_Id = false;
widget * Parent = nullptr;
public:
Pawns(widget * parent);
void movepawn(Place * On , bool undo);
Place * Get_Position();
void Set_Position(Place * a);
void Set_Choosen(bool a );
static inline bool Turn = false;
void Set_Id(bool x);
int Get_Id();
int Get_Finish();
void Set_Finish(int x);
signals: 
void pawnClicked(Pawns* clickedPawn);
};