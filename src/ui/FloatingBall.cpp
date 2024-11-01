#include "FloatingBall.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QApplication>
#include <QDebug>
#include<QMenu>
FloatingBall::FloatingBall(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupButtons();
    screenshotManager = new ScreenshotManager(this);
    
    // 连接截图按钮
    connect(screenshotButton, &QPushButton::clicked, 
            screenshotManager, &ScreenshotManager::startScreenshot);

    connect(chatButton, &QPushButton::clicked,[this](){
        qDebug()<<"构建aichat";
        ChatManager* chatWindow = new ChatManager();
        chatWindow->setAttribute(Qt::WA_DeleteOnClose);
        chatWindow->show();
         // 将 FloatingBall 窗口设置为非活动状态
        this->clearFocus();
        this->lower();
    });
}

FloatingBall::~FloatingBall() {
    if (sizeAnimation) {
        sizeAnimation->stop();
        delete sizeAnimation;
    }
}

void FloatingBall::setupUI() {
    // 设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_MacAlwaysShowToolWindow);


    //设置窗口为工具窗口
    setWindowFlags(windowFlags() | Qt::ToolTip);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_MacAlwaysShowToolWindow);
    createTrayIcon();
    
    // 设置固定初始大小
    setFixedSize(COLLAPSED_SIZE);
    
    // 移动到屏幕右上角
    if (QScreen *screen = QApplication::primaryScreen()) {
        QRect screenGeometry = screen->geometry();
       int centerY = (screenGeometry.height() - height()) /3;
        move(screenGeometry.width() - width() - 20, 2*centerY);

    }
    
    // 创建主布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(5);
    
    // 初始化双击定时器
    doubleClickTimer = new QTimer(this);
    doubleClickTimer->setSingleShot(true);
    connect(doubleClickTimer, &QTimer::timeout, this, &FloatingBall::handleSingleClick);
    
    // 初始化动画
    sizeAnimation = new QPropertyAnimation(this, "size");
    sizeAnimation->setDuration(300);
    sizeAnimation->setEasingCurve(QEasingCurve::OutCubic);


}

void FloatingBall::setupButtons() {
    screenshotButton = new QPushButton("截图", this);
    chatButton = new QPushButton("聊天", this);
    
    // 设置按钮样式
    QString buttonStyle = 
        "QPushButton {"
        "    background-color: rgba(255, 255, 255, 180);"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 5px;"
        "    color: black;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 220);"
        "}";
    
    screenshotButton->setStyleSheet(buttonStyle);
    chatButton->setStyleSheet(buttonStyle);
    
    mainLayout->addWidget(screenshotButton);
    mainLayout->addWidget(chatButton);
    
    screenshotButton->hide();
    chatButton->hide();
    
    
}
void FloatingBall::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        toggleExpansion();
        event->accept();
    }
}

void FloatingBall::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void FloatingBall::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void FloatingBall::toggleExpansion() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    QPoint globalPos = mapToGlobal(QPoint(0, 0));

    if (!isExpanded) {
        // 展开
        if (globalPos.x() < screenGeometry.width() / 2) {
            // 在左半边屏幕，向右展开
            setFixedSize(EXPANDED_SIZE);
        } else {
            // 在右半边屏幕，向左展开
            QPoint newPos = globalPos;
            newPos.setX(globalPos.x() - (EXPANDED_SIZE.width() - COLLAPSED_SIZE.width()));
            move(newPos);
            setFixedSize(EXPANDED_SIZE);
        }
        screenshotButton->show();
        chatButton->show();
    } else {
        // 折叠
        QPoint newPos = pos();
        if (globalPos.x() >= screenGeometry.width() / 2) {
            newPos.setX(newPos.x() + (EXPANDED_SIZE.width() - COLLAPSED_SIZE.width()));
        }
        setFixedSize(COLLAPSED_SIZE);
        move(newPos);
        screenshotButton->hide();
        chatButton->hide();
    }

    isExpanded = !isExpanded;
    update();
}
void FloatingBall::handleSingleClick() {
    isWaitingForDoubleClick = false;
}

void FloatingBall::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.setBrush(QColor(100, 100, 100, 180));
    painter.setPen(Qt::NoPen);
    
    if (size() == COLLAPSED_SIZE) {
        // 完全收起状态，绘制圆形
        painter.drawEllipse(rect());
    } else if (size() == EXPANDED_SIZE) {
        // 完全展开状态，绘制圆角矩形
        painter.drawRoundedRect(rect(), 15, 15);
    } else {
        // 过渡状态，根据大小插值计算形状
        qreal progress = (width() - COLLAPSED_SIZE.width()) / 
                        (qreal)(EXPANDED_SIZE.width() - COLLAPSED_SIZE.width());
        if (progress < 0.5) {
            painter.drawEllipse(rect());
        } else {
            painter.drawRoundedRect(rect(), 15, 15);
        }
    }
}

// 右键菜单
void FloatingBall::contextMenuEvent(QContextMenuEvent* event) {
    QMenu menu(this);
    menu.addAction("设置", this, &FloatingBall::showSettings);
    menu.addAction("切换显示/隐藏", this, &FloatingBall::toggleVisibility);
    menu.addSeparator();
    menu.addAction("退出", qApp, &QApplication::quit);
    
    menu.exec(event->globalPos());
}
void FloatingBall::toggleVisibility() {
    if (isVisible()) {
        hide();
        // 可以添加淡出动画
        QPropertyAnimation* fadeOut = new QPropertyAnimation(this, "windowOpacity");
        fadeOut->setDuration(200);
        fadeOut->setStartValue(opacity);
        fadeOut->setEndValue(0.0);
        connect(fadeOut, &QPropertyAnimation::finished, this, &QWidget::hide);
        fadeOut->start(QPropertyAnimation::DeleteWhenStopped);
    } else {
        // 显示前重置透明度
        setWindowOpacity(0.0);
        show();

        // 添加淡入动画
        QPropertyAnimation* fadeIn = new QPropertyAnimation(this, "windowOpacity");
        fadeIn->setDuration(200);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(opacity);
        fadeIn->start(QPropertyAnimation::DeleteWhenStopped);
    }
}

//创建系托盘
void FloatingBall::createTrayIcon()
{

    trayIcon = new QSystemTrayIcon(this);

    // 创建一个类似悬浮球的图标
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制主圆
    QRadialGradient gradient(16, 16, 16);
    gradient.setColorAt(0, QColor(100, 100, 255));
    gradient.setColorAt(1, QColor(50, 50, 200));
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(1, 1, 30, 30);

    // 添加高光效果
    painter.setBrush(QColor(255, 255, 255, 100));
    painter.drawEllipse(5, 5, 10, 10);

    trayIcon->setIcon(QIcon(pixmap));

    QMenu *trayMenu = new QMenu(this);
    QAction *showAction = trayMenu->addAction("显示/隐藏");
    QAction *settingsAction = trayMenu->addAction("设置");
    QAction *quitAction = trayMenu->addAction("退出");

    connect(showAction, &QAction::triggered, this, &FloatingBall::toggleVisibility);
    connect(settingsAction, &QAction::triggered, this, &FloatingBall::showSettings);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    qApp->setQuitOnLastWindowClosed(false);


}

void FloatingBall::showSettings() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        applySettings();
    }
}

void FloatingBall::applySettings() {
    ConfigManager* config = AppManager::getInstance().getConfigManager();
     if (!config) {
        qWarning() << "ConfigManager is not initialized";
        return;
    }
    
}

void FloatingBall::updateGlobalShortcuts() {
 
    // 更新其他快捷键...
}

