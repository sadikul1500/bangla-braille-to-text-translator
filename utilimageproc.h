#ifndef UTILIMAGEPROC_H
#define UTILIMAGEPROC_H

#include "databundle.h"

#include <QObject>
#include <QImage>
class UtilImageProc
{

public:
    UtilImageProc();

public :
    static QImage drawHLine(QImage img,QRgb rgb,QPoint center1,QPoint center2);
    static QImage drawVLine(QImage img,QRgb rgb,QPoint center1,QPoint center2);
    static void markByULBR(QImage &img,QRgb rgb,QList<int> &ULBR); //markByULBR
    static DataBundle markDotByPoint(QImage image,QPoint point);
    static DataBundle searchForBlackDotAndMark(QImage &image,QPoint point);
    static DataBundle getBrailleChPosLeft(QImage image, QPoint point);
    static DataBundle getBrailleChPosRight(QImage image, QPoint point);
    static DataBundle getBrailleChPosCenter(QImage &image, QPoint center);
    static DataBundle findCenter(QImage &image,QPoint cntrBlckDot);
    static DataBundle findChar(QImage image, QPoint startPt);
    static DataBundle readLine(QImage image, QPoint cnterCh);
    static DataBundle readLineLeftPart(QImage image, QPoint cnterCh);
    static DataBundle readLineRightPart(QImage image, QPoint cnterCh);
    static QImage mainImageToBinImage(QString imageFile);
    static void setBrailleProperty(QPoint distBetDot,QPoint distBetCh,int minDotWidth,int maxDotWidth,QPoint errToFindDot,QPoint errToFindCh);

public:
    static QPoint _distBetDot,_distBetCh,_errToFindDot,_errToFindCh;
    static int _minDotWidth,_maxDotWidth;
};

#endif // UTILIMAGEPROC_H
