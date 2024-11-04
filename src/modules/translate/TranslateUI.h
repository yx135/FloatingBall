#ifndef TRANSLATEUI_H
#define TRANSLATEUI_H
#include<QWidget>
#include <QTextEdit>
#include <QPushButton>
#include<QComboBox>
#include"Translator.h"
#include"AppManager.h"
class TranslateUI:public QWidget
{
    Q_OBJECT
public:
    TranslateUI(QWidget *parent = nullptr);
    ~TranslateUI();
    void setTextAndTranslate(const QString& text);
    void translateClipboardText(); 

private:
    Translator* m_translateManager = nullptr;
protected:
    QTextEdit* originalText;
    QTextEdit* translatedText;
    QPushButton* translateButton;
    QPushButton* clearButton;
    QComboBox* target_lang;
    QComboBox* translator;
    QStringList target_lang_list;
    QStringList translator_list;
    void setupUI();
    void connectButton();
    void initTargetLang();
    void initTranslator();

public slots:
    void clearText();
    void translateText();
};


#endif // TRANSLATEUI_H

