#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "global.h"
#include "cvlink.h"


static const int RETCODE_RESTART = 773;
namespace Ui {
class userInfoDialog;
}

class userInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit userInfoDialog(QWidget *parent = 0);
    ~userInfoDialog();


private slots:
    void on_pushButtonConfirm_clicked();


    void on_pushButtonSelect_clicked();

    void on_pushButtonHeadConfirm_clicked();

    void changePassResult(bool result, QString msg);

    void changeHeadResult(bool result);
signals:
    void changeHead();

private:
    Ui::userInfoDialog *ui;
};

#endif // USERINFODIALOG_H
