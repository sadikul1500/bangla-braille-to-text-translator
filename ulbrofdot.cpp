#include "ulbrofdot.h"
//using namespace  std;
#include <QDebug>
ULBROfDot::ULBROfDot(QImage image)
{
    img = image;
}
QList<int> ULBROfDot::findULBR(int y, int x) //assume (x,y) is like matlab
{
    int row = img.height();
    int col = img.width();
    int u,l,b,r;
    if(x<0 || y<0 || x>=row || y>=col)
    {
        QList<int> list;
        list<<0<<0<<0<<0;
        return list;
    }

    img.setPixel(y,x,qRgb(1,1,1));
//%-------------------upper------------------
    for(u=x;u>=1;u--)
        if(img.pixel(y,u-1)!=qRgb(0,0,0))
            break;
//%-------------------left------------------
    for(l=y;l>=1;l--)
        if(img.pixel(l-1,x)!=qRgb(0,0,0))
            break;
//%-------------------below------------------
    for(b=x;b<row-1;b++)
        if(img.pixel(y,b+1)!=qRgb(0,0,0))
            break;
//%-------------------right------------------
    for(r=y;r<col-1;r++)
        if(img.pixel(r+1,x)!=qRgb(0,0,0))
            break;

qDebug()<<"end ulbr measure"<<"(x,y)"<<x<<y<<endl;
    QList<int> ulbrList;
    if(y!=0 && img.pixel(y-1,x)==qRgb(0,0,0))
    {
        qDebug()<<"inside 1"<<"(x,y)"<<x<<y<<endl;
            ulbrList = findULBR(y-1,x);
            qDebug()<<"inside 11"<<"(x,y)"<<x<<y<<endl;
            u = std::min(ulbrList[0],u);
            l = std::min(ulbrList[1],l);
            b = std::max(ulbrList[2],b);
            r = std::max(ulbrList[3],r);
    }

    if(y!=col-1 && img.pixel(y+1,x)==qRgb(0,0,0))
    {
        qDebug()<<"working1"<<endl;
        ulbrList = findULBR(y+1,x);
        qDebug()<<"working2"<<endl;
        u = std::min(ulbrList[0],u);
        l = std::min(ulbrList[1],l);
        b = std::max(ulbrList[2],b);
        r = std::max(ulbrList[3],r);
    }
    if(x!=row-1 && img.pixel(y,x+1)==qRgb(0,0,0))
    {
        ulbrList = findULBR(y,x+1);
        u = std::min(ulbrList[0],u);
        l = std::min(ulbrList[1],l);
        b = std::max(ulbrList[2],b);
        r = std::max(ulbrList[3],r);
    }
    if(x!=0 && img.pixel(y,x-1)==qRgb(0,0,0))
    {
        ulbrList = findULBR(y,x-1);
        u = std::min(ulbrList[0],u);
        l = std::min(ulbrList[1],l);
        b = std::max(ulbrList[2],b);
        r = std::max(ulbrList[3],r);
    }
    QList<int> list;
    list<<u<<l<<b<<r;
    return list;

}
