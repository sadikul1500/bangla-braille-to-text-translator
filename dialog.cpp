#include "dialog.h"
#include "ui_dialog.h"
#include "ulbrofdot.h"
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSizePolicy>
#include <iostream>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include "utilimageprocfordialog.h"
using namespace  std;
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{

    initComponent();

    createUI();
    connect(openBtn,SIGNAL(clicked()),this,SLOT(openBtnHandler()));
    connect(zoomInBtn,&QPushButton::clicked,[=](){
        imageLabel->scaleImage(1.25);
        adjustScrollBar(1.25);
    });

    connect(zoomOutBtn,&QPushButton::clicked,[=](){
        imageLabel->scaleImage(0.8);
        adjustScrollBar(0.8);
    });

    //connect(imageLabel,SIGNAL(center1),this,SLOT(setCenter1(QPoint  point)));
    connect(imageLabel,SIGNAL(center1()),this,SLOT(setCenter1()));
    connect(imageLabel,SIGNAL(center2()),this,SLOT(setCenter2()));
    connect(imageLabel,SIGNAL(imagePixel()),this,SLOT(setPosAndPix()));
    connect(addHBtn,SIGNAL(clicked()),this,SLOT(addItemToHComboBox()));
    connect(addVBtn,SIGNAL(clicked()),this,SLOT(addItemToVComboBox()));
    connect(cancelBtn,&QPushButton::clicked,[=](){ reject(); });
    connect(okBtn,&QPushButton::clicked,[=](){ saveTrainedVariable();accept(); });
    connect(nextBtn,SIGNAL(clicked()),this,SLOT(nextBtnHandler()));


    resize(900,600);

}
QPoint Dialog::getAvgStd(QComboBox *comboBox)
{
    int max,min,avg,std,temp;
    min = comboBox->itemText(0).toInt();
    max = min;
    avg = min;
    for(int i=1;i<comboBox->count();i++)
    {
        temp = comboBox->itemText(i).toInt();
        avg += temp;
        if(min>temp)
            min = temp;
        if(max<temp)
            max = temp;
    }
    avg /= comboBox->count();
    std = (max-min)/2;
    return QPoint(avg,std);
}
void Dialog::nextBtnHandler()
{
    //----------------replace button-----------------
    dotDistCheckBox->setCheckable(true);
    dotDistCheckBox->setChecked(true);
    dotDistCheckBox->setDisabled(true);
    charDistCheckBox->setEnabled(true);
    charDistCheckBox->setCheckable(false);
    openSeltdXSeltdY->addWidget(dotPosLeft);
    openSeltdXSeltdY->addWidget(dotPosRight);
    openSeltdXSeltdY->addSpacing(50);
    okCanceLay->replaceWidget(nextBtn,okBtn);
    //--------------------replaced--------------------
    resetAllUI();
    placeImageToImageLabel(SelectedFile);
    if(SelectedFile!=" ") imageLabel->scaleImage(1);
    setDotProperty();           //retrieve dot property to identify a charachter
    avgStdXShower->setText(QString("%1,%2").arg(avgDotX).arg(stdDotX));
    avgStdYShower->setText(QString("%1,%2").arg(avgDotY).arg(stdDotY));
    m_isNextButtonClked = true;
}
void Dialog::addItemToHComboBox()
{
    QString str = hDistShower->text();
    if(!str.isEmpty())
    {
        horizontalComboBox->insertItem(0,str);
        horizontalComboBox->setCurrentIndex(0);
        hDistShower->clear();
        QPoint point = getAvgStd(horizontalComboBox);
        if(m_isNextButtonClked)
        {
            avgChX = point.x();
            stdChX = point.y();
            avgStdXShower->setText(QString("%1,%2").arg(avgChX).arg(stdChX));
        }
        else
        {
            avgDotX = point.x();
            stdDotX = point.y();
            avgStdXShower->setText(QString("%1,%2").arg(avgDotX).arg(stdDotX));
        }


        m_image = utilImageProcForDiaLog::drawHLine(m_image,qRgb(128,128,128),center1,center2);
        imageLabel->setPixmap(QPixmap::fromImage(m_image));
    }
}
void Dialog::addItemToVComboBox()
{
    QString str = vDistShower->text();
    if(!str.isEmpty())
    {
        varticalComboBox->insertItem(0,str);
        varticalComboBox->setCurrentIndex(0);
        vDistShower->clear();
        QPoint point = getAvgStd(varticalComboBox);
        if(m_isNextButtonClked)
        {
            avgChY = point.x();
            stdChY = point.y();
            avgStdYShower->setText(QString("%1,%2").arg(avgChY).arg(stdChY));
        }
        else
        {
            avgDotY = point.x();
            stdDotY = point.y();
            avgStdYShower->setText(QString("%1,%2").arg(avgDotY).arg(stdDotY));
        }

        m_image = utilImageProcForDiaLog::drawVLine(m_image,qRgb(128,128,128),center1,center2);
        imageLabel->setPixmap(QPixmap::fromImage(m_image));
    }
}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::setCenter1() //on ctrl + click
{
    if(m_image.pixel(imageLabel->point.x(),imageLabel->point.y())!=qRgb(0,0,0))
        return;
    if(m_isNextButtonClked)
        processChar(true);
    else
        processDot(true);
    imageLabel->setPixmap(QPixmap::fromImage(m_image));
    QString strPt = QString("%1,%2").arg(center1.x()).arg(center1.y());
    seltdCenterXShower->setText(strPt);
    if(!center1.isNull() && !center2.isNull())
    {
        updateDistance();
    }

}
void Dialog::setCenter2()  //on alt + click
{
    if(m_image.pixel(imageLabel->point.x(),imageLabel->point.y())!=qRgb(0,0,0))
        return;

    if(m_isNextButtonClked)
        processChar(false);
    else
        processDot(false);
    imageLabel->setPixmap(QPixmap::fromImage(m_image));
    QString strPt = QString("%1,%2").arg(center2.x()).arg(center2.y());
    seltdCenterYShower->setText(strPt);
    if(!center1.isNull() && !center2.isNull())
    {
        updateDistance();
    }
}
void Dialog::setPosAndPix()
{
    int x = imageLabel->point.x();
    int y = imageLabel->point.y();
    QString strPt = QString("%1,%2").arg(x).arg(y);
    pxPosShower->setText(strPt);
    QRgb rgb = m_image.pixel(x,y);
    QColor color(rgb);
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    strPt = QString("%1,%2,%3").arg(red).arg(green).arg(blue);
    rgbShower->setText(strPt);
}
void Dialog::adjustScrollBar(double factor)
{
    QScrollBar *hScrollBar = scrollArea->horizontalScrollBar();
    QScrollBar *vScrollBar = scrollArea->verticalScrollBar();
    hScrollBar->setValue(int(factor * hScrollBar->value()
                                 + ((factor - 1) * hScrollBar->pageStep()/2)));

    vScrollBar->setValue(int(factor * vScrollBar->value()
                                 + ((factor - 1) * vScrollBar->pageStep()/2)));
}
void Dialog::createUI()
{
    //-----QLabel------------------

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    QHBoxLayout *hLay = new QHBoxLayout();
                hLay->addWidget(horizontalComboBox);
                hLay->addWidget(hDistShower);
                hLay->addWidget(addHBtn);
                hLay->addWidget(varticalComboBox);
                hLay->addWidget(vDistShower);
                hLay->addWidget(addVBtn);
                hLay->addWidget(zoomInBtn);
                hLay->addWidget(zoomOutBtn);
    QHBoxLayout *hLay2 = new QHBoxLayout;
                hLay2->addWidget(new QLabel("PixelPos:"));
                hLay2->addWidget(pxPosShower);
                hLay2->addWidget(new QLabel("Pixel: "));
                hLay2->addWidget(rgbShower);
                hLay2->addWidget(new QLabel("AvX,StdX "));
                hLay2->addWidget(avgStdXShower);
                hLay2->addWidget(new QLabel("AvY,StdY "));
                hLay2->addWidget(avgStdYShower);
                hLay2->addWidget(new QLabel("DotAvgX,cur(X,Y) "));
                hLay2->addWidget(avgCurDotWithShower);
    QHBoxLayout *seltdCentXLay = new QHBoxLayout;
                QLabel *label = new QLabel("Cent1");
                        label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                 seltdCentXLay->addWidget(label);
                 seltdCentXLay->addWidget(seltdCenterXShower);
    QHBoxLayout *seltdCentYLay = new QHBoxLayout;
                label = new QLabel("Cent2");
                label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                seltdCentYLay->addWidget(label);
                seltdCentYLay->addWidget(seltdCenterYShower);
    openSeltdXSeltdY = new QVBoxLayout;
                openSeltdXSeltdY->addWidget(openBtn);
                openSeltdXSeltdY->addLayout(seltdCentXLay);
                openSeltdXSeltdY->addLayout(seltdCentYLay);
                openSeltdXSeltdY->addSpacing(50);
                //append dotPosLeft
                //append dotPosRight
                //append a spacing for CharDist
    QVBoxLayout *left = new QVBoxLayout;
                left->addLayout(openSeltdXSeltdY);
                left->addWidget(workingOnBtn);
                left->addWidget(dotDistCheckBox);
                left->addWidget(charDistCheckBox);
                left->addSpacerItem(new QSpacerItem(5,5,QSizePolicy::Minimum,QSizePolicy::Expanding));

    okCanceLay = new QHBoxLayout;
                okCanceLay->addSpacerItem(new QSpacerItem(5,5,QSizePolicy::Expanding));
                okCanceLay->addWidget(nextBtn);
                okCanceLay->addWidget(cancelBtn);
    QVBoxLayout *right = new QVBoxLayout;
                right->addLayout(hLay);
                right->addWidget(scrollArea);
                right->addLayout(hLay2);
                right->addLayout(okCanceLay);

    QHBoxLayout *lay = new QHBoxLayout(this);
                lay->addLayout(left);
                lay->addLayout(right);
}
void Dialog::initComponent()
{
        //ImageLabel
            imageLabel = new ImageLabel;
            imageLabel->setBackgroundRole(QPalette::Base);
            imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            imageLabel->setScaledContents(true);
        // ----combobox---declaration-----------
            horizontalComboBox = new QComboBox;
            varticalComboBox = new QComboBox;

        // ----QCheckBox---declaration-------------
            dotDistCheckBox = new QCheckBox("Dot Distance");
                       //dotDistCheckBox->setDisabled(true);
                       //dotDistCheckBox->setChecked(true);
                       dotDistCheckBox->setCheckable(false);
            charDistCheckBox = new QCheckBox("Char Distance");
                       charDistCheckBox->setDisabled(true);
        // --------QRadioButton------------
           dotPosLeft = new QRadioButton("Dot Position Left");
           dotPosRight = new QRadioButton("Dot Position Right");
            //----QLineEdit---------------
            hDistShower = new QLineEdit;
                    hDistShower->setText("");
                    hDistShower->setDisabled(true);
            vDistShower = new QLineEdit;
                    vDistShower->setDisabled(true);
            seltdCenterXShower = new QLineEdit;
                        //seltdCenterXShower->setPlaceholderText(QString("ctrl+click"));
                        seltdCenterXShower->setFixedSize(QSize(80,20));
                        seltdCenterXShower->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                        seltdCenterXShower->setDisabled(true);
            seltdCenterYShower = new QLineEdit;
                        //seltdCenterYShower->setPlaceholderText(QString("alt+click"));
                        seltdCenterYShower->setFixedSize(QSize(80,20));
                        seltdCenterYShower->setDisabled(true);
            rgbShower = new QLineEdit;
                        //rgbShower->setPlaceholderText(QString("click"));
                        rgbShower->setDisabled(true);
            avgCurDotWithShower = new QLineEdit;
                        avgCurDotWithShower->setDisabled(true);
            pxPosShower = new QLineEdit();
                       //pxPosShower->setPlaceholderText(QString("click"));
                       pxPosShower->setDisabled(true);
            avgStdXShower = new QLineEdit();
                       avgStdXShower->setDisabled(true);
            avgStdYShower = new QLineEdit();
                       avgStdYShower->setDisabled(true);

        // ----QPushButton-----declaration------
            workingOnBtn = new QPushButton("Working On");
                workingOnBtn->setFixedSize(QSize(133,20));
            nextBtn = new QPushButton("Next");
                    nextBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            cancelBtn = new QPushButton("Cancel");
                 cancelBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            okBtn = new QPushButton("Ok");
                okBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            zoomInBtn = new QPushButton("Zoom In");
            zoomOutBtn = new QPushButton("Zoom Out");
            addHBtn = new QPushButton("Add HList");
            addVBtn = new QPushButton("Add VList");
            openBtn = new QPushButton("Choose");
                openBtn->setFixedSize(QSize(133,25));
}
void Dialog::resetAllUI() //not member variable
{
    seltdCenterYShower->clear();
    seltdCenterYShower->setPlaceholderText(QString("alt+click"));
    seltdCenterXShower->clear();
    seltdCenterXShower->setPlaceholderText(QString("ctrl+click"));
    rgbShower->clear();
    rgbShower->setPlaceholderText(QString("click"));
    pxPosShower->clear();
    pxPosShower->setPlaceholderText(QString("click"));
    avgStdXShower->clear();
    avgStdYShower->clear();
    varticalComboBox->clear();
    horizontalComboBox->clear();
    hDistShower->clear();
    vDistShower->clear();
    imageLabel->clear();
    imageLabel->setScaleFactor(1);
}
void Dialog::placeImageToImageLabel(QString path)
{
    if(path==" ") return;
    m_image = utilImageProcForDiaLog::mainImageToBinImage(path);
    //m_image = QImage(path);
    imageLabel->setPixmap(QPixmap::fromImage(m_image));

}
void Dialog::processDot(bool cntr1)
{

    DataBundle dataBundle = utilImageProcForDiaLog::markDotByPoint(m_image,QPoint(imageLabel->point.x(),imageLabel->point.y()),minDotWidth,maxDotWidth); // here no problem with min and max because intitial is zero
    counter++;
    //qDebug()<<"X: "<<dataBundle.r-dataBundle.l<<endl;
    avgDotWidth = ((counter-1)*avgDotWidth+(dataBundle.r-dataBundle.l))/counter;
    avgCurDotWithShower->setText(QString("%1,%2,%3").arg(avgDotWidth).arg(dataBundle.r-dataBundle.l).arg(dataBundle.b-dataBundle.u));
    if(cntr1)
    {
        center1 = dataBundle.dotCenter;
        tempMinWidthSlctdCenter1 = std::min(tempMinWidthSlctdCenter1,dataBundle.r-dataBundle.l);
        tempMaxWidthSlctdCenter1 = std::max(tempMaxWidthSlctdCenter1,dataBundle.r-dataBundle.l);

    }
    else
    {
        center2 = dataBundle.dotCenter;
        tempMinWidthSlctdCenter2 = std::min(tempMinWidthSlctdCenter2,dataBundle.r-dataBundle.l);
        tempMaxWidthSlctdCenter2 = std::max(tempMaxWidthSlctdCenter2,dataBundle.r-dataBundle.l);
    }
    m_image = dataBundle.image;
}
void Dialog::processChar(bool cntr1)
{
     DataBundle dataBundle(m_image);
     dataBundle = utilImageProcForDiaLog::findCenter(m_image,QPoint(imageLabel->point.x(),imageLabel->point.y()),QPoint(avgDotX,avgDotY),minDotWidth,maxDotWidth,QPoint(QPoint(stdDotX,stdDotY)));
    if(cntr1)
        center1 = dataBundle.charCenter;
    else
        center2 = dataBundle.charCenter;
    m_image = dataBundle.image;
}
void Dialog::saveTrainedVariable()
{
    QSettings setting("mySoft","braille");
    setting.beginGroup("trainedVariable");
        setting.setValue("minDotWidth",minDotWidth);
        setting.setValue("maxDotWidth",maxDotWidth);
        setting.setValue("errToFindDot",QPoint(stdDotX,stdDotY));
        setting.setValue("distBetDot",QPoint(avgDotX,avgDotY));
        if(avgChX!=-1 && avgChY!=-1)
        {
            setting.setValue("errToFindChar",QPoint(stdChX,stdChY));
            setting.setValue("distBetChar",QPoint(avgChX,avgChY));
        }
        else
        {
            QPoint point = setting.value("errToFindChar",QPoint(200,200)).toPoint();
                    setting.setValue("errToFindChar",point);
            point = setting.value("distBetChar",QPoint(200,200)).toPoint();
                    setting.setValue("distBetChar",point);
        }

        setting.endGroup();
}
void Dialog::setDotProperty()
{
    if(avgDotX!=-1 && avgDotY!=-1) // if addHorizontal and addVartical clicked
    {
        minDotWidth = std::min(tempMinWidthSlctdCenter1,tempMinWidthSlctdCenter2);
        minDotWidth = std::max(int(minDotWidth*0.5),5);
        maxDotWidth = std::max(tempMaxWidthSlctdCenter1,tempMaxWidthSlctdCenter2);

        maxDotWidth *=1.2;
        //qDebug()<<stdDotX<<stdDotY<<endl;
        stdDotX = std::max(5,stdDotX); //error to find dot
        stdDotY = std::max(5,stdDotY); // erroro to find dot
        //qDebug()<<stdDotX<<stdDotY<<endl;

    }
    else // retrieve previous one we saved
    {
        QSettings setting("mySoft","braille");
        setting.beginGroup("trainedVariable");
            minDotWidth = setting.value("minDotWidth",5).toInt();
            maxDotWidth = setting.value("maxDotWidth",20).toInt();
            QPoint err = setting.value("errToFindDot",QPoint(5,5)).toPoint();
                   stdDotX = err.x(); stdDotY = err.y();
            QPoint dist = setting.value("distBetDot",QPoint(15,15)).toPoint();
                   avgDotX = dist.x(); avgDotY = dist.y();
        setting.endGroup();
    }

}
void Dialog::openBtnHandler()
{
    const QString DEFAULT_DIR_KEY("OPEN_FILE_KEY");
    QSettings setting("mySoft","braille");

    setting.beginGroup("Open_File");
    SelectedFile = QFileDialog::getOpenFileName(
            this, "Select a file", setting.value(DEFAULT_DIR_KEY).toString());
    if (!SelectedFile.isEmpty())
    {
            QDir CurrentDir;
            setting.setValue(DEFAULT_DIR_KEY,CurrentDir.absoluteFilePath(SelectedFile));

            resetAllUI();
            //resetMemberVariable
            placeImageToImageLabel(SelectedFile);
            imageLabel->scaleImage(1);

    }
    setting.endGroup();

}
void Dialog::updateDistance()
{
    int x_dist = abs(center1.x()-center2.x());
    int y_dist = abs(center1.y()-center2.y());
    hDistShower->setText(QString::number(x_dist));
    vDistShower->setText(QString::number(y_dist));
}


