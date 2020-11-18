#include "ulbrofdot.h"
#include "utilimageproc.h"
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

QPoint UtilImageProc::_distBetDot;
QPoint UtilImageProc::_distBetCh;
int UtilImageProc::_minDotWidth;
int UtilImageProc::_maxDotWidth;
QPoint UtilImageProc::_errToFindDot;
QPoint UtilImageProc::_errToFindCh;
UtilImageProc::UtilImageProc()
{
}

QImage UtilImageProc::drawHLine(QImage img, QRgb rgb, QPoint center1, QPoint center2)
{
    int from = std::min(center1.x(),center2.x());
    int to = std::max(center1.x(),center2.x());
    int height =(center1.y()+center2.y())/2;
    for(int i=from;i<=to;i++)
        img.setPixel(i,height,rgb);

    return  img;
}

QImage UtilImageProc::drawVLine(QImage img, QRgb rgb, QPoint center1, QPoint center2)
{
    int from = std::min(center1.y(),center2.y());
    int to = std::max(center1.y(),center2.y());
    int width =(center1.x()+center2.x())/2;
    for(int i=from;i<=to;i++)
        img.setPixel(width,i,rgb);

    return  img;
}

void UtilImageProc::markByULBR(QImage &img,QRgb rgb,QList<int> &ULBR)
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


    //return img;

}

DataBundle UtilImageProc::markDotByPoint(QImage image, QPoint point) //return dotcenter and modified image
{ // assuming point is a black dot
    ULBROfDot ulbrObjct(image);
    qApp->processEvents();
    QList<int> list = ulbrObjct.findULBR(point.x(),point.y());
    qApp->processEvents();
    int x = (list[1]+list[3])/2;
    int y = (list[0]+list[2])/2;
    //image = markByULBR(image,qRgb(128,128,128),list);
    markByULBR(image,qRgb(128,128,128),list);

    // ----preparing dataBundle for return-----

    DataBundle dataBundle(image);
               dataBundle.dotCenter = QPoint(x,y);
               if((list[3]-list[1]) < _minDotWidth && (list[2]-list[0]) < _minDotWidth)
                   dataBundle.isValidDot = false;
               else if((list[3]-list[1]) < (_minDotWidth/.45) && (list[2]-list[0]) < (_minDotWidth/.45))
                   dataBundle.isSmallDot = true;
               else if((list[3]-list[1]) > _maxDotWidth || (list[2]-list[0]) > _maxDotWidth)
               {
                   dataBundle.isLargeDot = true;
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

DataBundle UtilImageProc::searchForBlackDotAndMark(QImage &image,QPoint point)
{
    int errH = _errToFindDot.x(),errV = _errToFindDot.y();
    int row = image.height();
    int col = image.width();
    DataBundle dataBundle;

    int from = point.x();
    int to = point.x() + errH;
    for(int i=from;i<=to;i++)
    {
        if(!((i>=0 && i<col) && (point.y()>=0 && point.y()<row)))
            continue;
        if(image.pixel(i,point.y())==qRgb(0,0,0))
        {
            dataBundle = markDotByPoint(image,QPoint(i,point.y()));
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
            dataBundle = markDotByPoint(image,QPoint(i,point.y()));
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
            dataBundle = markDotByPoint(image,QPoint(point.x(),i));
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
            dataBundle = markDotByPoint(image,QPoint(point.x(),i));
            if(dataBundle.isValidDot) return dataBundle;
            i = dataBundle.u;
        }
    }
    dataBundle.isValidDot = false;
    return  dataBundle;
}
// searchForBlackDotAndMark handle negetive and positive index
DataBundle UtilImageProc::getBrailleChPosLeft(QImage image, QPoint point) //think x=col,y as qt
{  //return modified image and character center

    QString ch = "";
    int dotCount= 0;
    QPoint totalPoint;
           totalPoint.setX(0);
           totalPoint.setY(0);
    QPoint tempPoint = point;

     for(int i=1;i<=6;i++)
     {
         DataBundle dataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint);
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
         tempPoint.setY(tempPoint.y()+_distBetDot.y());

         if(i==3)
         {
             tempPoint = point;
             tempPoint.setX(tempPoint.x()+_distBetDot.x());
         }

     }

    DataBundle dataBundle(image);
                dataBundle.numberOfDotInCh = dotCount;
                dataBundle.charCenter = QPoint(totalPoint.x()/6,totalPoint.y()/6);
                dataBundle.binChar = ch;
    return  dataBundle;
}

DataBundle UtilImageProc::getBrailleChPosRight(QImage image, QPoint point)
{   // no need to search i know it is a valid black dot
    ULBROfDot ulbrObjct(image);
    QList<int> list = ulbrObjct.findULBR(point.x(),point.y());
    int centerX = (list[1]+list[3])/2;
    int centerY = (list[0]+list[2])/2;
    QPoint leftUpperDot = QPoint(centerX-_distBetDot.x(),centerY); // i am not sure this point is black or not
    return getBrailleChPosLeft(image,leftUpperDot);
}

DataBundle UtilImageProc::getBrailleChPosCenter(QImage &image, QPoint center)
{ // remember this point is not a black point

    int HdotDist = _distBetDot.x();
    int VdotDist = _distBetDot.y();
    QPoint tempPoint;
    DataBundle tempDataBundle;
    //----------------leftupper-----------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y()-VdotDist);
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,tempPoint);
    }
    //----------------RightUpper-----------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y()-VdotDist);
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosRight(image,tempPoint);
    }
    //---------leftMiddle---------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y());
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,QPoint(tempPoint.x(),tempPoint.y()-VdotDist));
    }

    //----------------rightMiddle---------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y());
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosRight(image,QPoint(tempPoint.x(),tempPoint.y()-VdotDist));
    }
    //-------------- leftBelow-----------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y()+VdotDist);
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosLeft(image,QPoint(tempPoint.x(),tempPoint.y()-(2*VdotDist)));
    }


    //-------------- RightBelow-----------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y()+VdotDist);
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint);
    if(tempDataBundle.isValidDot)
    {
        tempPoint = tempDataBundle.dotCenter;
        return  getBrailleChPosRight(image,QPoint(tempPoint.x(),tempPoint.y()-(2*VdotDist)));
    }



    DataBundle dataBundle;
                dataBundle.numberOfDotInCh = -1;
                dataBundle.charCenter = center;
                dataBundle.isValidDot = false;
                return  dataBundle;
}

DataBundle UtilImageProc::findCenter(QImage &image, QPoint cntrBlckDot)
{
    QPoint leftUpper = QPoint(cntrBlckDot.x()+(_distBetDot.x()/2),cntrBlckDot.y()+_distBetDot.y());
    //QPoint leftMiddle = QPoint(cntrBlckDot.x()+(_distBetDot.x()/2),cntrBlckDot.y());
    //QPoint leftBelow = QPoint(cntrBlckDot.x()+(_distBetDot.x()/2),cntrBlckDot.y()-_distBetDot.y());
    QPoint rightUpper = QPoint(cntrBlckDot.x()-(_distBetDot.x()/2),cntrBlckDot.y()+_distBetDot.y());
    //QPoint rightMiddle = QPoint(cntrBlckDot.x()-(_distBetDot.x()/2),cntrBlckDot.y());
    //QPoint rightBelow = QPoint(cntrBlckDot.x()-(_distBetDot.x()/2),cntrBlckDot.y()-_distBetDot.y());
    QList<QPoint> centerList;
                  centerList.append(leftUpper);
                  //centerList.append(leftMiddle);
                  //centerList.append(leftBelow);
                  centerList.append(rightUpper);
                  //centerList.append(rightMiddle);
                  //centerList.append(rightBelow);
     DataBundle dataBundle = getBrailleChPosCenter(image,centerList[0]);
     int maxChar = dataBundle.numberOfDotInCh;
     DataBundle dataBundleForMax = dataBundle;

     for(int i=1;i<centerList.count();i++)
     {

         dataBundle = getBrailleChPosCenter(image,centerList[i]);

         if(maxChar<dataBundle.numberOfDotInCh)
         {
             maxChar = dataBundle.numberOfDotInCh;
             dataBundleForMax = dataBundle;
         }

     }
     return dataBundleForMax;
}

DataBundle UtilImageProc::findChar(QImage image, QPoint startPt)
{
    int row = image.height();
    int col = image.width();
    DataBundle dataBundle,dataBundle2;
    for(int j=startPt.y();j<row;j+=5)
    {
        for(int i=startPt.x();i<col;i+=3)
        {
            if(image.pixel(i,j)==qRgb(0,0,0))
            {
                dataBundle = markDotByPoint(image,QPoint(i,j));
                if(dataBundle.isLargeDot || !dataBundle.isValidDot || dataBundle.isSmallDot)
                {
                    i+=15;
                    continue;
                }
                dataBundle = findCenter(image,dataBundle.dotCenter);
                if(!dataBundle.isValidDot || !dataBundle.isValidCh())
                {
                    i+=15;
                    continue;
                }
                /*QPoint leftPt = QPoint(dataBundle.charCenter.x()-_distBetCh.x(),dataBundle.charCenter.y());
                QPoint rightpt = QPoint(dataBundle.charCenter.x()+_distBetCh.x(),dataBundle.charCenter.y());
                if( isCharCenter(image,rightpt) || isCharCenter(image,leftPt)) // replaced && by || reduced Half time!!
                    return dataBundle;*/
                //--------------------------new------------------

//                dataBundle2 = getBrailleChPosCenter(image, QPoint(dataBundle.charCenter.x(),dataBundle.charCenter.y()*3));
//                if(dataBundle2.isValidDot)
//                {
//                    i+=15;
//                    continue;
//                }
                return  dataBundle;
                //----------------------

            }
        }
    }
    dataBundle.isValidDot = false;
    return dataBundle;
}

DataBundle UtilImageProc::readLineLeftPart(QImage image, QPoint cnterCh)
{
    QList<QString> charList;
    int isSpace = 0;
    DataBundle dataBundle(image);
	dataBundle.charCenter = QPoint(cnterCh);
    while(dataBundle.charCenter.x()>0)
    {

        dataBundle = getBrailleChPosCenter(image,dataBundle.charCenter);

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
        dataBundle.charCenter.setX(dataBundle.charCenter.x()-_distBetCh.x()); //corrected
    }
    dataBundle.image = image;
    dataBundle.charList = charList;
    return dataBundle;
}

DataBundle UtilImageProc::readLineRightPart(QImage image, QPoint cnterCh)
{ // i know it is a character
    int col = image.width();
    QList<QString> charList;
    int isSpace = 0;
    DataBundle dataBundle(image);
    dataBundle.charCenter = QPoint(cnterCh);
    while(dataBundle.charCenter.x()<col)
    {

        dataBundle = getBrailleChPosCenter(image,dataBundle.charCenter);

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
        dataBundle.charCenter.setX(dataBundle.charCenter.x()+_distBetCh.x());
    }
    dataBundle.image = image;
    dataBundle.charList = charList;
    return dataBundle;
}

DataBundle UtilImageProc::readLine(QImage image, QPoint cnterCh)
{
    DataBundle rightDataBundle = readLineRightPart(image,cnterCh);
    DataBundle leftDataBundle = readLineLeftPart(rightDataBundle.image,QPoint(cnterCh.x()-_distBetCh.x(),cnterCh.y()));
    leftDataBundle.charList.append(rightDataBundle.charList);
    return leftDataBundle;
}

QImage UtilImageProc::mainImageToBinImage(QString fileName)
{
    //return QImage(imageFile);
    string file = fileName.toStdString(); //if you need std c++ string

    Mat image, gray, blur, blackWhite, erosion, dilation;

    image = imread(file, IMREAD_COLOR);
    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(5, 5), 0, 0);
    threshold(blur, blackWhite, 0, 255, THRESH_BINARY | THRESH_OTSU);
    
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

void UtilImageProc::setBrailleProperty(QPoint distBetDot, QPoint distBetCh, int minDotWidth, int maxDotWidth, QPoint errToFindDot, QPoint errToFindCh)
{
    _distBetDot = distBetDot;
    _distBetCh = distBetCh;
    _minDotWidth = minDotWidth;
    _maxDotWidth = maxDotWidth;
    _errToFindDot = errToFindDot;
    _errToFindCh = errToFindCh;
}





