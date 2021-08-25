#ifndef ULBROFDOT_H
#define ULBROFDOT_H

#include <QImage>
#include <QList>


class ULBROfDot
{
public:
    ULBROfDot(QImage image);
    QList<int> findULBR(int x, int y);

public:
    QImage img;
    long cnt=0;

};

#endif // ULBROFDOT_H
