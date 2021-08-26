#include "scalevaraibles.h"


bool ScaleVaraibles::_isDebug = false;
ScaleVaraibles::ScaleVaraibles()
{
    _minDotWidth = 5;
    _maxDotWidth = 27;
    _errToFindDot = QPoint(10,10);

    _errToFindCh = QPoint(1,1);
    _distBetDot = QPoint(28,32);
    _distBetCh = QPoint(76,126);
    //qDebug()<<m_minDotWidth<<" "<<m_maxDotWidth<<" "<<m_errDot<<" "<<m_errCh<<" "<<m_distBetDot<<" "<<m_distBetCh;

    findingChar=false;
    shouldPrint=false;
}

