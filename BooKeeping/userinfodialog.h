#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "cvlink.h"

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

private:
    Ui::userInfoDialog *ui;
};

#endif // USERINFODIALOG_H
