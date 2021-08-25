#include "progresswidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <qevent.h>
progressWidget::progressWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Progress Info");
    isProcessing = false;
    isCancelled = false;
    initMemWidget();
    connectWidget();
    layoutWidget();
    setFixedSize(sizeHint());
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void progressWidget::setMaximum(int numberOfLine, int numberOfFile)
{
    individualProgressBar->setMaximum(numberOfLine);
    allProgressBar->setMaximum(numberOfFile*numberOfLine);
    this->numberOfFiles = numberOfFile;
    if(numberOfFile==1) allProgressBar->setVisible(false);
    else allProgressBar->setVisible(true);
    this->numberOfLines = numberOfLine;
    setValue(1,0);
    show();
    activateWindow();
    emit(processingStarted());
}

void progressWidget::setText(const QString &text)
{
    textShowLabel->setText(text);
}

void progressWidget::setValue(int filePos, int linePos)
{
    individualProgressBar->setValue(linePos);
    allProgressBar->setValue((filePos-1)*numberOfLines+linePos);

    if(numberOfFiles == filePos && numberOfLines == linePos)
        emit(processingEnded());

}

void progressWidget::makeCancel()
{
    isCancelled = true;
    emit(processCancelled());
}


progressWidget::~progressWidget()
{
}

void progressWidget::initMemWidget()
{
    individualProgressBar = new QProgressBar;
    allProgressBar = new QProgressBar;
    textShowLabel = new QLabel;
    cancellButton = new QPushButton("Cancel");
}

void progressWidget::connectWidget()
{
    connect(cancellButton,&QPushButton::clicked,[=](){
        makeCancel();
    });
    connect(this,SIGNAL(processCancelled()),this,SLOT(on_endignProcess()));
    connect(this,SIGNAL(processingEnded()),this,SLOT(on_endignProcess()));
    connect(this,SIGNAL(processingStarted()),this,SLOT(on_startingProcess()));
}

void progressWidget::layoutWidget()
{
    cancelLay = new QHBoxLayout;
                cancelLay->addWidget(textShowLabel);
                cancelLay->addStretch();
                cancelLay->addWidget(cancellButton);

    mainLay = new QVBoxLayout;
                mainLay->addWidget(individualProgressBar);
                mainLay->addWidget(allProgressBar);
                mainLay->addLayout(cancelLay);

                setLayout(mainLay);
}

void progressWidget::on_startingProcess()
{
    //qDebug()<<"on starting"<<endl;
    isProcessing = true;
    isCancelled = false;
}

void progressWidget::on_endignProcess() //on_endignProcess triggerd by either cancelled or finished
{
    //qDebug()<<"on endign"<<endl;
    isProcessing = false;
    close(); // this will basically produce a close event

}

void progressWidget::closeEvent(QCloseEvent *event)
{
    if(isProcessing){
        int result = QMessageBox::warning(this, tr("Progress Info"),
         tr("Currently an image being processed.\n"
         "Do you want to cancel?"),
         QMessageBox::Yes ,
         QMessageBox::No| QMessageBox::Default);
         //QMessageBox::Cancel | QMessageBox::Escape);
        if (result == QMessageBox::Yes){
            makeCancel();
        }
        else event->ignore();
    } else {
        event->accept();
    }
}

