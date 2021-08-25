#ifndef ULBROFDOTV2_H
#define ULBROFDOTV2_H

#include <QImage>



class ulbrofdotv2
{
public:
    ulbrofdotv2(QImage image);
    QList<int> findULBR(int x, int y);
    void rec(int n);

private:
    QImage image;
    int row,col;
    long cnt = 0;
};

#endif // ULBROFDOTV2_H
