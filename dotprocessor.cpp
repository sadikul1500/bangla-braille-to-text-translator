#include "dotprocessor.h"
#include "dotBoundaryFinder.h"
#include <QApplication>

DotProcessor::DotProcessor()
{

}

DataBundle DotProcessor::markDotByPoint(QImage image, QPoint point, bool mark)
{ // assuming point is a black dot
    // return: black pixel turns 1,1,1 only for very large dot //not by ref and by ref
    // return image can also be marked //not by ref and by ref
    if(!(image.pixel(point.x(),point.y())==qRgb(0,0,0))){
        qDebug()<<"Error: markDotByPoint (x,y): "<<point.x()<<","<<point.y()<<endl;
        return DataBundle();
    }
    QImage originalImg = image;
    DotBoundaryFinder ulbrObjct(image);
    qApp->processEvents();

    QList<int> list = ulbrObjct.findBoundary(point.x(),point.y());
    qApp->processEvents();
    int x = (list[1]+list[3])/2;
    int y = (list[0]+list[2])/2;

    if((list[3]-list[1]) > 2*_maxDotWidth || (list[2]-list[0]) > 2*_maxDotWidth) mark = false; // because mark intersect others!!
    if((list[3]-list[1]) < _minDotWidth && (list[2]-list[0]) < _minDotWidth) mark = false;
    if(_isDebug && mark) markByBoundaries(originalImg,qRgb(128,128,128),list);
    if(shouldPrint) qDebug()<<"dot size: "<<QPoint((list[3]-list[1]),(list[2]-list[0]))<<"minMax: "<<QPoint(_minDotWidth,_maxDotWidth)<<endl;

    DataBundle dataBundle(originalImg);
               dataBundle.dotCenter = QPoint(x,y);
               if((list[3]-list[1]) < _minDotWidth && (list[2]-list[0]) < _minDotWidth){
                    dataBundle.dotCenter = point;
                    dataBundle.isValidDot = false;
                    dataBundle.shouldInCharIdentification = false;
                   if(shouldPrint) qDebug()<<"very small Dot: "<<QPoint(_minDotWidth,_minDotWidth)<<endl;
               }

              else if(ulbrObjct.getArea()<30 || ((list[3]-list[1]) < (_minDotWidth*1.5) && (list[2]-list[0]) < (_minDotWidth*1.5))){
                   dataBundle.shouldInCharIdentification = false; //in next else you can use area to identify smalldot
                   if(shouldPrint) qDebug()<<"medium dot should in line identification: "<<QPoint(_minDotWidth*1.5,_minDotWidth*1.5)<<endl;
               }

               else if((list[3]-list[1]) > 2*_maxDotWidth || (list[2]-list[0]) > 2*_maxDotWidth){
                    dataBundle.shouldInCharIdentification = false;
                    dataBundle.isVeryLarge = true;
                    dataBundle.image = image; // contains 1,1,1 pixel
                    dataBundle.dotCenter = point;
                    if(shouldPrint) qDebug()<<"very large dot: "<<QPoint(2*_maxDotWidth,2*_maxDotWidth)<<endl;
               }
               else if((list[3]-list[1]) > _maxDotWidth || (list[2]-list[0]) > _maxDotWidth)
               {
                   dataBundle.isLargeDot = true;
                   dataBundle.shouldInCharIdentification = false;
                   if(shouldPrint) qDebug()<<"large dot: "<<QPoint(_maxDotWidth,_maxDotWidth)<<endl;
                   if((list[3]-list[1]) > _maxDotWidth && !((list[2]-list[0]) > _maxDotWidth))
                       dataBundle.dotCenter = QPoint(point.x(),y);
                   else if ((list[2]-list[0]) > _maxDotWidth && !((list[3]-list[1]) > _maxDotWidth))
                       dataBundle.dotCenter = QPoint(x,point.y());
                   else
                       dataBundle.dotCenter = QPoint(point.x(),point.y());
               }
               dataBundle.u = list[0];
               dataBundle.l = list[1];
               dataBundle.b = list[2];
               dataBundle.r = list[3];
               return  dataBundle;
}

DataBundle DotProcessor::searchForBlackDotAndMark(const QImage &image, QPoint point, bool mark)
{
    // return isValidDot = true for the dot grater than mnimum value
    // otherwise return false including not dot found

    int errH = _errToFindDot.x();
    errH = findingChar ? errH/2 : errH;
    int row = image.height();
    int col = image.width();
    DataBundle dataBundle(image);
//------------------------------------
    QList<QPoint> ptList;
    ptList.append(point);

    for(int i=3;i<=errH;i+=2) {
        ptList.append(QPoint(point.x()-i,point.y()));
        ptList.append(QPoint(point.x()-i,point.y()+i));
        ptList.append(QPoint(point.x(),point.y()+i));
        ptList.append(QPoint(point.x()+i,point.y()+i));
        ptList.append(QPoint(point.x()+i,point.y()));
        ptList.append(QPoint(point.x()+i,point.y()-i));
        ptList.append(QPoint(point.x(),point.y()-i));
        ptList.append(QPoint(point.x()-i,point.y()-i));
    }

    foreach(QPoint pt,ptList){
        if(!((pt.x()>=0 && pt.x()<col) && (pt.y()>=0 && pt.y()<row))) continue;

        if(dataBundle.image.pixel(pt.x(),pt.y())==qRgb(0,0,0))
        {
//            return markDotByPoint(dataBundle.image,QPoint(pt.x(),pt.y()),mark);
            dataBundle =  markDotByPoint(dataBundle.image,QPoint(pt.x(),pt.y()),mark);
            if(!dataBundle.isValidDot) continue;
            else return dataBundle;
             // return valid dot with other state(is large,is very large)
        }
        else if(_isDebug && mark) dataBundle.image.setPixel(pt.x(),pt.y(),qRgb(128,128,128));
    }

    dataBundle.isValidDot = false; // invalid dot for lesser size than minimum requirement including no dot found
    dataBundle.dotCenter = point;
    if(shouldPrint) qDebug()<<"no dot found"<<endl;
    return  dataBundle;
}
void DotProcessor::markByBoundaries(QImage &img, QRgb rgb, QList<int> &ULBR)
{
    int u = ULBR[0],l = ULBR[1],b= ULBR[2],r = ULBR[3];
    int row = img.height();
    int col = img.width();
    u>1 ? u-=2:u=1;
    l>1 ? l-=2:l=1;
    b<row-2 ? b+=2:b=row-1;
    r<col-2 ? r+=2:r = col-1;
    for(int i=l;i<=r;i++)
    {
        if(img.pixel(i,u)!=qRgb(0,0,0))
                img.setPixel(i,u,rgb);
        if(img.pixel(i,b)!=qRgb(0,0,0))
            img.setPixel(i,b,rgb);
    }

    for(int i=u;i<=b;i++)
    {
        if(img.pixel(l,i)!=qRgb(0,0,0))
            img.setPixel(l,i,rgb);
        if(img.pixel(r,i)!=qRgb(0,0,0))
            img.setPixel(r,i,rgb);
    }

}
