#include "ConfigManager.h"
#include <QDebug>

ConfigManager::ConfigManager(QObject* parent) : ModuleBase(parent) {
    // 构造函数实现
    //initialize();
}

ConfigManager::~ConfigManager() {
    // 析构函数实现
}

void ConfigManager::initialize() {
    qDebug() << "Initializing ConfigManager";

    // 设置默认配置
    if (!settings.contains("aichat/api_key")) {
        settings.setValue("aichat/api_key", default_apiKey);
    }
    if (!settings.contains("aichat/api_endpoint")) {
        settings.setValue("aichat/api_endpoint", default_apiEndpoint);
    }
    if (!settings.contains("aichat/prompt")) {
        settings.setValue("aichat/prompt", default_aichatPrompt);
    }
    settings.sync();
    qDebug()<<"Initializing ConfigManager 完成";
}

void ConfigManager::cleanup() {
    qDebug() << "Cleaning up ConfigManager";
    settings.sync();
}