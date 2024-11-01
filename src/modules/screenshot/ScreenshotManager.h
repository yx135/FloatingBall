#pragma once
#include <QObject>
#include "../../core/ModuleBase.h"
#include <QMessageBox>

class ScreenshotManager : public ModuleBase {
    Q_OBJECT
public:
    explicit ScreenshotManager(QObject* parent = nullptr);
    void initialize() override;
    void cleanup() override;
    
public slots:
    void startScreenshot();  // 原来的 takeScreenshot 方法
    
private:
    QWidget* getMainWindow() const;  // 辅助方法获取主窗口
};