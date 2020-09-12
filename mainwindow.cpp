
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilfileanddir.h"
#include "utilimageproc.h"
#include <QDebug>

#include <QAbstractItemView>
#include <QProgressDialog>
#include <QSettings>
#include <QSplitter>
#include <QStringListModel>
#include <QThread>
#include <QTime>
#include <qevent.h>
#include <qfiledialog.h>
#include <bits/stdc++.h>
#include "brailleToText.h"
#include "brailleToBangla.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
//    readMemberVariable();
    readDefaultMemberVariable();
    createActions();
    createMenu();
    initMemWidget();
    connectWidget();
    layoutWidget();
    setWindowTitle("Braille");
    this->setStyleSheet("background-color: rgb(112,180,213);");
    this->menuBar()->setStyleSheet("background-color: rgb(200,200,200)");
    setTabOrder(openListView,resultListView);

}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::on_environmenVariableAction_triggered()
{
    if(!shouldProceed()) return;

    Dialog w;
    int i = w.exec();
    if(i==1) readMemberVariable();


}

void MainWindow::readMemberVariable()
{
    QSettings setting("mySoft","braille");
    setting.beginGroup("trainedVariable");
        m_minDotWidth = setting.value("minDotWidth",5).toInt();
        m_maxDotWidth = setting.value("maxDotWidth",27).toInt();
//        qDebug()<<m_minDotWidth<<m_maxDotWidth<<endl;
        m_errDot = setting.value("errToFindDot",QPoint(3,4)).toPoint();
        m_errCh = setting.value("errToFindChar",QPoint(1,1)).toPoint();
        m_distBetDot = setting.value("distBetDot",QPoint(30,27)).toPoint();
        m_distBetCh = setting.value("distBetChar",QPoint(76,253)).toPoint();
//        qDebug()<<m_minDotWidth<<" "<<m_maxDotWidth<<" "<<m_errDot<<" "<<m_errCh<<" "<<m_distBetDot<<" "<<m_distBetCh;
        setting.endGroup();
}

void MainWindow::readDefaultMemberVariable()
{
    m_minDotWidth = 5;
    m_maxDotWidth = 27;
    m_errDot = 2*QPoint(3,4);
    m_errCh = QPoint(1,1);
    m_distBetDot = QPoint(30,27);
    m_distBetCh = QPoint(76,253);
    //qDebug()<<m_minDotWidth<<" "<<m_maxDotWidth<<" "<<m_errDot<<" "<<m_errCh<<" "<<m_distBetDot<<" "<<m_distBetCh;
}

void MainWindow::createMenu()
{
    menuFiles =  menuBar()->addMenu("Files");
        menuFiles->addAction(actionOpenFiles);
        menuFiles->addAction(actionAddFiles);
        menuFiles->addAction(actionRemoveFileForOpen);
        menuFiles->addAction(actionQuit);
    menuSettings =  menuBar()->addMenu("Settings");
        menuSettings->addAction(actionEnvironmenVariable);
    menuHelp =  menuBar()->addMenu("Help");
        menuHelp->addAction(actionContactUs);

}

void MainWindow::createActions()
{
    actionOpenFiles = new QAction("Open Images");
        actionOpenFiles->setShortcut(tr("Ctrl+O"));
    actionAddFiles =  new QAction("Add Images");
    actionRemoveFileForOpen =  new QAction("Remove Files");
        //actionRemoveFileForOpen->setShortcut(tr("Delete"));
    actionRemoveFileForResult = new QAction("Remove Files");
        //actionRemoveFileForResult->setShortcut(tr("Delete"));
    actionEnvironmenVariable = new QAction("Environment Variable");
    actionContactUs = new QAction("Contact Us");
    actionselectAllForOpen = new QAction("Select All");
        //actionselectAllForOpen->setShortcut(tr("Ctrl + A"));
    actionselectAllForResult = new QAction("Select All");
      //actionselectAllForResult->setShortcut(tr("Ctrl + A"));
    actionQuit = new QAction("Quit");
    actionSaveFiles = new QAction("Save Files");
        //actionSaveFiles->setShortcut(tr("Ctrl + S"));
}

void MainWindow::initMemWidget()
{
    //ImageLabel
        imageLabel = new ImageLabel;
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);
    //ScrollArea
        scrollArea = new QScrollArea(this);
        scrollArea->setBackgroundRole(QPalette::Dark);
    // openListView
        openListView = new OListView;
        openListView->addAction(actionselectAllForOpen);
        openListView->addAction(actionRemoveFileForOpen);

        resultListView = new OListView;
        resultListView->addAction(actionselectAllForResult);
        resultListView->addAction(actionSaveFiles);
        resultListView->addAction(actionRemoveFileForResult);
        imageLabel->setStyleSheet("background-color: rgb(128,200,200);");
        scrollArea->setStyleSheet("background-color: rgb(128,200,200);");
        openListView->setStyleSheet("background-color: rgb(180,200,210);");
        resultListView->setStyleSheet("background-color: rgb(180,200,210);");
        resultModel = new QStringListModel(this);
        resultListView->setModel(resultModel);
        resultListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // QPushButton
    convrt = new QPushButton("Convert");
    convrtAll = new QPushButton("Convert All");
    convrt->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    convrtAll->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    convrt->setStyleSheet("background-color: rgb(100,150,100);");
    convrtAll->setStyleSheet("background-color: rgb(100,200,100);");
}

void MainWindow::connectWidget()
{
    connect(openListView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onOpenListViewItemClicked()));
    connect(openListView, SIGNAL(upDownKeyPressed()), this, SLOT(onOpenListViewItemClicked()));
    connect(openListView, SIGNAL(deletPressed()), this, SLOT(removeForOpenView()));
    connect(convrtAll,SIGNAL(clicked()),this,SLOT(onCnvrtAllClick()));
    connect(convrt,SIGNAL(clicked()),this,SLOT(onCnvrtClick()));
    connect(resultListView,SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onResultListViewItemClicked()));
    connect(resultListView, SIGNAL(upDownKeyPressed()), this, SLOT(onResultListViewItemClicked()));
    connect(resultListView, SIGNAL(leftKeyPressed()), this, SLOT(ontextBtnClicked()));
    connect(resultListView, SIGNAL(rightKeyPressed()), this, SLOT(onimageBtnClicked()));
    connect(resultListView, SIGNAL(deletPressed()), this, SLOT(removeForResultView()));
    connect(resultListView, SIGNAL(cntrlPlusSPressed()), this, SLOT(on_saveFiles()));
    connect(resultListView, SIGNAL(cntrlPlusAltPlusSPressed()), this, SLOT(on_saveBinFiles()));

    //-------------Action-------------------------------
    connect(actionOpenFiles,SIGNAL(triggered()),this,SLOT(on_openFiles()));
    connect(actionEnvironmenVariable,SIGNAL(triggered()),this,SLOT(on_environmenVariableAction_triggered()));
    connect(actionAddFiles,SIGNAL(triggered()),this,SLOT(on_addFiles()));
    connect(actionRemoveFileForOpen,SIGNAL(triggered()),this,SLOT(removeForOpenView()));
    connect(actionRemoveFileForResult,SIGNAL(triggered()),this,SLOT(removeForResultView()));
    connect(actionselectAllForOpen,&QAction::triggered,[=](){openListView->selectAll();});
    connect(actionselectAllForResult,&QAction::triggered,[=](){resultListView->selectAll();});
    connect(actionQuit,&QAction::triggered,[=](){close();});
    connect(actionSaveFiles,SIGNAL(triggered()),this,SLOT(on_saveFiles()));

}

void MainWindow::layoutWidget()
{

    //------------------left---------------------

    QHBoxLayout *convrtLay = new QHBoxLayout;
                 convrtLay->addWidget(convrt);
                 convrtLay->addWidget(convrtAll);
                 convrtLay->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));
    QVBoxLayout *leftLay = new QVBoxLayout;
                 leftLay->addWidget(openListView);
                 leftLay->addLayout(convrtLay);

    //-------------------middle---------------
    scrollArea->setWidget(imageLabel);

    //QHBoxLayout *textImageBtnLay = new QHBoxLayout;
                //textImageBtnLay->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));
                //textImageBtnLay->addWidget(//textBtn);
                //textImageBtnLay->addWidget(//imageBtn);
    QVBoxLayout *middleLay = new QVBoxLayout;
                 middleLay->addWidget(scrollArea);
                 //middleLay->addLayout(textImageBtnLay);
    //--------------------right---------------
    //--------------------upper---------------
    QHBoxLayout *upperLay = new QHBoxLayout;
                 upperLay->addLayout(leftLay,1);
                 upperLay->addLayout(middleLay,7);
                 upperLay->addWidget(resultListView,1);

    // ---------------belowLay---------------------

    //-------------mainlay------------------------
    QVBoxLayout *mainLay = new QVBoxLayout;
                 mainLay->addLayout(upperLay);
    QWidget *widget = new QWidget;
            widget->setLayout(mainLay);
    setCentralWidget(widget);
}

void MainWindow::showBanglaText()
{
    if(m_textToShow!=nullptr)
    {
           imageLabel->setMargin(50);
           imageLabel->setAlignment(Qt::AlignTop);
           imageLabel ->setText(m_textToShow);
           imageLabel->resize(700,700);
    }
}

void MainWindow::setImage()
{
    imageLabel->setMargin(0);
    imageLabel->setPixmap(QPixmap::fromImage(m_imageToShow));
    imageLabel->scaleImage(1);
}

void MainWindow::onCnvrtClick()
{

    if(!shouldProceed()) return;

    QModelIndexList midList =  openListView->selectedIndexes();
    if(midList.empty()) return;
    int startIdx= midList.at(0).row();
    int endIdx = startIdx+midList.count()-1;
    convertAndSave(m_openFileList,m_binImageList,startIdx,endIdx);
    openListView->setCurrentIndex(QModelIndex());
    
}

void MainWindow::onCnvrtAllClick()
{
    if(!shouldProceed()) return;

    if(m_openFileList.empty()) return;
    convertAndSave(m_openFileList,m_binImageList,0,m_openFileList.length()-1);
    openListView->setCurrentIndex(QModelIndex());
}

void MainWindow::removeForOpenView()
{
    QModelIndexList midList =  openListView->selectedIndexes();
    if(midList.empty()) return;
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
    openListView->setCurrentIndex(QModelIndex());
}

void MainWindow::removeForResultView()
{
    if(isSaving) return;
    QModelIndexList midList =  resultListView->selectedIndexes();
    if(midList.empty()) return;
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
    resultListView->setCurrentIndex(QModelIndex());
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
    QModelIndex modelIdx = openListView->currentIndex();
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
    QModelIndex modelIdx = resultListView->currentIndex();
    int index = modelIdx.row();

    if(index>-1)
    {
        //textBtn->setStyleSheet("background-color: rgb(255,200,150);");
        //imageBtn->setStyleSheet("background-color: rgb(150,255,200);");
        //textBtn->setDisabled(false);
        //imageBtn->setDisabled(false);
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
                             "Images (*.png *.xpm *.jpg)");
    if(m_openFileList.empty()) return;
        QDir CurrentDir;
        setting.setValue(DEFAULT_DIR_KEY,CurrentDir.absoluteFilePath(m_openFileList[0]));
        m_currentImageDir = QFileInfo(m_openFileList[0]).dir();
    setting.endGroup();

    imageLabel->clear();
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
    QModelIndexList midList =  resultListView->selectedIndexes();
    if(midList.empty()) {isSaving = false; return;}
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
    resultListView->setCurrentIndex(QModelIndex());
    isSaving = false;
}

void MainWindow::on_saveBinFiles()
{
    binaryFiles=true;
    on_saveFiles();
    binaryFiles=false;
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
/*
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
             bool v1 = openListView->selectedIndexes().empty();
             bool v2 =resultListView->selectedIndexes().empty();
             if(!v1 && v2) removeForOpenView();
             else if(v1 && !v2) removeForResultView();
         }
    }
    return QMainWindow::event(event);
}
*/
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
    openListView->setModel(model);
    openListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
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
     QProgressBar progress(openListView);
     progress.setGeometry(0,openListView->height()/2,openListView->width(),20);
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
   QString resultDir = UtilFileAndDir::makeDirec(m_currentImageDir,"result");
   QString binDir = UtilFileAndDir::makeDirec(resultDir,"binDataFiles"); //sub directory of result
   QImage image;
   int numberOfLineExpected = 27;//row/m_distBetCh.y();
   progressWdgt.setMaximum(numberOfLineExpected,endIdxList-startIdxList+1);
   UtilImageProc::setBrailleProperty(m_distBetDot,m_distBetCh,m_minDotWidth, m_maxDotWidth,m_errDot,m_errCh);
   for(int index=startIdxList;index<=endIdxList;index++){

       QStringList binData;
       progressWdgt.setText("Processing: "+QFileInfo(m_openFileList.at(index)).fileName());
       image = m_binImageList.at(index);
       QPoint strtingPt = QPoint(20,20);

       //QTime global;
       //global.start();
       for(int i=1;i<=numberOfLineExpected;i++){

           //QTime time;
           //time.start();
           DataBundle dataBundle = UtilImageProc::findChar(image, strtingPt);
           //cout<<time.elapsed()/1000.0<<" Find "<<i<<endl;
           if(progressWdgt.cancelled()) goto endFunc;
           strtingPt = QPoint(20,dataBundle.charCenter.y()+m_distBetCh.y()-m_distBetDot.y()-2);
           if(dataBundle.isValidDot){
               //QTime time;
               //time.start();
               dataBundle = UtilImageProc::readLine(image,dataBundle.charCenter);
               if(progressWdgt.cancelled()) goto endFunc;
               //cout<<time.elapsed()/1000.0<<" ReadLine "<<i<<endl;
               if(!dataBundle.charList.empty()){
                   binData.append(dataBundle.charList);
                   binData.append("\n");
               }
               image = dataBundle.image;
           }
           else{
               progressWdgt.setValue(index-startIdxList+1,numberOfLineExpected);
               break;
           }

           progressWdgt.setValue(index-startIdxList+1,i);
       }
      //cout<<global.elapsed()/1000.0<<" Global "<<endl;
       if(progressWdgt.cancelled()) goto endFunc;
       bool savedFile = saveBinBrille(resultDir,QFileInfo(m_openFileList.at(index)).fileName(),binData);
       if(savedFile){
           nameList.append(QFileInfo(m_openFileList.at(index)).fileName());
           resultModel->setStringList(nameList);
           m_resultImagesList.append(image);

       }

   }
endFunc:
   QDir(resultDir).removeRecursively();
}

bool MainWindow::saveBinBrille(QString dir, QString saveFileName,QStringList binData)
{

    QRegExp rx("([a-zA-Z0-9_ ])+");
    if(rx.indexIn(saveFileName)!=-1)
        saveFileName = rx.capturedTexts().at(0);
    saveFileName.append(".txt");

    //QString tempFile=dir+"/"+"Data.txt";
    QString tempFile=dir+"/"+"binDataFiles"+"/"+saveFileName;
    saveFileName = dir+"/"+saveFileName;
    //qDebug()<<saveFileName<<" "<<tempFile;
    QFile file( tempFile );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Text) ) // overriding every time
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

    BrailleToText *brailleToText;
    BrailleToBangla brailleToBangla;
    brailleToText = &brailleToBangla;
    string inputFile = tempFile.toStdString();
    string outText = brailleToText->getText(inputFile, brailleToText);
    ofstream oFile(saveFileName.toStdString());
    oFile<<outText;
    oFile.close();
    //m_resultFileList.append(saveFileName);
    //loadBanglaText(saveFileName); //image12 is not working
    _banglaText.append(QString::fromStdString(outText));

    return true; // true if successfully saved the file
}
