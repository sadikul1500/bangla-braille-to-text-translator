#include "imagelabel.h"
#include "ulbrofdot.h"
#include "utilimageproc.h"

#include <QFontDatabase>
#include <QMouseEvent>



ImageLabel::ImageLabel(QWidget *parent)
{
    QFontDatabase::addApplicationFont(":/Fonts/Font/kalpurush.ttf");
    QFont font = QFont("Kalpurush", 20, 10);
    setFont(font);
    //imageLabel->setWordWrap(true);
    setWordWrap(true);
    resize(0,0);

}

double ImageLabel::getScaleFactor()
{
    return scaleFactor;
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    int x = ev->x()/scaleFactor;
    int y = ev->y()/scaleFactor;
    if(ev->modifiers()==Qt::ControlModifier)
    {
        point = QPoint(x,y);
        emit center1();
    }
    else if(ev->modifiers()==Qt::AltModifier)
    {
        point = QPoint(x,y);
        emit center2();
    }
    else
    {
        point = QPoint(x,y);
        emit imagePixel();
    }
}

void ImageLabel::setScaleFactor(double factor)
{
    scaleFactor = factor;
}


void ImageLabel::scaleImage(double factor)
{
    scaleFactor *= factor;
    resize(scaleFactor * pixmap()->size());
}
