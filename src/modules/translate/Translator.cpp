#include"Translator.h"
#include<QtNetwork>
Translator::Translator(QObject *parent)
:QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}