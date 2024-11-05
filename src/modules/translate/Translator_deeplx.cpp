#include"Translator_deeplx.h"
#include<QtNetwork>
#include"../../core/ConfigManager.h"
Translator_deeplx::Translator_deeplx()
{
    this->apiUrl = config->getValue("translator/deeplx_api",QString());
}

void Translator_deeplx::sendTranslate(QString text,QString target_lang)
{   
    qDebug()<<"发送翻译请求";
   QNetworkRequest request;
   request.setUrl(QUrl(this->apiUrl));
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
        emit translated(response);
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
