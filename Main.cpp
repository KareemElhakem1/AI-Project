#include <QApplication>
#include <QCoreApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>   
#include <QPushButton> 
#include <QRadioButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFrame>
#include <QMessageBox>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
using namespace Qt;

//To make the Qt objects easier to use for me
#define widget QWidget
#define label QLabel
#define button QPushButton
#define field QLineEdit
#define hbox QHBoxLayout
#define vbox QVBoxLayout
#define rbutton QRadioButton
void Switch_Window(button* a , widget* first, widget*second);
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    widget* Choose_Mode = new widget();
    widget*Friend_Game = new widget();
    widget* AI_Game = new widget();
    vbox* Mode_Buttons = new vbox();
    button* Friend_Mode = new button("Play with a friend");
    button* AI_Mode = new button("AI is your friend");
    Choose_Mode->setFixedSize(700, 500);
    Choose_Mode->setWindowTitle("Choose Game Mode");
    Choose_Mode->setWindowIcon(QIcon(":/Images/Icon.png"));
    Choose_Mode->setObjectName("MainMenu");
    Choose_Mode->setStyleSheet("QWidget#MainMenu { "
                "background-image: url(':/Images/Background.png'); "
                "background-position: center; "
                "} "
                "QPushButton { "
                "min-width: 300px; "
                "min-height: 60px; "
                "font-size: 24px; "
                "font-weight: bold; "
                "border-radius: 15px; "
                "background-color: rgba(230, 171, 11, 0.85); "
                "color: #11111b; "
                "} "
                "QPushButton:hover { "
                "background-color: rgba(255, 255, 255, 1.0); "
                "}");
    Friend_Mode->resize(00 , 400);
    Mode_Buttons->addWidget(Friend_Mode);
    Mode_Buttons->addWidget(AI_Mode);
    Mode_Buttons->setAlignment(AlignCenter);
    Switch_Window(Friend_Mode , Choose_Mode , Friend_Game);
    Switch_Window(AI_Mode , Choose_Mode , AI_Game);
    Choose_Mode->setLayout(Mode_Buttons);
    Choose_Mode->show();
    return app.exec();
}
void Switch_Window(button* a , widget* first, widget* second) {
    QObject::connect(a, &button::clicked, [=]() {
    if (first != nullptr && second != nullptr) {
        first->hide();
        second->show();
    }
}); 
}