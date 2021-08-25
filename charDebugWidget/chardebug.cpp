#include "chardebug.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QSettings>
#include <utilimageproc.h>
#include <QDebug>

CharDebug::CharDebug(QWidget *parent)
    : QWidget(parent)
{
    initComponent();
    createUI();
    connectAction();
    readVariables();
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void CharDebug::initComponent()
{
    //ImageLabel
        imageLabel = new ImageLabel;
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);

        openBtn = new QPushButton("Choose");
            openBtn->setFixedSize(QSize(133,25));

        rgbShower = new QLineEdit;
                    rgbShower->setDisabled(true);
        pxPosShower = new QLineEdit();
                   pxPosShower->setDisabled(true);
        distDotShower = new QLabel();
                   distDotShower->setDisabled(true);
        distCharShower = new QLabel();
                   distCharShower->setDisabled(true);
        minMaxShower = new QLabel();
                   minMaxShower->setDisabled(true);

}

void CharDebug::createUI()
{
    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    QHBoxLayout *upper = new QHBoxLayout();
                 upper->addWidget(openBtn);
                 upper->addWidget(distDotShower);
                 upper->addWidget(distCharShower);
                 upper->addWidget(minMaxShower);

    QHBoxLayout *below = new QHBoxLayout();
                below->addWidget(new QLabel("PixelPos:"));
                below->addWidget(pxPosShower);
                below->addWidget(new QLabel("Pixel: "));
                below->addWidget(rgbShower);
                below->addSpacerItem(new QSpacerItem(5,5,QSizePolicy::Expanding));
    QVBoxLayout *lay2 = new QVBoxLayout(this);
                lay2->addLayout(upper);
                lay2->addWidget(scrollArea);
                lay2->addLayout(below);

    resize(600,600);

}

void CharDebug::connectAction()
{
    connect(imageLabel,SIGNAL(imagePixel()),this,SLOT(setPosAndPix()));
    connect(imageLabel,SIGNAL(center1()),this,SLOT(ctrlPlusClick()));
    connect(imageLabel,SIGNAL(center2()),this,SLOT(altPlusClick()));
    connect(openBtn,SIGNAL(clicked()),this,SLOT(openBtnHandler()));
}

void CharDebug::readVariables()
{
///*
    m_minDotWidth = 5;
    m_maxDotWidth = 27;
    m_errDot = QPoint(10,10);
    m_errCh = QPoint(1,1);
    m_distBetDot = QPoint(28,32);
    m_distBetCh = QPoint(76,126);


//*/
/*
    m_minDotWidth = 5;
    m_maxDotWidth = 27;
    m_errDot = QPoint(9,9);
    m_errCh = QPoint(1,1);
    m_distBetDot = QPoint(31,31);
    m_distBetCh = QPoint(76,126);
//*/
        distDotShower->setText(QString("distDot: (%1,%2)").arg(m_distBetDot.x()).arg(m_distBetDot.y()));
        distCharShower->setText(QString("distChar: (%1,%2)").arg(m_distBetCh.x()).arg(m_distBetCh.y()));
        minMaxShower->setText(QString("minMax: (%1,%2)").arg(m_minDotWidth).arg(m_maxDotWidth));
}

CharDebug::~CharDebug()
{
    //qDebug()<<"destru";
}

void CharDebug::setPosAndPix()
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

void CharDebug::ctrlPlusClick() // click upleft or upright black dot
{
    if(m_image.pixel(imageLabel->point.x(),imageLabel->point.y())!=qRgb(0,0,0))
        return;
    UtilImageProc::setBrailleProperty(m_distBetDot,m_distBetCh,m_minDotWidth, m_maxDotWidth,m_errDot,m_errCh,true,true);
    UtilImageProc::findingChar = true;
    DataBundle db =  UtilImageProc::extractChar(m_image,QPoint(imageLabel->point.x(),imageLabel->point.y()));
    //DataBundle db =  UtilImageProc::findChar(m_image,QPoint(imageLabel->point.x(),imageLabel->point.y()));
    UtilImageProc::findingChar = false;
    imageLabel->setPixmap(QPixmap::fromImage(db.image));

}

void CharDebug::altPlusClick() // click center
{
    UtilImageProc::setBrailleProperty(m_distBetDot,m_distBetCh,m_minDotWidth, m_maxDotWidth,m_errDot,m_errCh,true,true);
    DataBundle db =  UtilImageProc::getBrailleChPosCenter(m_image,QPoint(imageLabel->point.x(),imageLabel->point.y()));
    imageLabel->setPixmap(QPixmap::fromImage(db.image));
}

void CharDebug::openBtnHandler()
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

            //resetMemberVariable
            m_image = UtilImageProc::mainImageToBinImage(SelectedFile);
            imageLabel->setPixmap(QPixmap::fromImage(m_image));
            imageLabel->scaleImage(1);

    }
    setting.endGroup();
}

