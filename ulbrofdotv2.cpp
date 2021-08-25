#include "ulbrofdotv2.h"
#include <QDebug>

ulbrofdotv2::ulbrofdotv2(QImage image)
{
    this->image = image;
    row = image.height();
    col = image.width();
}

QList<int> ulbrofdotv2::findULBR(int x, int y) // it is better to call x,y which is black and valid
{
    int u,l,b,r;
        u = b = y;
        l = r = x;
    if(!(x>=0 && x<col && y>=0 && y<row))
    {
        qDebug()<<"yes"<<endl;
        return QList<int>();
    }


    //qDebug()<<u<<" "<<l<<" "<<b<<" "<<r<<endl;
    qDebug()<<cnt++<<endl;
    //qDebug()<<"x "<<x<<"y "<<y<<endl;

    image.setPixel(x,y,qRgb(1,1,1));
    QList<int> ulbrList;
    if(x>0 && x<col && image.pixel(x-1,y) == qRgb(0,0,0)){
        ulbrList = findULBR(x-1,y);
        u = std::min(ulbrList[0],u);
        l = std::min(ulbrList[1],l);
        b = std::max(ulbrList[2],b);
        r = std::max(ulbrList[3],r);
    }
    if(x<col-1 && x>=0 && image.pixel(x+1,y) == qRgb(0,0,0)){
        ulbrList = findULBR(x+1,y);
        u = std::min(ulbrList[0],u);
        l = std::min(ulbrList[1],l);
        b = std::max(ulbrList[2],b);
        r = std::max(ulbrList[3],r);
    }
    if(y>0 && y<row && image.pixel(x,y-1) == qRgb(0,0,0)){
        ulbrList = findULBR(x,y-1);
        u = std::min(ulbrList[0],u);
        l = std::min(ulbrList[1],l);
        b = std::max(ulbrList[2],b);
        r = std::max(ulbrList[3],r);
    }
    if(y<row-1 && y>=0 && image.pixel(x,y+1) == qRgb(0,0,0)){
        ulbrList = findULBR(x,y+1);
        u = std::min(ulbrList[0],u);
        l = std::min(ulbrList[1],l);
        b = std::max(ulbrList[2],b);
        r = std::max(ulbrList[3],r);
    }

    ulbrList[0] = u;
    ulbrList[1] = l;
    ulbrList[2] = b;
    ulbrList[3] = r;

    return  ulbrList;

}

void ulbrofdotv2::rec(int n)
{
    for(int i=n;i>0;i--)
    qDebug()<<i<<endl;
}
