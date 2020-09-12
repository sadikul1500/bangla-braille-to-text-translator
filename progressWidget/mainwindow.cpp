#include "mainwindow.h"
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QPushButton *button1 = new QPushButton("button1");

    setCentralWidget(button1);
    connect(button1,&QPushButton::clicked,[=](){

        prog.show();
        prog.activateWindow();
        prog.setMaximum(3,4);
    }) ;
}
