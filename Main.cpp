#include "Pawns.h"
#include "Controller.h"
void Switch_Window(Button* a , widget* first, widget*second);
void Choose_Mode_Style(widget * home);
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Controller * Game_Controller = new Controller();
    Game_Controller->start();
    return app.exec();
}
