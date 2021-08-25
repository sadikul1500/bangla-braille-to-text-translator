#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utilfileanddir.h"
#include "utilimageproc.h"
#include <QDebug>

#include <QAbstractItemView>
#include <QDialog>
#include <QPainter>
#include <QPixmap>
#include <QPrintDialog>
#include <QPrinter>
#include <QProgressDialog>
#include <QSettings>
//#include <QSplitter>
#include <QStringListModel>
//#include <QTemporaryFile>
//#include <QThread>
//#include <QTime>
#include <qevent.h>
#include <qfiledialog.h>
#include <qfontdatabase.h>
//#include <bits/stdc++.h>
#include "brailleToText.h"
#include "brailleToBangla.h"
#include"frontPage/frontpage.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    FrontPage *fntpage = new FrontPage(this);
    fntpage->exec();
    setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    setWindowTitle("Braille");
    //readMemberVariable();
    readDefaultMemberVariable();
    createActions();
    initMemWidget();
    connectWidget();
    //ui->statusbar->showMessage("bsse1029@iit.du.ac.bd | bsse1003@iit.du.ac.bd");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_environmenVariableAction_triggered()
{
    if(!shouldProceed()) return;

    Dialog w;
    int i = w.exec();
    if(i==1) readMemberVariable();


}

void MainWindow::on_DebugAction_triggered()
{
    if(!shouldProceed()) return;
    w.show();
}

void MainWindow::readMemberVariable()
{
    QSettings setting("mySoft","braille");
    setting.beginGroup("trainedVariable");
        m_minDotWidth = setting.value("minDotWidth",5).toInt();
        m_maxDotWidth = setting.value("maxDotWidth",27).toInt();
        //qDebug()<<m_minDotWidth<<m_maxDotWidth<<endl;
        m_errDot = setting.value("errToFindDot",QPoint(5,5)).toPoint();
        m_errCh = setting.value("errToFindChar",QPoint(1,1)).toPoint();
        //qDebug()<<"m_errDot: "<<m_errDot<<" m_minDotWidth: "<<m_minDotWidth;
        m_distBetDot = setting.value("distBetDot",QPoint(30,27)).toPoint();
        m_distBetCh = setting.value("distBetChar",QPoint(76,253)).toPoint();
//        qDebug()<<m_minDotWidth<<" "<<m_maxDotWidth<<" "<<m_errDot<<" "<<m_errCh<<" "<<m_distBetDot<<" "<<m_distBetCh;
        setting.endGroup();
}

void MainWindow::readDefaultMemberVariable()
{
    m_minDotWidth = 8;
    m_maxDotWidth = 27;
    m_errDot = QPoint(10,10);

    m_errCh = QPoint(1,1);
    m_distBetDot = QPoint(28,32);
    m_distBetCh = QPoint(76,126);
    //qDebug()<<m_minDotWidth<<" "<<m_maxDotWidth<<" "<<m_errDot<<" "<<m_errCh<<" "<<m_distBetDot<<" "<<m_distBetCh;
    m_minDotWidth = 5;
    m_maxDotWidth = 20;
    m_errDot = QPoint(8,8);
    m_distBetDot = QPoint(25,25);
    m_distBetCh = QPoint(62,95);

}

void MainWindow::createActions()
{
    actionRemoveFileForResult = new QAction("Remove Files");
    actionselectAllForOpen = new QAction("Select All");
    actionselectAllForResult = new QAction("Select All");
    actionSaveFiles = new QAction("Save Files");
    exportAsPdfForResult = new QAction("Export As PDF");
    actionPrint = new QAction("Print");

}

void MainWindow::initMemWidget()
{
    //ImageLabel
        //imageLabel = new ImageLabel(nullptr);
        ui->imageLabel->setBackgroundRole(QPalette::Base);
        ui->imageLabel->setScaledContents(true);
    //ScrollArea
        ui->scrollArea->setBackgroundRole(QPalette::Dark);
        ui->scrollArea->setWidget(ui->imageLabel);
    // openListView
        ui->openListView->addAction(actionselectAllForOpen);
        ui->openListView->addAction(ui->actionRemoveFileForOpen);

        //ui->resultListView = new OListView;
        ui->resultListView->addAction(actionselectAllForResult);
        ui->resultListView->addAction(actionSaveFiles);
        ui->resultListView->addAction(actionRemoveFileForResult);
        ui->resultListView->addAction(exportAsPdfForResult);
        ui->resultListView->addAction(actionPrint);
        //scrollArea->setStyleSheet("background-color: rgb(128,200,200);");
        resultModel = new QStringListModel(this);
        ui->resultListView->setModel(resultModel);
        ui->resultListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // QPushButton
    //ui->convrt->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    //ui->convrtAll->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    //ui->convrt->setStyleSheet("background-color: rgb(100,150,100);");
    //ui->convrtAll->setStyleSheet("background-color: rgb(100,200,100);");
}

void MainWindow::connectWidget()
{
    connect(ui->openListView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onOpenListViewItemClicked()));
    connect(ui->openListView, SIGNAL(upDownKeyPressed()), this, SLOT(onOpenListViewItemClicked()));
    connect(ui->openListView, SIGNAL(deletPressed()), this, SLOT(removeForOpenView()));
    connect(ui->convrtAll,SIGNAL(clicked()),this,SLOT(onCnvrtAllClick()));
    connect(ui->convrt,SIGNAL(clicked()),this,SLOT(onCnvrtClick()));
    connect(ui->resultListView,SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onResultListViewItemClicked()));
    connect(ui->resultListView, SIGNAL(upDownKeyPressed()), this, SLOT(onResultListViewItemClicked()));
    connect(ui->resultListView, SIGNAL(leftKeyPressed()), this, SLOT(ontextBtnClicked()));
    connect(ui->resultListView, SIGNAL(rightKeyPressed()), this, SLOT(onimageBtnClicked()));
    connect(ui->resultListView, SIGNAL(deletPressed()), this, SLOT(removeForResultView()));
    connect(ui->resultListView, SIGNAL(cntrlPlusSPressed()), this, SLOT(on_saveFiles()));
    connect(ui->resultListView, SIGNAL(cntrlPlusAltPlusSPressed()), this, SLOT(on_saveBinFiles()));

    //-------------Action-------------------------------
    connect(ui->actionOpenFiles,SIGNAL(triggered()),this,SLOT(on_openFiles()));
    connect(ui->actionEnvironmenVariable,SIGNAL(triggered()),this,SLOT(on_environmenVariableAction_triggered()));
    //connect(actionIsDebug,&QAction::triggered,[=](){;});
    connect(ui->actionDebug,SIGNAL(triggered()),this,SLOT(on_DebugAction_triggered()));
    connect(ui->actionAddFiles,SIGNAL(triggered()),this,SLOT(on_addFiles()));
    connect(ui->actionRemoveFileForOpen,SIGNAL(triggered()),this,SLOT(removeForOpenView()));
    connect(actionRemoveFileForResult,SIGNAL(triggered()),this,SLOT(removeForResultView()));
    connect(actionselectAllForOpen,&QAction::triggered,[=](){ui->openListView->selectAll();});
    connect(actionselectAllForResult,&QAction::triggered,[=](){ui->resultListView->selectAll();});
    connect(ui->actionQuit,&QAction::triggered,[=](){close();});
    connect(actionSaveFiles,SIGNAL(triggered()),this,SLOT(on_saveFiles()));
    connect(ui->btnSave,SIGNAL(clicked()),this,SLOT(on_saveFiles()));
    connect(exportAsPdfForResult,SIGNAL(triggered()),this,SLOT(on_savePDFFiles()));
    connect(ui->btnPrint,SIGNAL(clicked()),this,SLOT(on_Print()));
    connect(actionPrint,SIGNAL(triggered()),this,SLOT(on_Print()));

}

void MainWindow::showBanglaText()
{
    if(m_textToShow!=nullptr)
    {
           ui->imageLabel->setMargin(50);
           ui->imageLabel->setAlignment(Qt::AlignHCenter);
           ui->imageLabel ->setText(m_textToShow);
           ui->imageLabel->resize(1000,ui->imageLabel->sizeHint().height());
           //ui->imageLabel->resize(1000,3000);
           //qDebug()<<ui->imageLabel->size();
    }
}

void MainWindow::setImage()
{
    ui->imageLabel->setMargin(0);
    ui->imageLabel->setPixmap(QPixmap::fromImage(m_imageToShow));
    ui->imageLabel->scaleImage(1);
    //ui->imageLabel->resize(1000,1000);
    //qDebug()<<ui->imageLabel->size();
}

void MainWindow::onCnvrtClick()
{

    if(!shouldProceed()) return;

    QModelIndexList midList =  ui->openListView->selectedIndexes();
    if(midList.empty()) {
        QMessageBox::information(this, tr("Information"),
         tr("Please select some image first.\n"),
         QMessageBox::Ok);
        return;
    }
    int startIdx= midList.at(0).row();
    int endIdx = startIdx+midList.count()-1;
    convertAndSave(m_openFileList,m_binImageList,startIdx,endIdx);
    ui->openListView->setCurrentIndex(QModelIndex());

}

void MainWindow::onCnvrtAllClick()
{
    if(!shouldProceed()) return;

    if(m_openFileList.empty()) return;
    convertAndSave(m_openFileList,m_binImageList,0,m_openFileList.length()-1);
    ui->openListView->setCurrentIndex(QModelIndex());
}

void MainWindow::removeForOpenView()
{
    QModelIndexList midList =  ui->openListView->selectedIndexes();
    if(midList.empty()) {
        QMessageBox::information(this, tr("Information"),
         tr("Please select some image first.\n"),
         QMessageBox::Ok);
        return;
    }
    int result = QMessageBox::warning(this, tr("Removing"),
     tr("Are you sure to remove %1 items.\n").arg(midList.count()),
     QMessageBox::Yes | QMessageBox::Default,
     QMessageBox::No | QMessageBox::Escape);
    if (result == QMessageBox::No) return;
    for(int i=0;i<midList.count();i++){
        int index = midList.at(i).row();
        if(index>-1){
            m_openFileList.removeAt(index-i);
            m_binImageList.removeAt(index-i);

        }
    }
    updateOpenListView();
    ui->openListView->setCurrentIndex(QModelIndex());
}

void MainWindow::removeForResultView()
{
    if(isSaving) return;
    QModelIndexList midList =  ui->resultListView->selectedIndexes();
    if(midList.empty()) {
        QMessageBox::information(this, tr("Information"),
         tr("Please select some file first.\n"),
         QMessageBox::Ok);
        return;
    }
    int result = QMessageBox::warning(this, tr("Removing"),
     tr("Are you sure to remove %1 item.\n").arg(midList.count()),
     QMessageBox::Yes | QMessageBox::Default,
     QMessageBox::No | QMessageBox::Escape);
    if (result == QMessageBox::No) return;
    for(int i=0;i<midList.count();i++){
        int index = midList.at(i).row();
        if(index>-1){

            _banglaText.removeAt(index-i);
            m_resultImagesList.removeAt(index-i);
            nameList.removeAt(index-i);
            _binText.removeAt(index-i);

        }
    }
    resultModel->setStringList(nameList);
    ui->resultListView->setCurrentIndex(QModelIndex());
}

void MainWindow::ontextBtnClicked()
{
    showBanglaText();
}

void MainWindow::onimageBtnClicked()
{
    setImage();
}

void MainWindow::onOpenListViewItemClicked()
{
    QModelIndex modelIdx = ui->openListView->currentIndex();
    int index = modelIdx.row();
    if(index>-1)
    {
        //textBtn->setStyleSheet("background-color: rgb(112,180,213);");
        //imageBtn->setStyleSheet("background-color: rgb(112,180,213);");
        //textBtn->setDisabled(true);
        //imageBtn->setDisabled(true);
        m_imageToShow = m_binImageList.at(index);
        m_textToShow = nullptr;
        setImage();
    }
}

void MainWindow::onResultListViewItemClicked()
{
    QModelIndex modelIdx = ui->resultListView->currentIndex();
    int index = modelIdx.row();

    if(index>-1)
    {
        m_textToShow = _banglaText.at(index);
        m_imageToShow = m_resultImagesList[index];
        showBanglaText();
    }
}

void MainWindow::on_openFiles() // path not saved !
{

    const QString DEFAULT_DIR_KEY("OPEN_FILE_KEY");
    QSettings setting("mySoft","braille");

    setting.beginGroup("Open_File");
    m_openFileList = QFileDialog::getOpenFileNames(
                             this,
                             "Select one or more files to open",
                             setting.value(DEFAULT_DIR_KEY).toString(),
                             "Images (*.png *.xpm *.jpg *.jpeg)");
    if(m_openFileList.empty()) return;
        QDir CurrentDir;
        setting.setValue(DEFAULT_DIR_KEY,CurrentDir.absoluteFilePath(m_openFileList[0]));
        m_currentImageDir = QFileInfo(m_openFileList[0]).dir();
    setting.endGroup();

    ui->imageLabel->clear();
    m_binImageList.clear();
    mainImageToBinImage(m_openFileList);
    updateOpenListView();
    if(progressWdgt.processing()) return;
    nameList.clear();
    resultModel->setStringList(nameList);
    _banglaText.clear();
    _binText.clear();
    m_resultImagesList.clear();



}

void MainWindow::on_addFiles()
{
    QStringList list;
    const QString DEFAULT_DIR_KEY("OPEN_FILE_KEY");
    QSettings setting("mySoft","braille");

    setting.beginGroup("Open_File");
        list = QFileDialog::getOpenFileNames(
                                 this,
                                 "Select one or more files to open",
                                 setting.value(DEFAULT_DIR_KEY).toString(),
                                 "Images (*.png *.xpm *.jpg)");
        if(list.empty()) return;
        QDir CurrentDir;
        setting.setValue(DEFAULT_DIR_KEY,CurrentDir.absoluteFilePath(list[0]));
    setting.endGroup();
    m_openFileList.append(list);
    updateOpenListView();
    mainImageToBinImage(list);
}

void MainWindow::on_saveFiles()
{
    isSaving = true;
    QModelIndexList midList =  ui->resultListView->selectedIndexes();
    if(midList.empty()) {
        QMessageBox::information(this, tr("Information"),
         tr("Please select some file first.\n"),
         QMessageBox::Ok);
        isSaving = false;
        return;
    }
    QString title;
    binaryFiles ? title = "Open Binary Files Directory" : title = "Open Text Files Directory";
    QString dir = QFileDialog::getExistingDirectory(this, title,
                                                      "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    int startIdx= midList.at(0).row();
    int endIdx = startIdx+midList.count()-1;
//    qDebug()<<startIdx<<" "<<endIdx<<endl;
    QString fullPath;
    for(int i=startIdx;i<=endIdx;i++){
        fullPath = UtilFileAndDir::getFileName(dir,nameList.at(i));
        if(fullPath.isEmpty()) return ; // directory is delected by someone
        QFile file(fullPath);
//        qDebug()<<fullPath<<endl;
        if ( file.open(QIODevice::WriteOnly | QIODevice::Text) ) // overriding every time
        {
            QTextStream stream( &file );
            stream.setCodec("UTF-8");
            if(!binaryFiles) stream<<_banglaText.at(i);
            else stream<<_binText.at(i);
        }
    }
    ui->resultListView->setCurrentIndex(QModelIndex());
    isSaving = false;
}

void MainWindow::on_saveBinFiles()
{
    binaryFiles=true;
    on_saveFiles();
    binaryFiles=false;
}

void MainWindow::on_savePDFFiles()
{
    isSaving = true;
    QModelIndexList midList =  ui->resultListView->selectedIndexes();
    if(midList.empty()) {
        QMessageBox::information(this, tr("Information"),
         tr("Please select some file first.\n"),
         QMessageBox::Ok);
        isSaving = false;
        return;
    }
    //------------------------------------------
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPaperSize(QPrinter::A4);
    int startIdx= midList.at(0).row();
    int endIdx = startIdx+midList.count()-1;
//    qDebug()<<startIdx<<" "<<endIdx<<endl;

    QPainter painter;

    int headerHeight = 100;
    int footerHeight = 40;
    int spacing = 20;
    int bodyHeight = printer.height()-(headerHeight+footerHeight+2*spacing);
    int indent = 30;
    QRect header(0,0,printer.width(),headerHeight);
    QRect body(indent,headerHeight+spacing,printer.width()-indent,bodyHeight);
    QRect footer(0,printer.height()-footerHeight,printer.width(),footerHeight);

//    painter.setPen(QColor(Qt::red));
//    painter.drawRect(header);
//    painter.drawRect(body);
//    painter.drawRect(footer);
    QImage iitLogo(":/logo/logo/iitLogo.png");
    QImage ictLogo(":/logo/logo/ictLogo.png");
    painter.begin(&printer);
    QFontDatabase::addApplicationFont(":/Fonts/Font/kalpurush.ttf");
    QFont font = QFont("Kalpurush", 20, 10);
    painter.setFont(font);
    for(int i=startIdx;i<=endIdx;i++){
        painter.drawImage(QRect(0,0,120,80),iitLogo);
        painter.drawImage(QRect(printer.width()-125,0,120,80),ictLogo);
        painter.drawText(body,Qt::AlignCenter,_banglaText.at(i));
        painter.drawText(footer,Qt::AlignHCenter,QString::number(i-startIdx+1));
        if(i!=endIdx) printer.newPage();
    }
    painter.end();
    ui->resultListView->setCurrentIndex(QModelIndex());
    isSaving = false;
}

void MainWindow::on_Print()
{
    isSaving = true;
    QModelIndexList midList =  ui->resultListView->selectedIndexes();

    if(midList.empty()) {
        QMessageBox::information(this, tr("Information"),
         tr("Please select some file first.\n"),
         QMessageBox::Ok);
        isSaving = false;
        return;
    }
    //------------------------------------------
    QPrinter printer;
    printer.setPaperSize(QPrinter::A4);
    int startIdx= midList.at(0).row();
    int endIdx = startIdx+midList.count()-1;
//    qDebug()<<startIdx<<" "<<endIdx<<endl;
    QPrintDialog printDialog(&printer,0);
    if(printDialog.exec()!=QDialog::Accepted) return;
    QPainter painter;

    int headerHeight = 100;
    int footerHeight = 40;
    int spacing = 20;
    int bodyHeight = printer.height()-(headerHeight+footerHeight+2*spacing);
    int indent = 30;
    QRect header(0,0,printer.width(),headerHeight);
    QRect body(indent,headerHeight+spacing,printer.width()-indent,bodyHeight);
    QRect footer(0,printer.height()-footerHeight,printer.width(),footerHeight);
    QImage iitLogo(":/logo/logo/iitLogo.png");
    QImage ictLogo(":/logo/logo/ictLogo.png");
    painter.begin(&printer);
    QFontDatabase::addApplicationFont(":/Fonts/Font/kalpurush.ttf");
    QFont font = QFont("Kalpurush", 20, 10);
    painter.setFont(font);
    for(int i=startIdx;i<=endIdx;i++){
        painter.drawImage(QRect(0,0,120,80),iitLogo);
        painter.drawImage(QRect(printer.width()-125,0,120,80),ictLogo);
        painter.drawText(body,Qt::AlignCenter,_banglaText.at(i));
        painter.drawText(footer,Qt::AlignHCenter,QString::number(i-startIdx+1));
        if(i!=endIdx) printer.newPage();
    }
    painter.end();
    ui->resultListView->setCurrentIndex(QModelIndex());
    isSaving = false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(progressWdgt.processing()) {
        int result = QMessageBox::warning(this, tr("Quit"),
         tr("Currently an image being processed.\n"
         "Sure to exit?"),
         QMessageBox::Yes | QMessageBox::Default,
         QMessageBox::No);
         if(result == QMessageBox::Yes){
             progressWdgt.makeCancel();
             event->accept();
         }
         else {
             event->ignore();
         }
    } else {
        event->accept();
    }
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
         int key=keyEvent->key();
         int modifires = keyEvent->modifiers();
         //qDebug()<<key<<" "<<modifires;
         if (key == 83 && modifires == 201326592){ //Ctrl + Alt + s
             binaryFiles=true;
             on_saveFiles();
             binaryFiles=false;
        } else if(key == 83 && modifires == 67108864){ //ctrl + s
             on_saveFiles();
         } else if(key == 16777223 && modifires ==0){
             bool v1 = ui->openListView->selectedIndexes().empty();
             bool v2 =ui->resultListView->selectedIndexes().empty();
             if(!v1 && v2) removeForOpenView();
             else if(v1 && !v2) removeForResultView();
         }
    }
    return QMainWindow::event(event);
}

bool MainWindow::shouldProceed()
{
    if(progressWdgt.processing()){
          msgBox.setWindowTitle("Information");
          msgBox.setText("Please cancel current process first..");
          msgBox.show();
        return false;
    }
    return true;

}


void MainWindow::updateOpenListView()
{
    QStringListModel *model = new QStringListModel(this);
    //-----------------
    QStringList nameList;
    for(auto &it:m_openFileList)
    {
        nameList.append(QFileInfo(it).fileName());
    }
    //-------------
    model->setStringList(nameList);
    ui->openListView->setModel(model);
    ui->openListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::loadBanglaText(QString resultTextFile)
{

    if(resultTextFile!=nullptr)
    {
        QFile file(resultTextFile);
        QString line;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {

            line.append("\n\n");
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            while (!stream.atEnd())
            {
                line.append("\t");
                line.append(stream.readLine()+"\n");
            }
            _banglaText.append(line);
            //qDebug()<<line;
        }
        file.close();
    }
}

bool MainWindow::mainImageToBinImage(QStringList &list)
{
     QProgressBar progress(ui->openListView);
     progress.setGeometry(0,ui->openListView->height()/2,ui->openListView->width(),20);
     progress.show();
     progress.setRange(0, list.length());
    for(int i=0; i<list.length();i++)
    {
        //progress.setLabelText(tr("Loading %1").arg(QFileInfo(m_openFileList.at(i)).fileName()));
        /////progress.setText(tr("Loading %1").arg(QFileInfo(m_openFileList.at(i)).fileName()));
        progress.setValue(i);
        m_binImageList.append(UtilImageProc::mainImageToBinImage(list.at(i)));
    }
    return true;
}

void MainWindow::convertAndSave(QStringList m_openFileList,QImageList m_binImageList, int startIdxList, int endIdxList) // i am sure that startindex and end index is valid
{
   //QString resultDir = UtilFileAndDir::makeDirec(m_currentImageDir,"result");
   //QString binDir = UtilFileAndDir::makeDirec(resultDir,"binDataFiles"); //sub directory of result
   QImage image;
   int numberOfLineExpected = 27;//row/m_distBetCh.y();
   progressWdgt.setMaximum(numberOfLineExpected,endIdxList-startIdxList+1);

   UtilImageProc::setBrailleProperty(m_distBetDot,m_distBetCh,m_minDotWidth, m_maxDotWidth,m_errDot,m_errCh,false,ui->actionIsDebug->isChecked());
   for(int index=startIdxList;index<=endIdxList;index++){

       QStringList binData;
       progressWdgt.setText("Processing: "+QFileInfo(m_openFileList.at(index)).fileName());
       image = m_binImageList.at(index);
       QPoint strtingPt = QPoint(0,0);

       for(int i=1;i<=numberOfLineExpected;i++){
           UtilImageProc::findingChar = true;
           DataBundle dataBundle = UtilImageProc::findChar(image, strtingPt);
           UtilImageProc::findingChar = false;

           if(progressWdgt.cancelled()) goto endFunc;
           strtingPt = QPoint(strtingPt.x(),dataBundle.charCenter.y()+m_distBetCh.y()-m_distBetDot.y()-5);
            //qDebug()<<strtingPt.y()<<" "<<dataBundle.charCenter<<" "<<dataBundle.isValidDot<<endl;
           if(dataBundle.isValidDot){
               dataBundle = UtilImageProc::readLine(dataBundle.image,dataBundle.charCenter);
               if(progressWdgt.cancelled()) goto endFunc;
               if(!dataBundle.charList.empty()){
                   binData.append(dataBundle.charList);
                   binData.append("\n");
               }
               image = dataBundle.image;
           }
           else{
               image = dataBundle.image;
               progressWdgt.setValue(index-startIdxList+1,numberOfLineExpected);
               break;
           }

           progressWdgt.setValue(index-startIdxList+1,i);
       }

       if(progressWdgt.cancelled()) goto endFunc;

       bool savedFile = saveBinBrille(QFileInfo(m_openFileList.at(index)).fileName(),binData);
       if(savedFile){
           nameList.append(QFileInfo(m_openFileList.at(index)).fileName());
           resultModel->setStringList(nameList);
           m_resultImagesList.append(image);

       }

   }
endFunc:;
   //QDir(resultDir).removeRecursively();
}

bool MainWindow::saveBinBrille(QString saveFileName,QStringList binData)
{

    QRegExp rx("([a-zA-Z0-9_ ])+");
    if(rx.indexIn(saveFileName)!=-1)
        saveFileName = rx.capturedTexts().at(0);
    saveFileName.append(".txt");
    //saveFileName = dir+"/"+saveFileName;
    //QString tempFile=dir+"/"+"binDataFiles"+"/"+saveFileName;
    QString tempFile="bin.txt";
    QFile file( tempFile );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text) ) // overriding every time
    {
        QTextStream stream( &file );
        for(auto &it:binData)
        {
            stream <<it;
            if(it!='\n') stream<<" ";
        }

    }

    file.close();
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        _binText.append(file.readAll());

 // generete bangla by sadi from tempfile and save that by savefilename
///*    //toggle this to save file when crash
    BrailleToText *brailleToText;
    BrailleToBangla brailleToBangla;
    brailleToText = &brailleToBangla;
    string inputFile = tempFile.toStdString();
    string outText = brailleToText->getText(inputFile, brailleToText);

    _banglaText.append(QString::fromStdString(outText));
//*/
    return true; // true if successfully saved the file
}

