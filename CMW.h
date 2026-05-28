#pragma once
#include "Pawns.h"
#include "GW.h"
class Choose_Mode_Window : public widget{
    private:
    vbox*   Mode_Buttons = new vbox();
    Button* Friend_Mode = new Button("Play with a friend");
    Button* AI_Mode = new Button("AI is your friend");
    public:
    Choose_Mode_Window();
    Button * get_button()
    {
        return this->Friend_Mode;
    }
};