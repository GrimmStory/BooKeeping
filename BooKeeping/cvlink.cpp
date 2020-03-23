#include "cvlink.h"

Cvlink::Cvlink()
{
    manager =new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
}

Cvlink::setUrl(QString url)
{
    this->postUrl = url;
    QUrl urls(this->postUrl);
    request.setUrl(urls);
}

Cvlink::setJson(QJsonObject data)
{
    QJsonDocument docum;

    docum.setObject(data);
    data_json=docum.toJson(QJsonDocument::Compact);
}

Cvlink::post()
{
    manager->post(request,data_json);
}

void Cvlink::postHttpRequest(QString url, QMap<QString, QString> argMap)
{
    QJsonObject json;
    foreach (QString key, argMap.keys()) {
        json.insert(key, argMap[key]);
    }
    mainlink.setUrl(url);
    mainlink.setJson(json);
    mainlink.post();
}

void Cvlink::replyFinished(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();//这个返回的JSON包所携带的所有信息

    QString result = reply->readAll();

    QVariant variantCookies = NULL;
    variantCookies = reply->header(QNetworkRequest::SetCookieHeader);
    if(variantCookies.isValid()){
    }

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "status code" << statusCode << endl;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << QStringLiteral("解析Json失败");
        return;
    }

    if (doucment.isObject()) {
        QJsonObject obj = doucment.object();
        QJsonValue value;
        value = obj.value("code");
        if (value.toString() == "000000") {
            value = obj.value("resultCode");
            if (value.toVariant().toString() == "000000") {          //使用账户密码登录成功
                emit passwordLogin(true);
            }else if(value.toVariant().toString() == "000001"){       //使用imel登录成功
                emit imelLogin(true);
            }else if(value.toVariant().toString() == "001001"){       //请求宿舍用户列表
                QString room;
                value = obj.value("data");
                QJsonObject secondJson = value.toObject();
                QString roomId = QString::number(secondJson["id"].toDouble());
                if(!secondJson["comment"].toString().isNull()){
                    room = secondJson["comment"].toString();
                    if(secondJson["users"].isArray()){
                        QJsonArray userJsonArray = secondJson["users"].toArray();
                        for(int i = 0; i < userJsonArray.size(); i ++){
                            QJsonValue childValue = userJsonArray.at(i);
                            QJsonObject childObject = childValue.toObject();
                            int id = childObject["id"].toDouble();
                            QString name = childObject["name"].toString();
                            userMap.insert(id, name);
                        }
                    }
                }
                emit queryUsers(userMap, roomId, room);
            }else if(value.toVariant().toString() == "002000"){       //请求宿舍用户列表
                QJsonArray dataArrary = obj.value("data").toArray();
                emit queryBookKeeping(dataArrary);
            }else if(value.toVariant().toString() == "002001"){       //入账
                emit bookResult(true);
            }else{
                qDebug() << "result " << "else" << endl;

            }
        }
    }
}

Cvlink mainlink;
