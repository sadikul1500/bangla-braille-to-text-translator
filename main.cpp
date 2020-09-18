#include "dialog.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QFontDatabase>
#include <QFileDialog>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow m;
    m.setWindowState(Qt::WindowMaximized);
    m.show();
    return a.exec();
}
