#ifndef ULBROFDOTV4_H
#define ULBROFDOTV4_H

#include <QImage>



class DotBoundaryFinder
{
public:
    DotBoundaryFinder(QImage &image);
    QList<int> findBoundary(int x, int y);
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
