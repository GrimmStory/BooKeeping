#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QProcess>
#include <QSettings>
#include <QImage>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QBuffer>
#include <QPixmap>
#include <QFile>

class Global
{
public:
    Global();
    static QString getWMIC(const QString &cmd);
    static QList<QNetworkCookie> cookies;
    static QByteArray imageToBase64(QString imagePath);
    static bool base64ToImage(QString base64, QString filePath);
    static QString parseTime_t(qint64 time_t);
    static QString getTime_t(QDateTime time);

public:
    static QSettings *IMConfig;
    static QString bookUrl;
    static QNetworkAccessManager *globalManager;
};

#endif // GLOBAL_H
