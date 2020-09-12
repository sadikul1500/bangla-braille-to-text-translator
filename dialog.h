#ifndef DIALOG_H
#define DIALOG_H
#include "imagelabel.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void adjustScrollBar(double factor);
    void updateDistance();
    void resetAllUI();
    void placeImageToImageLabel(QString string);
    void processDot(bool cntr1);
    void processChar(bool cntr1);
    void saveTrainedVariable();
    void setDotProperty();
public slots:
    void openBtnHandler();
    void setCenter1();
    void setCenter2();
    void setPosAndPix();
    void addItemToVComboBox();
    void addItemToHComboBox();
    QPoint getAvgStd(QComboBox *comboBox);
    void nextBtnHandler();



private:

    Ui::Dialog *ui;
    // --------------Qimage------
    QImage m_image;
    //----QComboBox-----------
    QComboBox *horizontalComboBox,*varticalComboBox;

    // ----QCheckBox---declaration-------------
    QCheckBox *dotDistCheckBox,*charDistCheckBox;
    //----QLineEdit---------------
    QLineEdit *hDistShower,*vDistShower,*seltdCenterXShower,*seltdCenterYShower,*rgbShower,*pxPosShower,*avgStdXShower,*avgStdYShower;
    // ----QPushButton-----declaration------
    QPushButton *workingOnBtn,*nextBtn,*cancelBtn,*okBtn,*zoomInBtn,*zoomOutBtn,*addHBtn,*addVBtn,*openBtn;
    //ImageLabel
    ImageLabel *imageLabel;
    //QScrollArea
    QScrollArea *scrollArea;

    //QPoint
    QPoint center1,center2;
    // variable
    int avgDotX=-1,avgDotY=-1,stdDotX=-1,stdDotY=-1,avgChX=-1,avgChY=-1,stdChX=-1,stdChY=-1;


    //----QLayout------
    QVBoxLayout *openSeltdXSeltdY;
    QHBoxLayout *okCanceLay;

    //-----QRadioButton-----
    QRadioButton *dotPosLeft,*dotPosRight;

    // after nextButton clicked
    bool m_isNextButtonClked = false;
    QString SelectedFile;
    // dot property
    int minDotWidth = 999999,tempMinWidthSlctdCenter1=999999,tempMinWidthSlctdCenter2=999999;
    int maxDotWidth = -1,tempMaxWidthSlctdCenter1 = -1,tempMaxWidthSlctdCenter2 = -1;
public:
    void initComponent();
    void createUI();

};

#endif // DIALOG_H
