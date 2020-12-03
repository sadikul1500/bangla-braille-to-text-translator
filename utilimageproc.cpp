#include "ulbrofdotv4.h"
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
bool UtilImageProc::findingChar = false;
bool UtilImageProc::shouldPrint = false;
bool UtilImageProc::_isDebug = false;
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


    //return img;

}

DataBundle UtilImageProc::markDotByPoint(QImage image, QPoint point,bool mark) //return dotcenter and modified image
{ // assuming point is a black dot
    // return: black pixel turns 1,1,1 only for very large dot //not by ref and by ref
    // return image can also be marked //not by ref and by ref
    if(!(image.pixel(point.x(),point.y())==qRgb(0,0,0))){
        qDebug()<<"Error: markDotByPoint (x,y): "<<point.x()<<","<<point.y()<<endl;
        return DataBundle();
    }
    QImage originalImg = image;
    ULBRofDotV4 ulbrObjct(image);
    qApp->processEvents();

    QList<int> list = ulbrObjct.findULBR(point.x(),point.y());
    qApp->processEvents();
    int x = (list[1]+list[3])/2;
    int y = (list[0]+list[2])/2;

    if((list[3]-list[1]) > 2*_maxDotWidth || (list[2]-list[0]) > 2*_maxDotWidth) mark = false; // because mark intersect others!!
    if(_isDebug && mark) markByULBR(originalImg,qRgb(128,128,128),list);
    if(shouldPrint) qDebug()<<"dot size: "<<QPoint((list[3]-list[1]),(list[2]-list[0]))<<"minMax: "<<QPoint(_minDotWidth,_maxDotWidth)<<endl;

    DataBundle dataBundle(originalImg);
               dataBundle.dotCenter = QPoint(x,y);
               if((list[3]-list[1]) < _minDotWidth && (list[2]-list[0]) < _minDotWidth){
                    dataBundle.dotCenter = point;
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

DataBundle UtilImageProc::searchForBlackDotAndMark(const QImage &image,QPoint point,bool mark) // important this function by reference modify nothing!!!
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
            return markDotByPoint(dataBundle.image,QPoint(pt.x(),pt.y()),mark);
             // return valid dot with other state(is large,is very large)
        }
        else if(_isDebug && mark) dataBundle.image.setPixel(pt.x(),pt.y(),qRgb(128,128,128));
    }

    dataBundle.isValidDot = false; // invalid dot for lesser size than minimum requirement including no dot found
    dataBundle.dotCenter = point;
    if(shouldPrint) qDebug()<<"no dot found"<<endl;
    return  dataBundle;
}

// searchForBlackDotAndMark handle negetive and positive index
DataBundle UtilImageProc::getBrailleChPosLeft(QImage image, QPoint point) //think x=col,y as qt
{  //return modified image and character center

    QString ch = "";
    int dotCount= 0;
    int shouldInCharCounter = 0;
    QPoint totalPoint;
           totalPoint.setX(0);
           totalPoint.setY(0);
    QPoint tempPoint = point;
    QList<QString> printLst;
    if(shouldPrint) printLst<<"leftUp: "<<"leftMd: "<<"leftDn: "<<"rightUp: "<<"rightMd: "<<"rightDn: ";
    //bool shouldInCharIdentification = true;
     for(int i=1;i<=6;i++)
     {
         if(shouldPrint) qDebug()<<printLst[i-1]<<endl;
         DataBundle dataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint,_isDebug);
         if(dataBundle.shouldInCharIdentification) shouldInCharCounter++;
         //shouldInCharIdentification = shouldInCharIdentification && dataBundle.shouldInCharIdentification; // if anyone is middle size
         if(dataBundle.isValidDot) // means dot exist and grether than minimum requirement
         {
             tempPoint = dataBundle.dotCenter;
             dotCount++;
             ch.append("1");
         }
         else   //dot doesnt exist or exist but lower than minimum requirement and it is not marked by 128,128,128
            ch.append("0");
         image = dataBundle.image; // save modified image
         totalPoint.setX(totalPoint.x()+tempPoint.x());
         totalPoint.setY(totalPoint.y()+tempPoint.y());
         tempPoint.setY(tempPoint.y()+_distBetDot.y());

         if(i==3)
         {
             tempPoint = point;
             tempPoint.setX(tempPoint.x()+_distBetDot.x());
         }

     }
    if(shouldPrint) qDebug()<<"should in line identification(at least one medium dot): "<<shouldInCharCounter<<endl;
    DataBundle dataBundle(image);
                dataBundle.numberOfDotInCh = dotCount;
                dataBundle.charCenter = QPoint(totalPoint.x()/6,totalPoint.y()/6);
                dataBundle.binChar = ch;
                dataBundle.shouldInCharIdentification = shouldInCharCounter>1?true :false; // at least support by two
    return  dataBundle;
}

DataBundle UtilImageProc::getBrailleChPosCenter(const QImage &image, QPoint center) // important by reference it returns nothing!!!
{ // remember this point is not a black point

    int HdotDist = _distBetDot.x();
    int VdotDist = _distBetDot.y();
    QPoint tempPoint;
    DataBundle tempDataBundle;
    //----------------leftupper-----------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y()-VdotDist);
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(image,tempPoint,false);

    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge) // we can depend
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,tempPoint);
    }
    //----------------RightUpper-----------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y()-VdotDist);
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge)
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()));
    }
    //---------leftMiddle---------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y());
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge)
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x(),tempPoint.y()-VdotDist));
    }

    //----------------rightMiddle---------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y());
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge)
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()-VdotDist));
    }
    //-------------- leftBelow-----------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y()+VdotDist);
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge)
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x(),tempPoint.y()-(2*VdotDist)));

    }


    //-------------- RightBelow-----------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y()+VdotDist);
    tempDataBundle = UtilImageProc::searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    tempPoint = tempDataBundle.dotCenter;
    return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()-(2*VdotDist)));

}

DataBundle UtilImageProc::findCenter(const QImage &image, QPoint cntrBlckDot) // important returns nothin by reference
{   // only used by extract() method
    QPoint leftUpper = QPoint(cntrBlckDot.x()+(_distBetDot.x()/2),cntrBlckDot.y()+_distBetDot.y());
    QPoint rightUpper = QPoint(cntrBlckDot.x()-(_distBetDot.x()/2),cntrBlckDot.y()+_distBetDot.y());
    if(shouldPrint) qDebug()<<"------------consdering left Upper---------------"<<endl;
     DataBundle dataBundle1 = getBrailleChPosCenter(image,leftUpper); // they do not modify image
     if(shouldPrint) qDebug()<<"------------consdering right Upper---------------"<<endl;
     DataBundle dataBundle2 = getBrailleChPosCenter(image,rightUpper);
     return dataBundle1.numberOfDotInCh>dataBundle2.numberOfDotInCh ? dataBundle1 : dataBundle2;
}

DataBundle UtilImageProc::extractChar(QImage &image, QPoint blackPix) // return nothing by ref by in case very large dot
{   // this function only used by line identification purpose
    if(!(image.pixel(blackPix.x(),blackPix.y())==qRgb(0,0,0))){
        qDebug()<<"Error: extractChar(): "<<blackPix.x()<<","<<blackPix.y()<<endl;
        return DataBundle();
    }
    DataBundle dataBundle;
    dataBundle = markDotByPoint(image,blackPix,false);
    if(dataBundle.isVeryLarge || dataBundle.isLargeDot || !dataBundle.shouldInCharIdentification){

        if(shouldPrint) qDebug()<<"extractChar(): "<<"problem with given dot"<<endl;
        if(dataBundle.isVeryLarge) image = dataBundle.image; // return by ref
        dataBundle.isValidDot = false;
        return dataBundle;
    }
    if(shouldPrint) qDebug()<<"extractChar(): "<<"no problem with given dot"<<endl;
    // okk this dot is almost perfect

    dataBundle = findCenter(image,dataBundle.dotCenter);
    if(!dataBundle.isValidCh() || !dataBundle.shouldInCharIdentification) //!dataBundle.isValidDot ||
    {
        if(shouldPrint) qDebug()<<"extractChar(): "<<"number of dots: "<<dataBundle.numberOfDotInCh<<endl;
        dataBundle.isValidDot = false;
        return dataBundle;
    }

    // okk character is also perfect
    // now mark this character and return


    int u = dataBundle.charCenter.y()-_distBetDot.y();
    int b = dataBundle.charCenter.y()+_distBetDot.y();
    int l = dataBundle.charCenter.x()-_distBetDot.x()/2;
    int r = dataBundle.charCenter.x()+_distBetDot.x()/2;
    if(_isDebug) markByULBR(image,qRgb(128,128,128),QList<int>()<<u<<l<<b<<r);
    if(_isDebug) markByULBR(image,qRgb(128,128,128),QList<int>()<<u-3<<l-3<<b+3<<r+3);
    dataBundle.image = image;
    return dataBundle;

}

DataBundle UtilImageProc::findChar(QImage image, QPoint startPt)
{
    int row = image.height();
    int col = image.width();
    DataBundle dataBundle;
    for(int j=startPt.y();j<row;j+=5)
    {
        for(int i=startPt.x();i<col;i+=3)
        {
            if(image.pixel(i,j)==qRgb(0,0,0))
            {

                dataBundle = extractChar(image,QPoint(i,j));
                if(dataBundle.isValidDot){

                    return dataBundle;
                }
                else i+=15; //continue

            }
            else if(_isDebug)  image.setPixel(i,j,qRgb(128,128,128));
        }
    }
    dataBundle.isValidDot = false;
    dataBundle.image = image;
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
        //qDebug()<<"come: "<<dataBundle.charCenter<<" "<<_distBetCh.x()<<endl;
        dataBundle = getBrailleChPosCenter(dataBundle.image,dataBundle.charCenter);

        if(dataBundle.numberOfDotInCh) // not an empty char
        {
            if(isSpace)
            {
                isSpace = 0;
                charList.prepend("space");

            }
            charList.prepend(dataBundle.binChar);
            //image = dataBundle.image;
        }
        else
            isSpace++;

        dataBundle.charCenter.setX(dataBundle.charCenter.x()-_distBetCh.x());
    }
    //dataBundle.image = image;
    dataBundle.charList = charList;
    return dataBundle;
}

DataBundle UtilImageProc::readLineRightPart(QImage image, QPoint cnterCh)
{ // i know it is a character
    int col = image.width();
    QList<QString> charList;
    int isSpace = 0;
    DataBundle dataBundle(image);
    //qDebug()<<"start"<<image.size()<<endl;
    dataBundle.charCenter = QPoint(cnterCh);
    while(dataBundle.charCenter.x()<col)
    {

        //qDebug()<<"------------------getBrailleChPosCenter line------------"<<endl;
        //qDebug()<<"befor"<<dataBundle.image.size()<<dataBundle.charCenter<<endl;
        dataBundle = getBrailleChPosCenter(dataBundle.image,dataBundle.charCenter);
        //qDebug()<<"------------------end getBrailleChPosCenter line------------"<<endl;
        //qDebug()<<"after"<<dataBundle.image.size()<<endl;
        if(dataBundle.numberOfDotInCh) // not an empty char
        {
            if(isSpace)
            {
                isSpace = 0;
                charList.append("space");

            }
            charList.append(dataBundle.binChar);
            //image = dataBundle.image;
        }
        else
            isSpace++;

        dataBundle.charCenter.setX(dataBundle.charCenter.x()+_distBetCh.x());
    }
    //qDebug()<<"end"<<dataBundle.image.size()<<endl;
    //dataBundle.image = image;
    dataBundle.charList = charList;
    return dataBundle;
}

DataBundle UtilImageProc::readLine(QImage image, QPoint cnterCh)
{
    //qDebug()<<"------------------right line------------"<<endl;
    //qDebug()<<image.size()<<endl;
    DataBundle rightDataBundle = readLineRightPart(image,cnterCh);
    //qDebug()<<"readline"<<rightDataBundle.image.size()<<endl;
//    qDebug()<<"------------------end right line------------"<<endl;
    //qDebug()<<"------------------left line------------"<<endl;
    DataBundle leftDataBundle = readLineLeftPart(rightDataBundle.image,QPoint(cnterCh.x()-_distBetCh.x(),cnterCh.y()));
    //qDebug()<<"------------------end left line------------"<<endl;
    //qDebug()<<leftDataBundle.image.size()<<endl;
    leftDataBundle.charList.append(rightDataBundle.charList);
    return leftDataBundle;
}

QImage UtilImageProc::mainImageToBinImage(QString fileName)
{
    //return QImage(imageFile);
    string file = fileName.toStdString(); //if you need std c++ string

    Mat image, gray, blur, median, blackWhite, erosion, dilation;

    image = imread(file, IMREAD_COLOR);
    Rect crop_region(25, 100, image.cols-30, image.rows-130);
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

void UtilImageProc::setBrailleProperty(QPoint distBetDot, QPoint distBetCh, int minDotWidth, int maxDotWidth, QPoint errToFindDot, QPoint errToFindCh,bool ShouldPrint,bool isDebug)
{
    _distBetDot = distBetDot;
    _distBetCh = distBetCh;
    _minDotWidth = minDotWidth;
    _maxDotWidth = maxDotWidth;
    _errToFindDot = errToFindDot;
    _errToFindCh = errToFindCh;
    shouldPrint = ShouldPrint;
    _isDebug = isDebug;
}





