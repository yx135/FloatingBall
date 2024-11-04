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
    //aichat_api_key->setLineWrapMode(QTextEdit::NoWrap);
    //aichat_api_endpoint->setLineWrapMode(QTextEdit::NoWrap);

    
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
    ConfigManager* config = AppManager::getInstance().getConfigManager();
    if (!config) {
        qWarning() << "ConfigManager is not initialized";
        return;
    }
    
    //config->setValue(ConfigKeys::SHORTCUT_TOGGLE, shortcutEdit->text());
    //config->setValue(ConfigKeys::GENERAL_STARTUP, startupCheck->isChecked());
    //config->setValue(ConfigKeys::APPEARANCE_OPACITY, opacitySlider->value() / 100.0f);
    
    // 保存 AI Chat 设置
    config->setValue("aichat/api_key", aichat_api_key->toPlainText());
    config->setValue("aichat/api_endpoint", aichat_api_endpoint->toPlainText());
    config->setValue("aicht/prompt",aichat_Prompt->toPlainText());
   
    accept();
}

void SettingsDialog::loadSettings() {
    qDebug()<<"开始加载配置...";
    ConfigManager* config = AppManager::getInstance().getConfigManager();
    if (!config) {
        qWarning() << "ConfigManager is not initialized";
        return;
    }
    
    //shortcutEdit->setText(config->getValue(ConfigKeys::SHORTCUT_TOGGLE, QString("Ctrl+Shift+F")));
    //startupCheck->setChecked(config->getValue(ConfigKeys::GENERAL_STARTUP, false));
    //opacitySlider->setValue(config->getValue(ConfigKeys::APPEARANCE_OPACITY, 0.4) * 100);
    
    // 加载 AI Chat 设置
    //qDebug()<<"aichat/api_key:"<<config->getValue("aichat/api_key", QString());
    //qDebug()<<"aichat/api_endpoint:"<<config->getValue("aichat/api_endpoint",QString());
    aichat_api_key->setText(config->getValue("aichat/api_key", QString()));
    aichat_api_endpoint->setText(config->getValue("aichat/api_endpoint",QString()));
    aichat_Prompt->setText(config->getValue("aichat/prompt",QString()));
    qDebug()<<"加载配置完成";
}
