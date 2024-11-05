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
    QTextEdit* aichat_Prompt;
    
    //翻译设置控件
    QTextEdit* translator_deeplx_api;

    void setupUI();
    void setupChatSettings();
    void setupTranslateSettings();
    //一些默认配置：
    ConfigManager* config = nullptr;
    QString default_apiKey = "sk-adfrHgGRxcyfISG90135D10cEc824f14B1A15cA1Cd357a36-73";
    QString default_apiEndpoint = "https://oneapidev.onrender.com/v1/chat/completions";
    QString default_aichatPrompt = "你是一个有帮助的AI助手,请用中文回答问题，对于这句话无需向我回复";
    QString default_deeplx_api = "https://api.deeplx.org/rzd2JN1zIXlHiFagHmY5c9M2rguamJk9JU48qwmonHU/translate";
    public:
    void setDefaultConfig();

};
