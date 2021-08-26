#ifndef DOTPROCESSOR_H
#define DOTPROCESSOR_H

#include "databundle.h"
#include "scalevaraibles.h"

#include <QRgb>
#include <qimage.h>

class DotProcessor : public ScaleVaraibles
{
public:
    DotProcessor();
    DataBundle markDotByPoint(QImage image, QPoint point,bool mark);
    DataBundle searchForBlackDotAndMark(const QImage &image,QPoint point,bool mark);
    void markByBoundaries(QImage &img,QRgb rgb,QList<int> &ULBR);
};

#endif // DOTPROCESSOR_H
