#include"TranslateManager.h"
#include<QtNetwork>
TranslateManager::TranslateManager(QObject *parent)
:QObject(parent)
{
    qDebug()<<"构建翻译管理器";
    manager = new QNetworkAccessManager(this);
}


void TranslateManager::sendTranslate(QString text)
{
    qDebug()<<"发送翻译请求";
   QNetworkRequest request;
   request.setUrl(QUrl(this->apiUrl));
  // request.setRawHeader("Authorization",("DeepL-Auth-Key "+apiKey).toUtf8());
   request.setRawHeader("Content-Type","application/json");

   QJsonObject json;
   json["text"]=text;   
   json["target_lang"]=target_lang;
    //打印发送请求
   qDebug()<<QJsonDocument(json).toJson();
   QNetworkReply *reply = manager->post(request,QJsonDocument(json).toJson());
   QObject::connect(reply,&QNetworkReply::finished,this,[=](){
    if(reply->error()==QNetworkReply::NoError)
    {
        QString response = reply->readAll();
        qDebug()<<"翻译响应成功："<<response;
        QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
        QString translatedText = doc.object()["data"].toString();
        emit translated(translatedText);
    }
    else
    {
        qDebug()<<"翻译请求错误："<<reply->errorString();
        qDebug()<<"错误代码："<<reply->error();
        qDebug()<<"服务器响应："<<reply->readAll();
    }
    reply->deleteLater();  // 防止内存泄漏
   });
}
