#include "startlabel.h"
#include <QDebug>
StartLabel::StartLabel(QWidget *parent) : QLabel(parent)
{

}

void StartLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    emit clicked();
}
