#ifndef CVLINK_H
#define CVLINK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>
#include <QApplication>
#include "global.h"


class Cvlink : public QObject
{
    Q_OBJECT

public:
    Cvlink();
    setUrl(QString url);
    setJson(QJsonObject data);
    post();
    void postHttpRequest(QString url, QMap<QString,QString> argMap);
private:
    QString postUrl;
    QNetworkRequest request;
    QByteArray data_json;
    QNetworkAccessManager *manager;
    QMap<int, QString> userMap;
    void loginResult(QJsonObject obj);
    void imelLoginResult(QJsonObject obj);
    void loginLogResult(QJsonObject obj);
    void changePassResult(QJsonObject obj);
    void getUesrInfo(QJsonObject obj);
    void changeHead(QJsonObject obj);
    void queryRoomMate(QJsonObject obj);
    void getBookHistory(QJsonObject obj);
    void bookResults(QJsonObject obj);
    void outBookResult(QJsonObject obj);
    void picScanResult(QJsonObject obj);

private slots:
    void replyFinished(QNetworkReply *);

signals:
    void imelLogin(bool loginResult, QString msg);
    void passwordLogin(bool loginResult, QString msg);
    void queryUsers(QMap<int,QString> map, QString roomId, QString room);
    void queryBookKeeping(QJsonArray dataArrary);
    void bookResult(bool bookResult, QString msg);
    void lastLoginTime(qint64 time);
    void changePasswordResult(bool result, QString msg);
    void changeHeadResult(bool result);
    void outBook(QJsonObject obj);
    void outBookError(QString msg);
    void picScan(QJsonObject obj);
};

extern Cvlink mainlink;

#endif // CVLINK_H
