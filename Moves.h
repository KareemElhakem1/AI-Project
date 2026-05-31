#pragma once
class Pawns;
class Place;
class Fences;

class Move {
public:
    bool isFence;

    Pawns* movedPawn;
    Place* oldPlace;
    Place* newPlace;

    Fences* placedFence;
    bool playerTurn; 

    Move(Pawns* p, Place* oldP, Place* newP) {
        isFence = false;
        movedPawn = p;
        oldPlace = oldP;
        newPlace = newP;
        placedFence = nullptr;
        playerTurn = false;
    }
    Move(Fences* f, bool player) {
        isFence = true;
        placedFence = f;
        playerTurn = player;
        movedPawn = nullptr;
        oldPlace = nullptr;
    }
};