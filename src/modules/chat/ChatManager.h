#ifndef AICHAT_H
#define AICHAT_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include<QJsonArray>
#include <QListWidget>
#include <QSplitter>
#include <QScreen>
#include <QDateTime>
#include<QDialog>
#include<QComboBox>
#include<QLabel>
#include <QCloseEvent>
#include "../../core/ConfigManager.h"
#include "../../core/AppManager.h"

class ConfigManager;
class ChatManager : public QWidget
{
    Q_OBJECT

public:
    explicit ChatManager(QWidget *parent = nullptr);
    ~ChatManager();

public:
   // Ui::MainWindow *ui;
    QString m_apiKey;
    QString m_apiEndpoint;
    QString m_prompt;
    QComboBox* model;
    QTextEdit* chatTextEdit;
    QLineEdit* chatInputLine;
    QPushButton* sendButton;
    QNetworkAccessManager* networkManager;
    QPushButton *cancelButton;
    QNetworkReply *currentReply;
    QPushButton *newChatButton;
    QPushButton *historyButton;
    QDialog *historyDialog;
    QListWidget *historyList;
    
    QString currentChatId;
    QMap<QString, QJsonArray> chatHistories;
    QJsonArray chatHistory;
    QStringList modelist;
    ConfigManager* config;

    void toggleChatWidget();
    void sendMessage();
    void sendMessageToAI(const QString &message);
    void handleAIResponse();
    void showHistory();
    void createNewChat();
    void loadChat(QListWidgetItem *item);
    void saveCurrentChat();
    void updateHistoryList();
    void loadChat(const QString &chatId);
    void initmodelist();
    void closeEvent(QCloseEvent *event) override;
    void updateApiConfig();
    void sendPromptMessageToAI();
private slots:
    // ... 其他槽函数 ...
    void handleNetworkError(QNetworkReply::NetworkError error);
    void cancelRequest();
    void deleteSelectedChat();
    void loadSelectedChat(QListWidgetItem *item);

};

#endif // AICHAT_H
