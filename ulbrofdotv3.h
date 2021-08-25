#ifndef ULBROFDOTV3_H
#define ULBROFDOTV3_H

#include <QImage>



class ULBRofDotV3
{
public:
    ULBRofDotV3(QImage image);
    QList<int> findULBR(int x, int y);
    bool traversePath(QImage &image,int x,int y);
    void addCirList(QImage &image,int x,int y);
private:
    QImage _image;
    int row,col;
    int index;
    int u,l,b,r;
    QList<QPoint> cirPointList;

};

#endif // ULBROFDOTV3_H
