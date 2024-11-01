#pragma once

#include <QWidget>
#include <QRect>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QScreen>

class ScreenshotSelector : public QWidget
{
    Q_OBJECT

public:
    ScreenshotSelector(QWidget *parent = nullptr);

signals:
    void screenshotTaken(const QRect &rect);
    void selectionCanceled();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //void keyPressEvent(QKeyEvent *event) override;

private:
    bool selecting;
    QPoint startPos;
    QRect selectedRect;
   // QPixmap screenPixmap;  // 存储屏幕截图
};