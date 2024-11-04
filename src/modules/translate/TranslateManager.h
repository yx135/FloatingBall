#ifndef TRANSLATEMANAGER_H
#define TRANSLATEMANAGER_H
#include <QObject>
#include"QString"
#include"QDebug"
#include<QNetworkAccessManager>

class TranslateManager:public QObject
{
    Q_OBJECT
private:
    QString apiKey="rzd2JN1zIXlHiFagHmY5c9M2rguamJk9JU48qwmonHU";
    QString apiUrl="https://api.deeplx.org/rzd2JN1zIXlHiFagHmY5c9M2rguamJk9JU48qwmonHU/translate";
    QString target_lang = "ZH";
    QNetworkAccessManager *manager;
public:
    void sendTranslate(QString text);
    TranslateManager(QObject *parent = nullptr);

signals:
    void translated(QString text);  
};

#endif 