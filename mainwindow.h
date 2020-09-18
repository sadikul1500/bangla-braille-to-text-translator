#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dialog.h"
#include <QListView>
#include <QProgressBar>
#include <QMainWindow>
#include <QDir>
#include "olistview.h"
#include <qstringlistmodel.h>
#include <qmessagebox.h>
#include "progressWidget/progresswidget.h"
namespace Ui {
class MainWindow;
}

typedef QList<QImage> QImageList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readMemberVariable();
    void readDefaultMemberVariable();
    void initMemWidget();
    void connectWidget();
    void layoutWidget();
    void showBanglaText();
    void setImage();
    void updateOpenListView();
    void loadBanglaText(QString resultTextFile);
    bool mainImageToBinImage(QStringList &list);
    void convertAndSave(QStringList m_openFileList,QImageList m_binImageList,int startIdxList,int endIdxList);
    bool saveBinBrille(QString dir,QString name,QStringList binData);
private slots:
    void on_environmenVariableAction_triggered();
    void onCnvrtClick();
    void onCnvrtAllClick();
    void removeForOpenView();
    void removeForResultView();
    void ontextBtnClicked();
    void onimageBtnClicked();
    void onOpenListViewItemClicked();
    void onResultListViewItemClicked();
    void on_openFiles();
    void on_addFiles();
    void on_saveFiles();
    void on_saveBinFiles();
    void on_savePDFFiles();
    void on_Print();

protected:
 void closeEvent(QCloseEvent *event);
 //bool event(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    bool shouldProceed();
    void createMenu();
    void createActions();
public:

    // imageLabel
    ImageLabel *imageLabel;
    // scrollArea
    QScrollArea *scrollArea;
    // input position
    QPoint inputPt;
    // input box
    QLineEdit *inputBox;
    // QListOpenListView
    OListView *openListView,*resultListView;
    progressWidget progressWdgt;
    // QPushButton
    QPushButton *convrt,*convrtAll;
    // QLabel
    QLabel *brailleTextLabel,*brailleImageLabel;
    // ------memberVariable-----------
    QStringList m_openFileList,m_resultFileList,nameList;
    QList<QString> _banglaText,_binText;
    QImage m_imageToShow;
    QImageList m_resultImagesList,m_binImageList;
    QString m_textToShow;
    QPoint m_errDot,m_errCh,m_distBetDot,m_distBetCh;
    int m_minDotWidth,m_maxDotWidth;
    QDir m_currentImageDir;
    bool isSaving = false,binaryFiles=false;
    //------------------model-------------------------
    QStringListModel *resultModel = nullptr;
    QMessageBox msgBox;
    //----------------menu--------------
    QMenu *menuFiles,*menuSettings,*menuHelp;
    //-----------------actions--------------Files--
    QAction *actionOpenFiles,*actionAddFiles,*actionRemoveFileForOpen,*actionQuit;
    //-----------------actions--------------Settings--
    QAction *actionEnvironmenVariable;
    //-----------------actions--------------Help--
    QAction *actionContactUs;
    // --------------action for context menu------------
    QAction *actionselectAllForOpen,*actionselectAllForResult,*actionRemoveFileForResult,*actionSaveFiles;
    QAction *exportAsPdfForResult,*actionPrint;

};

#endif // MAINWINDOW_H
