#include"Translator.h"
#include<QtNetwork>
#include"../../core/AppManager.h"
#include"../../core/ConfigManager.h"
Translator::Translator(QObject *parent)
:QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    config = AppManager::getInstance().getConfigManager();
}