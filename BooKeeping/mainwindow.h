#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QDebug>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QJsonParseError>
#include <QResizeEvent>
#include <QSystemTrayIcon>
#include <QEvent>
#include "cvlink.h"
#include "logindialog.h"
#include "userinfodialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    bool postHttpRequest(QUrl url, QMap<QString,QString> argMap);
    void initUser();
    void initHead();
    QString roomId;
    QString room;
    QAction *reSetAction;
    QAction *quitAction;
    QMenu *trayMenu;
    QMap<int, QString> userMap;
    int backId;
    qint8 globalNum;
    QString pageSize;
    bool addFlag = true;
    int mHeight;
    int mWidght;
    userInfoDialog *userInfoSetup;
    initMenu();

private slots:
    void bookKeeping();
    void outBookKeeping();
    void initOrg(QMap<int, QString> userMap);
    void on_pushButtonBook_clicked();
    void historyKeeping();
    void on_pushButtonFirst_clicked();
    void on_pushButtonPre_clicked();
    void on_pushButtonNext_clicked();
    void on_pushButtonLast_clicked();
    void showMenu();
    void closeMenu();
    void queryUsers(QMap<int,QString> map, QString roomId, QString room);
    void queryBookKeeping(QJsonArray dataArrary);
    void bookResult(bool bookResult);
    void lastLoginTime(qint64 time);
    void userInfoSetupShow();
};

#endif // MAINWINDOW_H
