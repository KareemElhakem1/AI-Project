#include "Controller.h"
void Controller :: start()
{
 Win = new Choose_Mode_Window();
 Game = new Game_Window();
 Win->show();
 Switch_Window(Win->get_button() , Win , Game );
} 