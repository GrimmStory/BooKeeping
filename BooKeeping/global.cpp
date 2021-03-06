#include "global.h"

QSettings* Global::IMConfig;
QString Global::bookUrl;
QList<QNetworkCookie> Global::cookies;
QNetworkAccessManager* Global::globalManager;

Global::Global()
{

}

/**
* @brief    获取系统信息
* @params   cmd 命令参数
* @author   Joker
*/
QString Global::getWMIC(const QString &cmd)
{
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    return result.trimmed();
}

/**
* @brief    图片base64编码
* @params   imagePath 图片路径
* @author   Joker
*/
QByteArray Global::imageToBase64(QString imagePath)
{
    QImage image(imagePath);
    QByteArray ba;
    QBuffer buf(&ba);
    image.save(&buf, "jpg");
    QByteArray hexed = ba.toBase64();
    buf.close();
    qDebug() << "base64 " << hexed << endl;
    return hexed;
}

bool Global::base64ToImage(QString base64, QString filePath)
{
    QPixmap image;
    qDebug() << "base64 is " << base64 << endl;
    image.loadFromData(QByteArray::fromBase64(base64.toLocal8Bit()));
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    if(image.save(&file, "JPG")){
        return true;
    } else {
        return false;
    }
}

/**
* @brief    解析时间戳
* @params   time_t 时间戳
* @author   Joker
*/
QString Global::parseTime_t(qint64 time_t)
{
    QDateTime time = QDateTime::fromTime_t(time_t / 1000);
    QString result = time.toString("yyyy/MM/dd hh:mm:ss");
    return result;
}

/**
* @brief    获取时间戳
* @params
* @author   Joker
*/
QString Global::getTime_t(QDateTime time)
{
    qint64 timeT = time.toTime_t();   //将当前时间转为时间戳
    return QString::number(timeT * 1000);
}

QPixmap Global::PixmapToRound(const QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}


