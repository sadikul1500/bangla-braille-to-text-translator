#ifndef STARTLABEL_H
#define STARTLABEL_H

#include <QWidget>
#include <QLabel>
class StartLabel : public QLabel
{
    Q_OBJECT
public:
    explicit StartLabel(QWidget *parent = nullptr);
protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;
signals:
    void clicked();

};

#endif // STARTLABEL_H
