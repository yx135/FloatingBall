#include"Translator_ai.h"
#include<QtNetwork>
#include"../../core/ConfigManager.h"
Translator_ai::Translator_ai()
{
    this->apiUrl = config->getValue("translator/ai_api_endpoint",QString());
    this->apiKey = config->getValue("translator/ai_api_key",QString());
}

void Translator_ai::sendTranslate(QString text,QString target_lang,QString modelename)
{   
qDebug()<<"发送翻译请求";
   QNetworkRequest request;
   request.setUrl(QUrl(this->apiUrl));
   request.setRawHeader("Content-Type","application/json");
   request.setRawHeader("Authorization","Bearer "+this->apiKey.toUtf8());

   QJsonObject json;
   json["model"]=modelename;
   QJsonArray messages;
   QString user_content = QString("请将如下内容翻译：翻译文本:%1,翻译目标语言:%2;"
                                 "返回内容严格按照如下json格式，不要有其他回复:"
                                 "{"
                                 "\"data\":\"翻译后的文本\","
                                 "\"alternatives\":[\"其他翻译1\",\"其他翻译2\",\"其他翻译3\"]"
                                 "}");
   user_content = user_content.arg(text).arg(target_lang);
   messages.append(QJsonObject{{"role", "user"}, {"content", user_content}});
   json["messages"] = messages;

    //打印发送请求
   qDebug()<<QJsonDocument(json).toJson().toStdString().c_str();
   QNetworkReply *reply = manager->post(request,QJsonDocument(json).toJson());
   QObject::connect(reply,&QNetworkReply::finished,this,[=](){
    if(reply->error()==QNetworkReply::NoError)
    {
        QString response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QString aiMessage = jsonObject["choices"].toArray().first().toObject()["message"].toObject()["content"].toString();
        qDebug()<<"翻译响应成功："<<aiMessage;
        emit translated(aiMessage);
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
