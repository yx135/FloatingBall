#include "TranslateUI.h"
#include <QVBoxLayout>
#include<QDebug>
#include<QApplication>
#include<QScreen>
#include<QClipboard>
#include<QLabel>
TranslateUI::TranslateUI(QWidget *parent):QWidget(parent)
{        qDebug()<<"构建翻译";

        setupUI();
        connectButton();
        initTargetLang();
        initTranslator();
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
    target_lang = new QComboBox(this);
    translator = new QComboBox(this);

    //翻译器布局
    QWidget* translaorwidget = new QWidget(this);
    QHBoxLayout* translatorlayout = new QHBoxLayout(translaorwidget);
    QLabel* translator_lael = new QLabel("翻译器",translaorwidget);
    translatorlayout ->addWidget(translator_lael);
    translatorlayout->addWidget(translator);
    translatorlayout->setContentsMargins(0, 0, 0, 0);

    //目标语言布局
    QWidget* langwidget = new QWidget(this);
    QHBoxLayout* tlayout = new QHBoxLayout(langwidget);
    QLabel* label = new QLabel("目标语言",langwidget);
    tlayout->addWidget(label);
    tlayout->addWidget(this->target_lang);
    tlayout->setContentsMargins(0, 0, 0, 0);



    //设置水平布局
    QHBoxLayout* hlayout = new QHBoxLayout(widget);
    hlayout->addWidget(translateButton);
    hlayout->addWidget(clearButton);
    hlayout->setContentsMargins(0, 5, 0, 5);

    //设置布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(originalText);
    layout->addWidget(translaorwidget);
    layout->addWidget(langwidget);
    layout->addWidget(translatedText);
    layout->addWidget(widget);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(1);

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
    //获取目标语言
    QString target_lang_text = target_lang->currentText();
    // 创建翻译管理器并保存为成员变量，避免内存泄漏
    if (m_translateManager) {
        m_translateManager->deleteLater();//删除翻译管理器
    }
    m_translateManager = AppManager::getInstance().getTranslator(translator->currentText());
    
    // 连接信号槽
    connect(m_translateManager, &Translator::translated,
            this, [this](const QString &text){
                translatedText->append(text);
            });
    
    // 发送翻译请求
    m_translateManager->sendTranslate(text,target_lang_text);
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
void TranslateUI::setTextAndTranslate(const QString& text)
{
    originalText->setText(text);
    translateText();
}
void TranslateUI::translateClipboardText()
{
   // 获取剪贴板内容
    QClipboard *clipboard = QApplication::clipboard();
    QString selectedText = clipboard->text();
    if (!selectedText.isEmpty()) {
       setTextAndTranslate(selectedText);
    }
}
void TranslateUI::initTargetLang()
{
    target_lang_list.append("ZH");
    target_lang_list.append("EN");
    target_lang->addItems(target_lang_list);
}
void TranslateUI::initTranslator()
{
    translator_list.append("deeplx");
    translator_list.append("AI");
    translator->addItems(translator_list);

}
