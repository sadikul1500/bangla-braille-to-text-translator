#ifndef UTILIMAGEPROCFORDIALOG_H
#define UTILIMAGEPROCFORDIALOG_H
#include "databundle.h"

#include <QObject>
#include <QImage>

class utilImageProcForDiaLog
{
public:
    utilImageProcForDiaLog();

public :
    static QImage drawHLine(QImage img,QRgb rgb,QPoint center1,QPoint center2);
    static QImage drawVLine(QImage img,QRgb rgb,QPoint center1,QPoint center2);
    static QImage markByULBR(QImage img,QRgb rgb,QList<int> ULBR); //markByULBR
    static DataBundle markDotByPoint(QImage image,QPoint point,int minWidth,int maxWidth);
    static DataBundle searchForBlackDotAndMark(QImage image,QPoint point,int minWidth,int maxWidth,QPoint err);
    static DataBundle getBrailleChPosLeft(QImage image, QPoint point,int HdotDist,int VdotDist,int minWidth,int maxWidth,QPoint err);
    static DataBundle getBrailleChPosCenter(QImage image, QPoint center,QPoint distBetDot,int minWidth,int maxWidth,QPoint errToFindDot);
    static DataBundle findCenter(QImage image,QPoint cntrBlckDot,QPoint distBetDot,int minWidth, int maxWidth,QPoint errToFindDot);
    static DataBundle findChar(QImage image, QPoint startPt,QPoint distBetDot,QPoint distBetCh, int minWidth, int maxWidth,QPoint errToFindDot);

    static QImage mainImageToBinImage(QString imageFile);

};

#endif // UTILIMAGEPROCFORDIALOG_H
