#include "ulbrofdotv3.h"
#include <QDebug>
ULBRofDotV3::ULBRofDotV3(QImage image)
{
    this->_image = image;
    row = image.height();
    col = image.width();
}

QList<int> ULBRofDotV3::findULBR(int x, int y)
{
    QImage image;
    image = _image;
    for(int i=x;i<col;i++){
        //qDebug()<<"ok"<<row<<" "<<col<<endl;
        if(image.pixel(i,y) != qRgb(0,0,0)){
            if(!traversePath(_image,i-1,y)){
                image = _image;
                i = index - 1;
                continue;
            }

             else
                break;
        }
    }
    //qDebug()<<"ok";
    QList<int> list;
    list<<u<<l<<b<<r;
    return list;
}

bool ULBRofDotV3::traversePath(QImage &image,int x, int y)
{
    u = b = y;
    l = r = x;
    image.setPixel(x,y,qRgb(1,1,1));
    addCirList(image,x,y);
    QPoint point;
    while(!cirPointList.empty()){
        point = cirPointList.takeFirst();
        image.setPixel(point.x(),point.y(),qRgb(1,1,1));
        u = std::min(point.y(),u);
        l = std::min(point.x(),l);
        b = std::max(point.y(),b);
        r = std::max(point.x(),r);
        if(point.x() > x && point.y()==y){
            index = point.x();
            return  false;
        }
        addCirList(image,point.x(),point.y());
    }
    return  true;
}

void ULBRofDotV3::addCirList(QImage &image, int x, int y)
{
    QList<QPoint> adjPt;
    adjPt<<QPoint(x-1,y-1);
    adjPt<<QPoint(x,y-1);
    adjPt<<QPoint(x+1,y-1);
    adjPt<<QPoint(x+1,y);
    adjPt<<QPoint(x+1,y+1);
    adjPt<<QPoint(x,y+1);
    adjPt<<QPoint(x-1,y+1);
    adjPt<<QPoint(x-1,y);
    QRgb preCol = image.pixel(adjPt[0].x(),adjPt[0].y());
    for(int i=1;i<adjPt.length();i++){
        QRgb cellColor = image.pixel(adjPt[i].x(),adjPt[i].y());
        if(cellColor != preCol){
            preCol = cellColor;
            if(cellColor == qRgb(0,0,0))
                cirPointList.append(adjPt[i]);
            else
                cirPointList.append(adjPt[i-1]);
        }

    }
}
