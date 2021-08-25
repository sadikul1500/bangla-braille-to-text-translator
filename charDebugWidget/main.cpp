#include "chardebug.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CharDebug w;
    w.show();
    return a.exec();
}
