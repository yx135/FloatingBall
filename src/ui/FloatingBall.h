#pragma once
#include <QWidget>
#include <QPoint>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include "../core/AppManager.h"
#include "../core/ConfigManager.h" 
#include "../modules/chat/ChatManager.h" // 添加这行来包含 ChatManager 的头文件
#include "../modules/screenshot/ScreenshotManager.h"
#include "SettingsDialog.h"

class ScreenshotManager;  // 前向声明

class FloatingBall : public QWidget {
    Q_OBJECT
    
public:
    explicit FloatingBall(QWidget *parent = nullptr);
    ~FloatingBall();
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

    
private:
    void setupUI();
    void setupButtons();
    void toggleExpansion();
    void handleSingleClick();
    void toggleVisibility();
    
    QPoint dragPosition;
    bool isExpanded = false;
    bool isWaitingForDoubleClick = false;
    
    // UI 元素
    QVBoxLayout* mainLayout = nullptr;
    QPushButton* screenshotButton = nullptr;
    QPushButton* chatButton = nullptr;
    QTimer* doubleClickTimer = nullptr;
    QPropertyAnimation* sizeAnimation = nullptr;
    
    // 尺寸常量
    const QSize COLLAPSED_SIZE{50, 50};
    const QSize EXPANDED_SIZE{150, 200};
    QPoint ballPos;

    // 设置相关
     float opacity = 0.4f;
     bool startWithSystem = false;
     QString toggleShortcut = "Ctrl+Shift+F";
     void applySettings();
     void showSettings();
    void updateGlobalShortcuts();
     //系统托盘
     QSystemTrayIcon* trayIcon = nullptr;
     void createTrayIcon();

     //截图
     ScreenshotManager* screenshotManager;
     //ScreenshotSelector* screenshotSelector = nullptr;
    QString readStyleFromFile(const QString &filePath);

};
