#include "utilimageprocfordialog.h"
#include "dotBoundaryFinder.h"
#include <QApplication>

#include <QDebug>

#include<iostream>
#include<string>
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core/ocl.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/photo.hpp>

using namespace  std;
using namespace cv;
utilImageProcForDiaLog::utilImageProcForDiaLog()
{

}

QImage utilImageProcForDiaLog::drawHLine(QImage img, QRgb rgb, QPoint center1, QPoint center2)
{
    int from = std::min(center1.x(),center2.x());
    int to = std::max(center1.x(),center2.x());
    int height =(center1.y()+center2.y())/2;
    for(int i=from;i<=to;i++)
        img.setPixel(i,height,rgb);

    return  img;
}

QImage utilImageProcForDiaLog::drawVLine(QImage img, QRgb rgb, QPoint center1, QPoint center2)
{
    int from = std::min(center1.y(),center2.y());
    int to = std::max(center1.y(),center2.y());
    int width =(center1.x()+center2.x())/2;
    for(int i=from;i<=to;i++)
        img.setPixel(width,i,rgb);

    return  img;
}

QImage utilImageProcForDiaLog::markByULBR(QImage img, QRgb rgb, QList<int> ULBR)
{
    int u = ULBR[0],l = ULBR[1],b= ULBR[2],r = ULBR[3];
    int row = img.height();
    int col = img.width();
    if(u>1) u-=2;
    if(l>1) l-=2;
    if(b<row-2) b+=2;
    if(r<col-2) r+=2;
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


    return img;
}

DataBundle utilImageProcForDiaLog::markDotByPoint(QImage image, QPoint point, int minWidth, int maxWidth)
{ // assuming point is a black dot
    DotBoundaryFinder ulbrObjct(image);
    qApp->processEvents();
    QList<int> list = ulbrObjct.findBoundary(point.x(),point.y());
    //qDebug()<<"area: "<<ulbrObjct.getArea()<<endl;
    qApp->processEvents();
    int x = (list[1]+list[3])/2;
    int y = (list[0]+list[2])/2;
    image = markByULBR(image,qRgb(128,128,128),list);

    // ----preparing dataBundle for return-----

    DataBundle dataBundle(image);
               dataBundle.dotCenter = QPoint(x,y);
               if((list[3]-list[1]) < minWidth || (list[2]-list[0]) < minWidth)
                   dataBundle.isValidDot = false;
               if((list[3]-list[1]) > maxWidth || (list[2]-list[0]) > maxWidth)
               {
                   dataBundle.isLargeDot = true;
                   if((list[3]-list[1]) > maxWidth && !((list[2]-list[0]) > maxWidth))
                       dataBundle.dotCenter = QPoint(point.x(),y);
                   else if ((list[2]-list[0]) > maxWidth && !((list[3]-list[1]) > maxWidth))
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

DataBundle utilImageProcForDiaLog::searchForBlackDotAndMark(QImage image, QPoint point, int minWidth, int maxWidth, QPoint err)
{
    int errH = err.x(),errV = err.y();
    int row = image.height();
    int col = image.width();
    DataBundle dataBundle(image);

    int from = point.x();
    int to = point.x() + errH;
    for(int i=from;i<=to;i++)
    {
        if(!((i>=0 && i<col) && (point.y()>=0 && point.y()<row)))
            continue;
        if(image.pixel(i,point.y())==qRgb(0,0,0))
        {
            dataBundle = markDotByPoint(image,QPoint(i,point.y()),minWidth,maxWidth);
            if(dataBundle.isValidDot) return dataBundle;
            i = dataBundle.r;
        }
        else image.setPixel(i,point.y(),qRgb(128,128,128));
    }
    to = point.x() - errH;
    for(int i=from;i>=to;i--)
    {
        if(!((i>=0 && i<col) && (point.y()>=0 && point.y()<row)))
            continue;
        if(image.pixel(i,point.y())==qRgb(0,0,0))
        {
            dataBundle = markDotByPoint(image,QPoint(i,point.y()),minWidth,maxWidth);
            if(dataBundle.isValidDot) return dataBundle;
            i = dataBundle.l;
        }
        else image.setPixel(i,point.y(),qRgb(128,128,128));
    }
    from = point.y();
    to = point.y() + errV;
    for(int i=from;i<=to;i++)
    {
        if(!((i>=0 && i<row) && (point.x()>=0 && point.x()<col)))
            continue;
        if(image.pixel(point.x(),i)==qRgb(0,0,0))
        {
            dataBundle = markDotByPoint(image,QPoint(point.x(),i),minWidth,maxWidth);
            if(dataBundle.isValidDot) return dataBundle;
            i = dataBundle.b;
        }
        else image.setPixel(point.x(),i,qRgb(128,128,128));
    }
    to = point.y() - errV;
    for(int i=from;i>=to;i--)
    {
        if(!((i>=0 && i<row) && (point.x()>=0 && point.x()<col)))
            continue;
        if(image.pixel(point.x(),i)==qRgb(0,0,0))
        {
            dataBundle = markDotByPoint(image,QPoint(point.x(),i),minWidth,maxWidth);
            if(dataBundle.isValidDot) return dataBundle;
            i = dataBundle.u;
        }
        else image.setPixel(point.x(),i,qRgb(128,128,128));
    }
    dataBundle.isValidDot = false;
    dataBundle.image = image;
    return  dataBundle;
}
// searchForBlackDotAndMark handle negetive and positive index

DataBundle utilImageProcForDiaLog::getBrailleChPosLeft(QImage image, QPoint point, int HdotDist, int VdotDist, int minWidth, int maxWidth, QPoint err)
{  //return modified image and character center

    QString ch = "";
    int dotCount= 0;
    QPoint totalPoint;
           totalPoint.setX(0);
           totalPoint.setY(0);
    QPoint tempPoint = point;

     for(int i=1;i<=6;i++)
     {
         DataBundle dataBundle = utilImageProcForDiaLog::searchForBlackDotAndMark(image,tempPoint,minWidth,maxWidth,err);
         image = dataBundle.image;
         if(dataBundle.isValidDot)
         {
             //image = dataBundle.image;
             tempPoint = dataBundle.dotCenter;
             dotCount++;
             ch.append("1");
         }
         else
            ch.append("0");

         totalPoint.setX(totalPoint.x()+tempPoint.x());
         totalPoint.setY(totalPoint.y()+tempPoint.y());
         tempPoint.setY(tempPoint.y()+VdotDist);

         if(i==3)
         {
             tempPoint = point;
             tempPoint.setX(tempPoint.x()+HdotDist);
         }

     }
    image.setPixel(totalPoint.x()/6,totalPoint.y()/6,qRgb(128,128,128));
    image.setPixel(totalPoint.x()/6,totalPoint.y()/6-1,qRgb(128,128,128));
    image.setPixel(totalPoint.x()/6,totalPoint.y()/6-2,qRgb(128,128,128));
    image.setPixel(totalPoint.x()/6,totalPoint.y()/6+2,qRgb(128,128,128));
    image.setPixel(totalPoint.x()/6,totalPoint.y()/6+1,qRgb(128,128,128));

    DataBundle dataBundle(image);
                dataBundle.numberOfDotInCh = dotCount;
                dataBundle.charCenter = QPoint(totalPoint.x()/6,totalPoint.y()/6);
                dataBundle.binChar = ch;
    return  dataBundle;
}

DataBundle utilImageProcForDiaLog::getBrailleChPosCenter(QImage image, QPoint center, QPoint distBetDot, int minWidth, int maxWidth, QPoint errToFindDot)
{ // remember this point is not a black point

    int HdotDist = distBetDot.x();
    int VdotDist = distBetDot.y();
    QPoint tempPoint;
    DataBundle tempDataBundle(image);
    //----------------leftupper-----------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y()-VdotDist);
    tempDataBundle = utilImageProcForDiaLog::searchForBlackDotAndMark(image,tempPoint,minWidth,maxWidth,errToFindDot);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,tempPoint,HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
    }
    //----------------RightUpper-----------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y()-VdotDist);
    tempDataBundle = utilImageProcForDiaLog::searchForBlackDotAndMark(image,tempPoint,minWidth,maxWidth,errToFindDot);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()),HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
    }
    //---------leftMiddle---------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y());
    tempDataBundle = utilImageProcForDiaLog::searchForBlackDotAndMark(image,tempPoint,minWidth,maxWidth,errToFindDot);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,QPoint(tempPoint.x(),tempPoint.y()-VdotDist),HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
    }

    //----------------rightMiddle---------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y());
    tempDataBundle = utilImageProcForDiaLog::searchForBlackDotAndMark(image,tempPoint,minWidth,maxWidth,errToFindDot);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()-VdotDist),HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
    }
    //-------------- leftBelow-----------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y()+VdotDist);
    tempDataBundle = utilImageProcForDiaLog::searchForBlackDotAndMark(image,tempPoint,minWidth,maxWidth,errToFindDot);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,QPoint(tempPoint.x(),tempPoint.y()-(2*VdotDist)),HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
    }


    //-------------- RightBelow-----------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y()+VdotDist);
    tempDataBundle = utilImageProcForDiaLog::searchForBlackDotAndMark(image,tempPoint,minWidth,maxWidth,errToFindDot);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()-(2*VdotDist)),HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
    }


    DataBundle dataBundle(image);
                dataBundle.charCenter = center;
                dataBundle.isValidDot = false;
                return  dataBundle;
}

DataBundle utilImageProcForDiaLog::findCenter(QImage image, QPoint blackPoint, QPoint distBetDot, int minWidth, int maxWidth, QPoint errToFindDot)
{
    QImage tempImage = image;
    DotBoundaryFinder ulbrObjct(tempImage);
    QList<int> list = ulbrObjct.findBoundary(blackPoint.x(),blackPoint.y());
    QPoint cntrBlckDot((list[1]+list[3])/2,(list[0]+list[2])/2);
    //qDebug()<<cntrBlckDot<<endl;
            //--------------
    QPoint leftUpper = QPoint(cntrBlckDot.x()+(distBetDot.x()/2),cntrBlckDot.y()+distBetDot.y());
    QPoint leftMiddle = QPoint(cntrBlckDot.x()+(distBetDot.x()/2),cntrBlckDot.y());
    QPoint leftBelow = QPoint(cntrBlckDot.x()+(distBetDot.x()/2),cntrBlckDot.y()-distBetDot.y());
    QPoint rightUpper = QPoint(cntrBlckDot.x()-(distBetDot.x()/2),cntrBlckDot.y()+distBetDot.y());
    QPoint rightMiddle = QPoint(cntrBlckDot.x()-(distBetDot.x()/2),cntrBlckDot.y());
    QPoint rightBelow = QPoint(cntrBlckDot.x()-(distBetDot.x()/2),cntrBlckDot.y()-distBetDot.y());
    QList<QPoint> centerList;
                  centerList.append(leftUpper);
                  centerList.append(leftMiddle);
                  centerList.append(leftBelow);
                  centerList.append(rightUpper);
                  centerList.append(rightMiddle);
                  centerList.append(rightBelow);
     DataBundle dataBundle = getBrailleChPosCenter(image,centerList[0],distBetDot,minWidth,maxWidth,errToFindDot);
     int maxChar = dataBundle.numberOfDotInCh;
     DataBundle dataBundleForMax = dataBundle;

     for(int i=1;i<=5;i++)
     {

         dataBundle = getBrailleChPosCenter(image,centerList[i],distBetDot,minWidth,maxWidth,errToFindDot);

         if(maxChar<dataBundle.numberOfDotInCh)
         {
             maxChar = dataBundle.numberOfDotInCh;
             dataBundleForMax = dataBundle;
         }

     }
     return dataBundleForMax;
}

QImage utilImageProcForDiaLog::mainImageToBinImage(QString fileName)
{
    //return QImage(imageFile);
    string file = fileName.toStdString(); //if you need std c++ string

    Mat image, gray, blur, median, blackWhite, erosion, dilation;

    image = imread(file, IMREAD_COLOR);
    Rect crop_region(25, 25, image.cols-25, image.rows-25);
    image = image(crop_region);

    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(5, 5), 0, 0);
    medianBlur(blur, median, 5);
    //threshold(blur, blackWhite, 0, 255, THRESH_BINARY | THRESH_OTSU);
    //threshold(blur, blackWhite, 0, 255, THRESH_OTSU);
    adaptiveThreshold(median, blackWhite, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 13, 6);
    //threshold( blur, blackWhite, 230, 255, THRESH_BINARY );
    erode(blackWhite, erosion, getStructuringElement(MORPH_RECT, Size(5, 5)));
    dilate(erosion, dilation, getStructuringElement(MORPH_RECT, Size(5, 5)));

    string out = "out_bin.png";

    imwrite(out, dilation);
    //QImage imgOut= QImage((uchar*) blackWhite.data, blackWhite.cols, blackWhite.rows, blackWhite.step, QImage::Format_Grayscale8);
    //cout<<endl<<blackWhite<<endl;
    //string outFile = "out_" + file;
    QString str = QString::fromUtf8(out.c_str());
    QImage qimg;
    qimg.load(str, "png");
    //cout<<"qimage loaded"<<endl;

    return qimg;  //QImage(str); // you must convert your image to QImage then return it
}
