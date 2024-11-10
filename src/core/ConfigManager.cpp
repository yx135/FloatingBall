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
    qDebug()<<"Initializing ConfigManager 完成";
    qDebug() << settings.fileName();
}

void ConfigManager::cleanup() {
    qDebug() << "Cleaning up ConfigManager";
    settings.sync();
}
void ConfigManager::sync() {
    settings.sync();
}
