#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include<QFormLayout>
#include<QListWidget>
#include<QStackedWidget>
#include "../core/AppManager.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    shortcutEdit = new QLineEdit(this);
    startupCheck = new QCheckBox(this);
    opacitySlider = new QSlider(Qt::Horizontal, this);
    stackedWidget = new QStackedWidget(this);
    
    // 初始化 AI Chat 设置控件
    aichat_api_key = new QTextEdit(this);
    aichat_api_endpoint = new QTextEdit(this);
    aichat_api_key->setFixedHeight(80);
    aichat_api_endpoint->setFixedHeight(80);
    aichat_Prompt = new QTextEdit(this);
    aichat_Prompt->setFixedHeight(80);
    //翻译设置
    translator_deeplx_api = new QTextEdit(this);
    translator_deeplx_api->setFixedHeight(50);
    translator_ai_apiKey = new QTextEdit(this);
    translator_ai_apiKey->setFixedHeight(50);
    translator_ai_apiEndpoint = new QTextEdit(this);
    translator_ai_apiEndpoint->setFixedHeight(50);
    setupUI();
    loadSettings();
}

void SettingsDialog::setupUI() {
    setWindowTitle(tr("设置"));
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // 创建分类列表
    QListWidget* categoryList = new QListWidget(this);
    categoryList->setFixedWidth(80);
    categoryList->addItem(tr("常规"));
    categoryList->addItem(tr("AI Chat"));
    categoryList->addItem(tr("翻译"));
    // 创建常规窗口
    QWidget* generalPage = new QWidget(this);
    QFormLayout* generalLayout = new QFormLayout(generalPage);
    generalLayout->addRow(tr("显示/隐藏快捷键:"), shortcutEdit);
    generalLayout->addRow(tr("开机启动:"), startupCheck);
    generalLayout->addRow(tr("透明度:"), opacitySlider);
    stackedWidget->addWidget(generalPage);
    
    // 添加 AI Chat 设置页面
    QWidget* chatSettingsPage = new QWidget(this);
    QFormLayout* chatLayout = new QFormLayout(chatSettingsPage);
    chatLayout->addRow(tr("AI Chat API Key:"), aichat_api_key);
    chatLayout->addRow(tr("AI Chat API 端点:"), aichat_api_endpoint);
    chatLayout->addRow(tr("提示词:"),aichat_Prompt);
    stackedWidget->addWidget(chatSettingsPage);

    // 添加翻译设置页面
    QWidget* translateSettingsPage = new QWidget(this);
    QFormLayout* translateLayout = new QFormLayout(translateSettingsPage);
    translateLayout->addRow(tr("DeepL API Key:"), translator_deeplx_api);
    translateLayout->addRow(tr("AI 端点:"), translator_ai_apiEndpoint);
    translateLayout->addRow(tr("AI key:"), translator_ai_apiKey);
    stackedWidget->addWidget(translateSettingsPage);
    translateLayout->setContentsMargins(0,0,0,0);
    translateSettingsPage->setContentsMargins(0,0,0,0);
    
    // 创建水平布局
    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(categoryList);
    horizontalLayout->addWidget(stackedWidget);
    mainLayout->addLayout(horizontalLayout);
    
    // 添加按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* okButton = new QPushButton(tr("确定"), this);
    QPushButton* cancelButton = new QPushButton(tr("取消"), this);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号和槽
    connect(categoryList, &QListWidget::currentRowChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::saveSettings);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    // 设置初始页面
    categoryList->setCurrentRow(0);
}
void SettingsDialog::saveSettings() {
    if (!config) {
        qWarning() << "ConfigManager is not initialized";
        return;
    }
    
    //config->setValue(ConfigKeys::SHORTCUT_TOGGLE, shortcutEdit->text());
    //config->setValue(ConfigKeys::GENERAL_STARTUP, startupCheck->isChecked());
    //config->setValue(ConfigKeys::APPEARANCE_OPACITY, opacitySlider->value() / 100.0f);
    
    // 保存 AI Chat 设置
    qDebug()<<"保存配置";
    config->setValue("aichat/api_key", aichat_api_key->toPlainText());
    config->setValue("aichat/api_endpoint", aichat_api_endpoint->toPlainText());
    config->setValue("aichat/prompt",aichat_Prompt->toPlainText());

    // 保存翻译设置
    config->setValue("translator/deeplx_api",translator_deeplx_api->toPlainText());
    config->setValue("translator/ai_api_key",translator_ai_apiKey->toPlainText());
    config->setValue("translator/ai_api_endpoint",translator_ai_apiEndpoint->toPlainText());
    qDebug()<<"保存配置完成";
    config->sync();
    accept();
}

void SettingsDialog::loadSettings() {
    qDebug()<<"开始加载配置...";
    config = AppManager::getInstance().getConfigManager();
    if (!config) {
        qWarning() << "ConfigManager is not initialized";
        return;
    }
    qDebug()<<"加载配置";
    
    //shortcutEdit->setText(config->getValue(ConfigKeys::SHORTCUT_TOGGLE, QString("Ctrl+Shift+F")));
    //startupCheck->setChecked(config->getValue(ConfigKeys::GENERAL_STARTUP, false));
    //opacitySlider->setValue(config->getValue(ConfigKeys::APPEARANCE_OPACITY, 0.4) * 100);
    
    aichat_api_key->setText(config->getValue("aichat/api_key", QString()));
    aichat_api_endpoint->setText(config->getValue("aichat/api_endpoint",QString()));
    aichat_Prompt->setText(config->getValue("aichat/prompt",QString()));
    translator_deeplx_api->setText(config->getValue("translator/deeplx_api",QString()));
    translator_ai_apiKey->setText(config->getValue("translator/ai_api_key",QString()));
    translator_ai_apiEndpoint->setText(config->getValue("translator/ai_api_endpoint",QString()));
    qDebug()<<"加载配置完成";
}
void SettingsDialog::setDefaultConfig() 
{
    config->setValue("aichat/api_key", default_aichat_apiKey);
    config->setValue("aichat/api_endpoint", default_aichat_apiEndpoint);
    config->setValue("aichat/prompt", default_aichat_Prompt);
    config->setValue("translator/deeplx_api", default_deeplx_api);
    config->setValue("translator/ai_api_key", default_translate_ai_apiKey);
    config->setValue("translator/ai_api_endpoint", default_translate_ai_apiEndpoint);
    config->sync();
}
