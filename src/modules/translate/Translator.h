#ifndef TRANSLATEMANAGER_H
#define TRANSLATEMANAGER_H
#include <QObject>
#include"QString"
#include"QDebug"
#include<QNetworkAccessManager>
#include <QClipboard>  
class Translator:public QObject
{
    Q_OBJECT
protected:
    QString apiUrl;
    QNetworkAccessManager *manager;
public:
    virtual void sendTranslate(QString text,QString target_lang) = 0;
    Translator(QObject *parent = nullptr);
signals:
    void translated(QString text);  
};

#endif 
