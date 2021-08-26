#ifndef CHARREADER_H
#define CHARREADER_H

#include "databundle.h"
#include "scalevaraibles.h"
#include "dotprocessor.h"

class CharReader : public ScaleVaraibles
{
public:
    CharReader();
    DataBundle getBrailleChPosCenter(const QImage &image, QPoint center);
    DataBundle getBrailleChPosLeft(QImage image, QPoint point);

private:
    DotProcessor dotProcessor;
};

#endif // CHARREADER_H
