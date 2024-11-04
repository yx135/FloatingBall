#ifndef TRANSLATOR_DEEPLX_H
#define TRANSLATOR_DEEPLX_H
#include <QObject>
#include"QString"
#include"QDebug"
#include<QNetworkAccessManager>
#include"Translator.h"
class Translator_deeplx:public Translator
{
    Q_OBJECT
private:
    QString apiUrl="https://api.deeplx.org/rzd2JN1zIXlHiFagHmY5c9M2rguamJk9JU48qwmonHU/translate";
public:
    void sendTranslate(QString text,QString target_lang);
    Translator_deeplx();
};

#endif 
