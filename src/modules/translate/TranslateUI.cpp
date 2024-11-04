#include "TranslateUI.h"
#include <QVBoxLayout>
#include<QDebug>
#include<QApplication>
#include<QScreen>
#include"TranslateManager.h"
TranslateUI::TranslateUI(QWidget *parent):QWidget(parent)
{        qDebug()<<"构建翻译";

        setupUI();
        connectButton();
        qDebug()<<"构建完成";

}

void TranslateUI::setupUI()
{
    setWindowTitle("翻译工具");
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
  
     this->resize(200,200);
    //设置窗口大小
    //this->setFixedSize(100, 80);
    originalText = new QTextEdit(this);
    translatedText = new QTextEdit(this);
    
    QWidget* widget = new QWidget(this);
    translateButton = new QPushButton("翻译", widget);
    clearButton = new QPushButton("清空", widget);

    //设置高度
    //originalText->setFixedHeight(100);
    //translatedText->setFixedHeight(100);
    //设置布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(originalText);
    layout->addWidget(translatedText);
    layout->addWidget(widget);

    //设置水平布局
    QHBoxLayout* hlayout = new QHBoxLayout(widget);
    hlayout->addWidget(translateButton);
    hlayout->addWidget(clearButton);
    //设置���局

    //移动到屏幕右下角
    // 修改移动到屏幕右下角的代码
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = screenGeometry.width() - this->width();
    int y = screenGeometry.height() - this->height();
    this->move(x, y);}

void TranslateUI::clearText()
{
    originalText->clear();
    translatedText->clear();
}

void TranslateUI::translateText()
{
    // 清除之前的翻译结果
    qDebug()<<"翻译： "<<originalText->toPlainText();
    translatedText->clear();
    
    // 获取要翻译的文本
    QString text = originalText->toPlainText();
    if (text.isEmpty()) {
        translatedText->append("请输入要翻译的文本");
        return;
    }
    
    // 创建翻译管理器并保存为成员变量，避免内存泄漏
    if (m_translateManager) {
        m_translateManager->deleteLater();//删除翻译管理器
    }
    m_translateManager = new TranslateManager(this);
    
    // 连接信号槽
    connect(m_translateManager, &TranslateManager::translated,
            this, [this](const QString &text){
                translatedText->append(text);
            });
    
    // 发送翻译请求
    m_translateManager->sendTranslate(text);
}

void TranslateUI::connectButton()
{
    connect(translateButton, &QPushButton::clicked, this, &TranslateUI::translateText);
    connect(clearButton, &QPushButton::clicked, this, &TranslateUI::clearText);
    //文本改变
    //connect(originalText,&QTextEdit::textChanged,this,&TranslateUI::translateText);
    //回车
   // connect(originalText,&QTextEdit::returnPressed,this,&TranslateUI::translateText);

}   
TranslateUI::~TranslateUI()
{

}
