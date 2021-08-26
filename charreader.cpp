#include "charreader.h"

CharReader::CharReader()
{

}

DataBundle CharReader::getBrailleChPosCenter(const QImage &image, QPoint center)
{ // remember this point is not a black point

    int HdotDist = _distBetDot.x();
    int VdotDist = _distBetDot.y();
    QPoint tempPoint;
    DataBundle tempDataBundle;
    //----------------leftupper-----------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y()-VdotDist);
    tempDataBundle = dotProcessor.searchForBlackDotAndMark(image,tempPoint,false);

    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge) // we can depend
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,tempPoint);
    }
    //----------------RightUpper-----------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y()-VdotDist);
    tempDataBundle = dotProcessor.searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge)
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()));
    }
    //---------leftMiddle---------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y());
    tempDataBundle = dotProcessor.searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge)
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x(),tempPoint.y()-VdotDist));
    }

    //----------------rightMiddle---------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y());
    tempDataBundle = dotProcessor.searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge)
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()-VdotDist));
    }
    //-------------- leftBelow-----------------
    tempPoint = QPoint(center.x()-(HdotDist/2),center.y()+VdotDist);
    tempDataBundle = dotProcessor.searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    if(tempDataBundle.isValidDot && !tempDataBundle.isVeryLarge)
    {
        tempPoint = tempDataBundle.dotCenter;
        return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x(),tempPoint.y()-(2*VdotDist)));

    }


    //-------------- RightBelow-----------------
    tempPoint = QPoint(center.x()+(HdotDist/2),center.y()+VdotDist);
    tempDataBundle = dotProcessor.searchForBlackDotAndMark(tempDataBundle.image,tempPoint,false);
    tempPoint = tempDataBundle.dotCenter;
    return getBrailleChPosLeft(tempDataBundle.image,QPoint(tempPoint.x()-HdotDist,tempPoint.y()-(2*VdotDist)));

}

DataBundle CharReader::getBrailleChPosLeft(QImage image, QPoint point)
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
         DataBundle dataBundle = dotProcessor.searchForBlackDotAndMark(image,tempPoint,_isDebug);
         if(dataBundle.shouldInCharIdentification) shouldInCharCounter++;
         //shouldInCharIdentification = shouldInCharIdentification && dataBundle.shouldInCharIdentification; // if anyone is middle size
         if(dataBundle.isValidDot) // means dot exist and grether than minimum requirement
         {
             //tempPoint = dataBundle.dotCenter;
             dotCount++;
             ch.append("1");
         }
         else   //dot doesnt exist or exist but lower than minimum requirement and it is not marked by 128,128,128
            ch.append("0");
         image = dataBundle.image; // save modified image
//         totalPoint.setX(totalPoint.x()+tempPoint.x());
//         totalPoint.setY(totalPoint.y()+tempPoint.y());
         // editing
         totalPoint.setX(totalPoint.x()+dataBundle.dotCenter.x());
         totalPoint.setY(totalPoint.y()+dataBundle.dotCenter.y());

         // end
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
