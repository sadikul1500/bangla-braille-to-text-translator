#ifndef CHARDEBUG_H
#define CHARDEBUG_H

#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>
#include "../imagelabel.h"
class CharDebug : public QWidget
{
    Q_OBJECT
private:

public:
    CharDebug(QWidget *parent = nullptr);
    void initComponent();
    void createUI();
    void connectAction();
    void readVariables();
    ~CharDebug();
public slots:
    void setPosAndPix();
    void ctrlPlusClick();
    void altPlusClick();
    void openBtnHandler();

private:
    //ImageLabel
    ImageLabel *imageLabel;
    //  QPushButton
    QPushButton *openBtn;
    QImage m_image;
    QLineEdit *rgbShower,*pxPosShower;
    QLabel *distDotShower,*distCharShower,*minMaxShower;
    //QScrollArea
    QScrollArea *scrollArea;
    QString SelectedFile;
    // variables
    QPoint m_errDot,m_errCh,m_distBetDot,m_distBetCh;
    int m_minDotWidth,m_maxDotWidth;
};
#endif // CHARDEBUG_H
