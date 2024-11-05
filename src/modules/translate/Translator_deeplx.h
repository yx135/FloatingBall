#ifndef TRANSLATOR_DEEPLX_H
#define TRANSLATOR_DEEPLX_H

#include "Translator.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QNetworkAccessManager>

class Translator_deeplx : public Translator
{
    Q_OBJECT
private:
    QString apiUrl = "";
public:
    explicit Translator_deeplx();
    void sendTranslate(QString text, QString target_lang) override;
};

#endif // TRANSLATOR_DEEPLX_H
