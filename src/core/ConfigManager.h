/*
说明：
ConfigManager 负责管理应用程序的配置。
详细功能：
1. 读取和写入配置文件
2. 提供对配置的访问接口
*/
#pragma once
#include "ModuleBase.h"
#include <QSettings>
#include <QString>

class ConfigManager : public ModuleBase {
    Q_OBJECT

public:
    explicit ConfigManager(QObject* parent = nullptr);
    ~ConfigManager() override;
    
    void initialize() override;
    void cleanup() override;
    
    template<typename T>
    T getValue(const QString& key, const T& defaultValue = T()) const {
        return settings.value(key, defaultValue).template value<T>();
    }
    
    template<typename T>
    void setValue(const QString& key, const T& value) {
        settings.setValue(key, value);
        settings.sync();
        emit configChanged(key);
    }

signals:
    void configChanged(const QString& key);
    
private:
    QSettings settings{"FloatingBall", "Config"};
    //默认配置
   QString default_apiKey = "sk-adfrHgGRxcyfISG90135D10cEc824f14B1A15cA1Cd357a36-73";
    QString default_apiEndpoint = "https://oneapidev.onrender.com/v1/chat/completions";
    QString default_aichatPrompt = "你是一个有帮助的AI助手,请用中文回答问题，对于这句话无需向我回复";

};
