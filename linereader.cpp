#include "linereader.h"

Linereader::Linereader()
{

}

DataBundle Linereader::readLine(QImage image, QPoint cnterCh)
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


DataBundle Linereader::readLineRightPart(QImage image, QPoint cnterCh)
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
        dataBundle = charReader.getBrailleChPosCenter(dataBundle.image,dataBundle.charCenter);
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


DataBundle Linereader::readLineLeftPart(QImage image, QPoint cnterCh)
{
    QList<QString> charList;
    int isSpace = 0;
    DataBundle dataBundle(image);
    dataBundle.charCenter = QPoint(cnterCh);
    while(dataBundle.charCenter.x()>0)
    {
        //qDebug()<<"come: "<<dataBundle.charCenter<<" "<<_distBetCh.x()<<endl;
        dataBundle = charReader.getBrailleChPosCenter(dataBundle.image,dataBundle.charCenter);

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
