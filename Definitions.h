#pragma once
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
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
#include <QMouseEvent>
#include <QPoint>
#define widget QWidget
#define label QLabel
#define Button QPushButton
#define field QLineEdit
#define hbox QHBoxLayout
#define vbox QVBoxLayout
#define rbutton QRadioButton
#define mevent QMouseEvent
#define point QPoint
#define Grid QGridLayout 
using namespace Qt;
inline void Switch_Window(Button* a , widget* first, widget* second){
    QObject::connect(a, &Button::clicked, [=]() {
    if (first != nullptr && second != nullptr) {
        first->hide();
        second->show();
    }
}); 
}