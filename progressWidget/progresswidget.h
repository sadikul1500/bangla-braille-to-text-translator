#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
class QProgressBar;
class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class progressWidget : public QWidget
{
    Q_OBJECT

public:
    progressWidget(QWidget *parent = nullptr);
    void setMaximum(int numberOfLine,int numberOfFile);
    void setText(const QString &text);
    void setValue(int filePos,int linePos);
    bool processing(){ return isProcessing;}
    bool cancelled(){ return  isCancelled;}
    void makeCancel();
    QSize sizeHint() const { return QSize(500,QWidget::sizeHint().height());}
    ~progressWidget();
signals:
    void processingStarted();
    void processingEnded();
    void processCancelled();
private:
    void initMemWidget();
    void connectWidget();
    void layoutWidget();
private slots:
    void on_startingProcess();
    void on_endignProcess();
protected:
    void closeEvent(QCloseEvent *event);
private:
    QProgressBar *individualProgressBar,*allProgressBar;
    QLabel *textShowLabel;
    QPushButton *cancellButton;
    QHBoxLayout *cancelLay;
    QVBoxLayout *mainLay;
    bool isProcessing;
    bool isCancelled;
    int numberOfFiles,numberOfLines;

};
#endif // PROGRESSWIDGET_H
