#include "screenshotSelector.h"
#include <QApplication>
#include <QScreen>
#include <QKeyEvent>

ScreenshotSelector::ScreenshotSelector(QWidget *parent) : QWidget(parent), selecting(false)
{
    // 设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setCursor(Qt::CrossCursor);

/*
    // 捕获整个屏幕
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        screenPixmap = screen->grabWindow(0);
        setGeometry(screen->geometry());
    }
*/
}

void ScreenshotSelector::paintEvent(QPaintEvent*)
{
    /*
    QPainter painter(this);
    
    // 绘制背景截图
    painter.drawPixmap(rect(), screenPixmap);
    
    // 添加半透明遮罩
    painter.fillRect(rect(), QColor(0, 0, 0, 100));

    if (!selectedRect.isNull())
    {
        // 清除选区的遮罩，显示原始图像
        painter.drawPixmap(selectedRect, screenPixmap, selectedRect);
        
        // 绘制选区边框
        painter.setPen(QPen(Qt::white, 2));
        painter.drawRect(selectedRect);
    }
    */
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, 0));

    if (!selectedRect.isNull())
    {
        painter.fillRect(selectedRect, QColor(255, 255, 255, 0));
        painter.setPen(QPen(Qt::black, 1));
        painter.drawRect(selectedRect);
    }
}

void ScreenshotSelector::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        selecting = true;
        startPos = event->pos();
        selectedRect = QRect();
        update();
    }
}

void ScreenshotSelector::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting)
    {
        selectedRect = QRect(startPos, event->pos()).normalized();
        update();
    }
}

void ScreenshotSelector::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        selecting = false;
        selectedRect = QRect(startPos, event->pos()).normalized();
        
        if (!selectedRect.isEmpty()) {
            emit screenshotTaken(selectedRect);
        }
        close();
    }
}

/*
void ScreenshotSelector::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit selectionCanceled();
        close();
    }
}
*/