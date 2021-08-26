#ifndef SCALEVARAIBLES_H
#define SCALEVARAIBLES_H

#include <QPoint>
#include <QDebug>
class ScaleVaraibles
{
public:
    ScaleVaraibles();
    static bool _isDebug;
    QPoint _distBetCh,_distBetDot;
protected:
    QPoint _errToFindDot,_errToFindCh;
    int _minDotWidth,_maxDotWidth;
    bool findingChar,shouldPrint;

};

#endif // SCALEVARAIBLES_H
