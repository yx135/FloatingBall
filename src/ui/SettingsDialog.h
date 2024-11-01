#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QSlider>
#include<QString>
#include <QStackedWidget>
#include <QFormLayout>
#include "../core/AppManager.h"
#include "../core/ConfigManager.h"  
class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    
private slots:
    void saveSettings();
    void loadSettings();
    
private:
    // UI 控件
    QLineEdit* shortcutEdit;
    QCheckBox* startupCheck;
    QSlider* opacitySlider;
    QStackedWidget* stackedWidget;
    
    // AI Chat 设置控件
    QTextEdit* aichat_api_key;
    QTextEdit* aichat_api_endpoint;
    
    void setupUI();
    void setupChatSettings();
};
