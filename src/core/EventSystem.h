/*
说明：
EventSystem 负责处理应用程序的事件。
详细功能：
1. 初始化事件系统
2. 提供事件的注册和处理接口
*/
#pragma once

#include <QString>

class EventSystem {
public:
    EventSystem() = default;
    ~EventSystem() = default;
    
    void initialize();
    void cleanup();
};