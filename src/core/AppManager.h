/*
说明：
AppManager 是应用程序的核心管理器，负责初始化和管理各个模块。
详细功能：
1. 初始化 ConfigManager 和 EventSystem
2. 提供对 ConfigManager 和 EventSystem 的访问接口
*/

#pragma once

#include <QDebug>
#include <memory>

class ConfigManager;
class EventSystem;

class AppManager {
public:
    static AppManager& getInstance();
    
    void initialize();
    void cleanup();
    
    ConfigManager* getConfigManager() const { return configManager.get(); }
    EventSystem* getEventSystem() const { return eventSystem.get(); }

private:
    AppManager() = default;
    ~AppManager() = default;
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;

    std::unique_ptr<ConfigManager> configManager;
    std::unique_ptr<EventSystem> eventSystem;
    
    bool isInitialized = false;
};