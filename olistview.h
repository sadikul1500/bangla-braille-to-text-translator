#ifndef OLISTVIEW_H
#define OLISTVIEW_H

#include <qlistview.h>


class OListView : public QListView
{
    Q_OBJECT
public:
    OListView();
    QModelIndexList selectedIndexes() const override{return QAbstractItemView::selectedIndexes();}
protected:
    bool event(QEvent *e) override;
public: signals:
    void upDownKeyPressed();
    void leftKeyPressed();
    void rightKeyPressed();
    void deletPressed();
    void cntrlPlusSPressed();
    void cntrlPlusAltPlusSPressed();
};


#endif // OLISTVIEW_H
