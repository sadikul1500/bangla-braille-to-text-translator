#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>


class ImageLabel : public QLabel
{
    Q_OBJECT
signals:
    void center1();
    void center2();
    void imagePixel();

public slots:
    void scaleImage(double factor);
public:
    ImageLabel();
    double getScaleFactor();
    void mouseReleaseEvent(QMouseEvent *ev);// override;
    void setScaleFactor(double factor);


public:
    QPoint point;
private:
    double scaleFactor = 1;

};

#endif // IMAGELABEL_H
