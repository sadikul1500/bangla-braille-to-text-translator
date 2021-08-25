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
    static DataBundle markDotByPoint(QImage image,QPoint point,bool mark=true);
    static DataBundle searchForBlackDotAndMark(const QImage &image,QPoint point,bool mark=true);
    static DataBundle getBrailleChPosLeft(QImage image, QPoint point);
    static DataBundle getBrailleChPosRight(QImage image, QPoint point);
    static DataBundle getBrailleChPosCenter(const QImage &image, QPoint center);
    static DataBundle findCenter(const QImage &image,QPoint cntrBlckDot);
    static bool hasEnoughDot(QImage &image,int row);
    static DataBundle extractChar(QImage &image, QPoint blackPix);
    static DataBundle findChar(QImage image, QPoint startPt);
    static DataBundle readLine(QImage image, QPoint cnterCh);
    static DataBundle readLineLeftPart(QImage image, QPoint cnterCh);
    static DataBundle readLineRightPart(QImage image, QPoint cnterCh);
    static QImage mainImageToBinImage(QString imageFile);
    static void setBrailleProperty(QPoint distBetDot, QPoint distBetCh, int minDotWidth, int maxDotWidth, QPoint errToFindDot, QPoint errToFindCh, bool ShouldPrint=false,bool isDebug=false);

public:
    static QPoint _distBetDot,_distBetCh,_errToFindDot,_errToFindCh;
    static int _minDotWidth,_maxDotWidth;
    static bool findingChar,shouldPrint,_isDebug;
};

#endif // UTILIMAGEPROC_H
