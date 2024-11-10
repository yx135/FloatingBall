/*
翻译模块
返回格式：
{
    "data":"翻译后的文本",
    "alternatives":["其他翻译1","其他翻译2","其他翻译3"]
}
*/
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QClipboard>

// 前向声明
class ConfigManager;
class AppManager;  

class Translator : public QObject
{
    Q_OBJECT
protected:
    QString apiUrl;
    QNetworkAccessManager *manager;
    ConfigManager* config;
    
public:
    explicit Translator(QObject *parent = nullptr);
    virtual void sendTranslate(QString text, QString target_lang,QString modelename) = 0;

signals:
    void translated(QString text); 
};

#endif // TRANSLATOR_H
