#ifndef TRANSLATEUI_H
#define TRANSLATEUI_H
#include<QWidget>
#include <QTextEdit>
#include <QPushButton>
#include"TranslateManager.h"
class TranslateUI:public QWidget
{
    Q_OBJECT
public:
    TranslateUI(QWidget *parent = nullptr);
    ~TranslateUI();
private:
    TranslateManager* m_translateManager = nullptr;
protected:
    QTextEdit* originalText;
    QTextEdit* translatedText;
    QPushButton* translateButton;
    QPushButton* clearButton;
    void setupUI();
    void connectButton();

public slots:
    void clearText();
    void translateText();
};


#endif // TRANSLATEUI_H

