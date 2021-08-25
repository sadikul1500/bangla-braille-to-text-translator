#include "olistview.h"
#include <QDebug>
#include <QEvent>
#include <qevent.h>
OListView::OListView(QWidget *parent)
{
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    setSelectionRectVisible(true);
    setContextMenuPolicy(Qt::ActionsContextMenu);


}

bool OListView::event(QEvent *event){

    if (event->type() == QEvent::KeyPress) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            //qDebug()<<keyEvent->key()<<" "<<keyEvent->modifiers()<<endl;
         if ((keyEvent->key() == 16777235 || keyEvent->key() == 16777237) && keyEvent->modifiers()==0) { //up
             emit(upDownKeyPressed());
         }
         else if (keyEvent->key() == 16777234 && keyEvent->modifiers()==0) { //left
             emit(leftKeyPressed());
         }
         else if (keyEvent->key() == 16777236/*rightArrow*/ && keyEvent->modifiers()==67108864/*Ctrl*/) {
             emit(rightKeyPressed());
         }
         else if (keyEvent->key() == 16777223 && keyEvent->modifiers()==0) {
             emit(deletPressed());
         }
         else if (keyEvent->key() == 83  && keyEvent->modifiers()==67108864) {
             emit(cntrlPlusSPressed());
         }
         else if (keyEvent->key() == 83  && keyEvent->modifiers()==201326592) {
             emit(cntrlPlusAltPlusSPressed());
         }


    }
    return QListView::event(event);

}
