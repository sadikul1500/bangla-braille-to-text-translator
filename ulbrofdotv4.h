#ifndef ULBROFDOTV4_H
#define ULBROFDOTV4_H

#include <QImage>



class ULBRofDotV4
{
public:
    ULBRofDotV4(QImage &image);
    QList<int> findULBR(int x, int y);
    double getArea();
private:
    bool isValidAndBlack(int x,int y);
    void fillUpAdjList(int x,int y);
private:
    QImage *image;
    QList<QPoint> *stackList;
    double area = 0;
    int row;
    int col;
    int u,l,b,r;
};

#endif // ULBROFDOTV4_H
