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
    void sync();
    
    template<typename T>
    T getValue(const QString& key, const T& defaultValue = T()) const {
        return settings.value(key, defaultValue).template value<T>();
    }
    
    template<typename T>
    void setValue(const QString& key, const T& value) {
        settings.setValue(key, value);
        settings.sync();
        emit configChanged();
    }

signals:
    void configChanged();
    
private:
    QSettings settings{"FloatingBall", "Config"};
   
};
