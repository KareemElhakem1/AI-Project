#include "CMW.h"
void Choose_Mode_Style(widget * home);
Choose_Mode_Window :: Choose_Mode_Window()
{
    this->setFixedSize(700, 500);
    this->setWindowTitle("Choose Game Mode");
    this->setWindowIcon(QIcon(":/Images/Icon.png"));
    this->setObjectName("MainMenu");
    Choose_Mode_Style(this);
    Mode_Buttons->addWidget(Friend_Mode);
    Mode_Buttons->addWidget(AI_Mode);
    Mode_Buttons->setAlignment(AlignCenter);
    this->setLayout(Mode_Buttons);
}
void Choose_Mode_Style(widget * home)
{
home->setStyleSheet("QWidget#MainMenu { "
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
}