#ifndef DATABUNDLE_H
#define DATABUNDLE_H

#include <QImage>



class DataBundle
{
public:
    DataBundle(QImage img);
    DataBundle();
    bool isValidCh();
    bool isAllZero(int startIdx,int endIdx);
    QImage image;
    QString binChar;
    QPoint charCenter,dotCenter;
    bool isValidDot,isLargeDot,isSmallDot; // false means it is a black dot but may be noise bcz width and height is too small
    int u=0,l=0,b=0,r=0;
    int numberOfDotInCh;
    QList<QString> charList;

};

#endif // DATABUNDLE_H
