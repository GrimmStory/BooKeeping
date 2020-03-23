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

private slots:
    void replyFinished(QNetworkReply *);

signals:
    void imelLogin(bool loginResult);
    void passwordLogin(bool loginResult);
    void queryUsers(QMap<int,QString> map, QString roomId, QString room);
    void queryBookKeeping(QJsonArray dataArrary);
    void bookResult(bool bookResult);
};

extern Cvlink mainlink;

#endif // CVLINK_H
