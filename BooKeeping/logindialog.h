#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QDebug>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QNetworkCookie>
#include <QJsonParseError>
#include <QGraphicsDropShadowEffect>
#include "cvlink.h"
#include "mainwindow.h"
#include "global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
signals:
    void login();

private slots:
    void on_pushButtonLogin_clicked();

    void on_pushButtonClose_clicked();

    void imelResult(bool result, QString msg);

    void passwordLogin(bool result, QString msg);
private:
    Ui::LoginDialog *ui;
    QNetworkAccessManager *manager;
    QPoint relativePos;
    QString loginFlag;
    QString UUID;
    QString userName;
    QString passWord;
    showLoginMenu();

};

#endif // LOGINDIALOG_H
