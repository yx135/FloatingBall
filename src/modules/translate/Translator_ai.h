#ifndef TRANSLATOR_AI_H
#define TRANSLATOR_AI_H

#include "Translator.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QNetworkAccessManager>

class Translator_ai : public Translator
{
    Q_OBJECT
private:
    QString apiUrl = "";
    QString apiKey = "";
public:
    explicit Translator_ai();
    void sendTranslate(QString text, QString target_lang,QString modelename) override;
};

#endif // TRANSLATOR_AI_H
