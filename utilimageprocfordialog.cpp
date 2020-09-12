#include "utilimageprocfordialog.h"
#include "ulbrofdot.h"
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
    ULBROfDot ulbrObjct(image);
    qApp->processEvents();
    QList<int> list = ulbrObjct.findULBR(point.x(),point.y());
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
    }
    dataBundle.isValidDot = false;
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
         if(dataBundle.isValidDot)
         {
             image = dataBundle.image;
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

    DataBundle dataBundle(image);
                dataBundle.numberOfDotInCh = dotCount;
                dataBundle.charCenter = QPoint(totalPoint.x()/6,totalPoint.y()/6);
                dataBundle.binChar = ch;
    return  dataBundle;
}

DataBundle utilImageProcForDiaLog::getBrailleChPosRight(QImage image, QPoint point, int HdotDist, int VdotDist, int minWidth, int maxWidth, QPoint err)
{   // no need to search i know it is a valid black dot
    ULBROfDot ulbrObjct(image);
    QList<int> list = ulbrObjct.findULBR(point.x(),point.y());
    int centerX = (list[1]+list[3])/2;
    int centerY = (list[0]+list[2])/2;
    QPoint leftUpperDot = QPoint(centerX-HdotDist,centerY); // i am not sure this point is black or not
    return getBrailleChPosLeft(image,leftUpperDot,HdotDist,VdotDist,minWidth,maxWidth,err);
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
        return  getBrailleChPosRight(image,tempPoint,HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
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
        return  getBrailleChPosRight(image,QPoint(tempPoint.x(),tempPoint.y()-VdotDist),HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
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
        return  getBrailleChPosRight(image,QPoint(tempPoint.x(),tempPoint.y()-(2*VdotDist)),HdotDist,VdotDist,minWidth,maxWidth,errToFindDot);
    }



    DataBundle dataBundle(image);
                dataBundle.charCenter = center;
                dataBundle.isValidDot = false;
                return  dataBundle;
}

DataBundle utilImageProcForDiaLog::findCenter(QImage image, QPoint cntrBlckDot, QPoint distBetDot, int minWidth, int maxWidth, QPoint errToFindDot)
{
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

bool utilImageProcForDiaLog::isCharCenter(QImage image, QPoint cntrChar, QPoint distBetDot, int minWidth, int maxWidth, QPoint errToFindDot)
{
    DataBundle originalChar = getBrailleChPosCenter(image,cntrChar,distBetDot,minWidth,maxWidth,errToFindDot);
    if(!originalChar.isValidDot) return  false;
    int from =cntrChar.x()-distBetDot.x();
    int to = cntrChar.x()+distBetDot.x();
    for(int i=from;i<to;i+=2)
    {
        DataBundle tempDataBundle = getBrailleChPosCenter(image,QPoint(i,cntrChar.y()),distBetDot,minWidth,maxWidth,errToFindDot);
        if(tempDataBundle.numberOfDotInCh>originalChar.numberOfDotInCh)
            return  false;
    }
    from =cntrChar.y()-distBetDot.y();
    to = cntrChar.y()+distBetDot.y();
    for(int i=from;i<to;i+=2)
    {
        DataBundle tempDataBundle = getBrailleChPosCenter(image,QPoint(cntrChar.x(),i),distBetDot,minWidth,maxWidth,errToFindDot);
        if(tempDataBundle.numberOfDotInCh>originalChar.numberOfDotInCh)
            return  false;
    }
    return  true;
}

DataBundle utilImageProcForDiaLog::findChar(QImage image, QPoint startPt, QPoint distBetDot, QPoint distBetCh, int minWidth, int maxWidth, QPoint errToFindDot)
{
    int row = image.height();
    int col = image.width();
    DataBundle dataBundle = DataBundle(image);
    for(int j=startPt.y();j<row;j+=5)
    {
        for(int i=startPt.x();i<col;i+=3)
        {
            if(image.pixel(i,j)==qRgb(0,0,0))
            {
                dataBundle = markDotByPoint(image,QPoint(i,j),minWidth,maxWidth);
                if(dataBundle.isLargeDot || !dataBundle.isValidDot) continue;
                dataBundle = findCenter(image,dataBundle.dotCenter,distBetDot,minWidth,maxWidth,errToFindDot);
                QPoint leftPt = QPoint(dataBundle.charCenter.x()-distBetCh.x(),dataBundle.charCenter.y());
                QPoint rightpt = QPoint(dataBundle.charCenter.x()+distBetCh.x(),dataBundle.charCenter.y());
                if(isCharCenter(image,leftPt,distBetDot,minWidth,maxWidth,errToFindDot) && isCharCenter(image,rightpt,distBetDot,minWidth,maxWidth,errToFindDot))
                    return dataBundle;

            }
        }
    }
    dataBundle.isValidDot = false;
    return dataBundle;
}

DataBundle utilImageProcForDiaLog::readLineLeftPart(QImage image, QPoint cnterCh, QPoint distBetDot, QPoint distBetCh, int minWidth, int maxWidth, QPoint errToFindDot)
{
    QList<QString> charList;
    int isSpace = 0;
    DataBundle dataBundle(image);
    dataBundle.charCenter = QPoint(cnterCh);
    while(dataBundle.charCenter.x()>0)
    {

        dataBundle = getBrailleChPosCenter(image,dataBundle.charCenter,distBetDot,minWidth,maxWidth,QPoint((isSpace/10.0+1)*errToFindDot.x(),(isSpace/10.0+1)*errToFindDot.y()));

        if(dataBundle.isValidDot) // actually valid character
        {
            if(isSpace)
            {
                isSpace = 0;
                charList.prepend("space");

            }
            charList.prepend(dataBundle.binChar);
            image = dataBundle.image;
        }
        else
            isSpace++;
        dataBundle.charCenter.setX(dataBundle.charCenter.x()-distBetCh.x()); //corrected
    }
    dataBundle.image = image;
    dataBundle.charList = charList;
    return dataBundle;
}

DataBundle utilImageProcForDiaLog::readLineRightPart(QImage image, QPoint cnterCh, QPoint distBetDot, QPoint distBetCh, int minWidth, int maxWidth, QPoint errToFindDot)
{ // i know it is a character
    int col = image.width();
    QList<QString> charList;
    int isSpace = 0;
    DataBundle dataBundle(image);
    dataBundle.charCenter = QPoint(cnterCh);
    while(dataBundle.charCenter.x()<col)
    {

        dataBundle = getBrailleChPosCenter(image,dataBundle.charCenter,distBetDot,minWidth,maxWidth,QPoint((isSpace/10.0+1)*errToFindDot.x(),(isSpace/10.0+1)*errToFindDot.y()));

        if(dataBundle.isValidDot)
        {
            if(isSpace)
            {
                isSpace = 0;
                charList.append("space");

            }
            charList.append(dataBundle.binChar);
            image = dataBundle.image;
        }
        else
            isSpace++;
        dataBundle.charCenter.setX(dataBundle.charCenter.x()+distBetCh.x());
    }
    dataBundle.image = image;
    dataBundle.charList = charList;
    return dataBundle;
}

DataBundle utilImageProcForDiaLog::readLine(QImage image, QPoint cnterCh, QPoint distBetDot, QPoint distBetCh, int minWidth, int maxWidth, QPoint errToFindDot)
{
    DataBundle rightDataBundle = readLineRightPart(image,cnterCh,distBetDot,distBetCh,minWidth,maxWidth,errToFindDot);
    DataBundle leftDataBundle = readLineLeftPart(rightDataBundle.image,QPoint(cnterCh.x()-distBetCh.x(),cnterCh.y()),distBetDot,distBetCh,minWidth,maxWidth,errToFindDot);
    leftDataBundle.charList.append(rightDataBundle.charList);
    return leftDataBundle;
}

QImage utilImageProcForDiaLog::mainImageToBinImage(QString fileName)
{
    //return QImage(imageFile);
    string file = fileName.toStdString(); //if you need std c++ string

    Mat image, gray, blur, blackWhite;

    image = imread(file, IMREAD_COLOR);
    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(5, 5), 0, 0);
    threshold(blur, blackWhite, 0, 255, THRESH_BINARY | THRESH_OTSU);

    string out = "out_bin.png";

    imwrite(out, blackWhite);
    //QImage imgOut= QImage((uchar*) blackWhite.data, blackWhite.cols, blackWhite.rows, blackWhite.step, QImage::Format_Grayscale8);
    //cout<<endl<<blackWhite<<endl;
    //string outFile = "out_" + file;
    QString str = QString::fromUtf8(out.c_str());
    QImage qimg;
    qimg.load(str, "png");
    //cout<<"qimage loaded"<<endl;

    return qimg;  //QImage(str); // you must convert your image to QImage then return it
}
