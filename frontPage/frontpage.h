#ifndef FRONTPAGE_H
#define FRONTPAGE_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class FrontPage; }
QT_END_NAMESPACE

class FrontPage : public QDialog
{
    Q_OBJECT

public:
    FrontPage(QWidget *parent = nullptr);
    ~FrontPage();
protected:
    void closeEvent(QCloseEvent *) override;
private:
    Ui::FrontPage *ui;

};
#endif // FRONTPAGE_H
