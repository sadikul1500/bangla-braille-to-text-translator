#include "ulbrofdotv4.h"
#include <QDebug>
#include <bits/stdc++.h>
using namespace std;
ULBRofDotV4::ULBRofDotV4(QImage &image)
{
    this->image =&image;
    row = image.height();
    col = image.width();
    stackList = new QList<QPoint>();
}

QList<int> ULBRofDotV4::findULBR(int x, int y)
{
    if(!isValidAndBlack(x,y)){
        qDebug()<<"Err from ULBRofDotV4 (x,y): "<<x<<","<<y<<endl;
        return QList<int>()<<0<<0<<0<<0;
    }
    area = 1;
    u = b = y;
    l = r = x;

    image->setPixel(x,y,qRgb(1,1,1));
    stackList->append(QPoint(x,y));
    QPoint point;
    while(!stackList->empty()){
        area++;
        point = stackList->takeFirst();
        u = std::min(point.y(),u);
        l = std::min(point.x(),l);
        b = std::max(point.y(),b);
        r = std::max(point.x(),r);
        fillUpAdjList(point.x(),point.y());
    }
    //cout<<area<<endl;
    return  QList<int>()<<u<<l<<b<<r;
}

bool ULBRofDotV4::isValidAndBlack(int x, int y)
{
    if(x>=0 && x<col)
        if(y>=0 && y<row)
            if(image->pixel(x,y) == qRgb(0,0,0))
                return  true;
    return  false;
}

void ULBRofDotV4::fillUpAdjList(int x, int y)
{
    QList<QPoint> tempList;
    tempList.append(QPoint(x-1,y));
    tempList.append(QPoint(x+1,y));
    tempList.append(QPoint(x,y-1));
    tempList.append(QPoint(x,y+1));

    for(int i=0;i<=3;i++)
        if(isValidAndBlack(tempList[i].x(),tempList[i].y())){

            image->setPixel(tempList[i].x(),tempList[i].y(),qRgb(1,1,1));
            stackList->append(tempList[i]);
        }

}

double ULBRofDotV4::getArea()
{
    return area;
}
