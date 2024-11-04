#include "FloatingBall.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QApplication>
#include <QDebug>
#include<QMenu>
#include <QWindow>
#include<QThread>
#ifdef Q_OS_MAC
#include <objc/message.h>
#include <objc/runtime.h>
#endif
FloatingBall::FloatingBall(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupButtons();
    setupGlobalShortcuts();
    
    // 连接截图按钮
    connect(screenshotButton, &QPushButton::clicked, [this](){
        screenshotManager = new ScreenshotManager(this);
        screenshotManager->startScreenshot();
    });

    connect(chatButton, &QPushButton::clicked,[this](){
        qDebug()<<"构建aichat";
        ChatManager* chatWindow = new ChatManager();
        chatWindow->setAttribute(Qt::WA_DeleteOnClose);
        chatWindow->show();
         // 将 FloatingBall 窗口设置为非活动状态
        this->clearFocus();
        this->lower();
    });
    //翻译
    connect(translateButton, &QPushButton::clicked,[this](){
        TranslateUI* translateWindow = new TranslateUI();
        translateWindow->setAttribute(Qt::WA_DeleteOnClose);
        translateWindow->show();
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
    
    // 移动到屏幕右中角
    if (QScreen *screen = QApplication::primaryScreen()) {
        QRect screenGeometry = screen->geometry();
       int centerY = (screenGeometry.height() - height()) /3;
        move(screenGeometry.width() - width() - 20, 2*centerY);

    }
    
    // 创建主布局
    mainLayout = new QVBoxLayout(this);
    //设置主布局的边距
    mainLayout->setContentsMargins(5, 5, 5, 5);
    //设置主布局的间距
    mainLayout->setSpacing(5);
    
    // 初始化双击定时器
    doubleClickTimer = new QTimer(this);
    doubleClickTimer->setSingleShot(true);
    connect(doubleClickTimer, &QTimer::timeout, this, &FloatingBall::handleSingleClick);
    
    // 初始化动画
    /*
    sizeAnimation = new QPropertyAnimation(this, "size");
    sizeAnimation->setDuration(300);
    sizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
    */

}

void FloatingBall::setupButtons() {
    screenshotButton = new QPushButton("截图", this);
    chatButton = new QPushButton("聊天", this);
    translateButton = new QPushButton("翻译", this);

    
    //从文件读取样式
    QString buttonStyle = readStyleFromFile(":/resources/styles/style.qss");
    //判断是否读取成功
    if(buttonStyle.isEmpty()){
        qWarning()<<"读取样式失败";
    }
    else{
        screenshotButton->setStyleSheet(buttonStyle);
        chatButton->setStyleSheet(buttonStyle);
        translateButton->setStyleSheet(buttonStyle);
    }
    
    mainLayout->addWidget(screenshotButton);
    mainLayout->addWidget(chatButton);
    mainLayout->addWidget(translateButton);
    
    screenshotButton->hide();
    chatButton->hide();
    translateButton->hide();
    
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
    //获取屏幕
    QScreen *screen = QGuiApplication::primaryScreen();
    //获取屏幕几何信息
    QRect screenGeometry = screen->geometry();
    //获取全局坐标
    QPoint globalPos = mapToGlobal(QPoint(0, 0));
    //mapToGlobal(QPoint(0, 0)) 将窗口坐标转换为全局坐标

    if (!isExpanded) {
        ballPos = globalPos;
        // 展开
        if (globalPos.x() < screenGeometry.width() / 2) {
            // 在左半边屏幕，向右展开
            //如果在左下脚，向右上展开，
            setFixedSize(EXPANDED_SIZE);

            if(globalPos.y() > screenGeometry.height() / 2){
                move(globalPos.x(),
                     globalPos.y() - (EXPANDED_SIZE.height() - COLLAPSED_SIZE.height()));
            }
        } else {
            // 在右半边屏幕，向左展开
            QPoint newPos = globalPos;
            newPos.setX(globalPos.x() - (EXPANDED_SIZE.width() - COLLAPSED_SIZE.width()));
            move(newPos);
            setFixedSize(EXPANDED_SIZE);
            //如果右下角，向左下展开，否则向左上展开
            if(globalPos.y() > screenGeometry.height() / 2){
                move(globalPos.x() - (EXPANDED_SIZE.width() - COLLAPSED_SIZE.width()),
                     globalPos.y() - (EXPANDED_SIZE.height() - COLLAPSED_SIZE.height()));
            }
        }
        screenshotButton->show();
        chatButton->show();
        translateButton->show();
    } else {
        // 折叠
        QPoint newPos = pos();
        if (globalPos.x() >= screenGeometry.width() / 2) {
            newPos.setX(newPos.x() + (EXPANDED_SIZE.width() - COLLAPSED_SIZE.width()));
        }
        setFixedSize(COLLAPSED_SIZE);
       // move(newPos);
        move(ballPos);
        screenshotButton->hide();
        chatButton->hide();
        translateButton->hide();
    }

    isExpanded = !isExpanded;
    update();
}
void FloatingBall::handleSingleClick() {
    //单击事件
    isWaitingForDoubleClick = false;
}

void FloatingBall::paintEvent(QPaintEvent *) {
    //绘制事件
    QPainter painter(this);
    //设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    //设置画刷
    painter.setBrush(QColor(100, 100, 100, 180));
    //设置画笔
    painter.setPen(Qt::NoPen);
    
    //判断当前窗口大小，size()含义: 
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
    
    //显示菜单
    menu.exec(event->globalPos());
}
//切换显示/隐藏
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


QString FloatingBall::readStyleFromFile(const QString &filePath)
{
    QFile file(filePath);
    QString styleSheet;
    
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        styleSheet = stream.readAll();
        file.close();
    } else {
        qDebug() << "无法打开样式文件:" << filePath;
    }
    
    return styleSheet;
}


//全局快捷键
void FloatingBall::setupGlobalShortcuts()
{
    qDebug() << "设置全局快捷键";
    screenShortcut = new QHotkey(QKeySequence("Ctrl+Shift+P"),true, this);
    chatHotkey = new QHotkey(QKeySequence("Ctrl+Shift+C"), true, this);
    translateHotkey = new QHotkey(QKeySequence("Ctrl+Shift+T"), true, this);
    //截图快捷键
    connect(screenShortcut, &QHotkey::activated, [this]() {
        qDebug() << "触发截图键";
        screenshotManager = new ScreenshotManager(this);
        screenshotManager->startScreenshot();
    });

    //聊天快捷键
    connect(chatHotkey, &QHotkey::activated, [this]() {
        qDebug() << "触发聊天键";
        ChatManager* chatWindow = new ChatManager();
        chatWindow->setAttribute(Qt::WA_DeleteOnClose);
        chatWindow->show();
    });

    //翻译快捷键
    connect(translateHotkey, &QHotkey::activated, [this]() {
        qDebug() << "触发翻译键";
        QClipboard *clipboard = QApplication::clipboard();
        QString originalText = clipboard->text();
        this->copyTextToClipboard();
        // 给一点时间让复制操作完成
        QTimer::singleShot(100, [this, originalText]() {
            QString newText = QApplication::clipboard()->text();
            
            if (auto translateUI = AppManager::getInstance().getTranslateUI()) {
                translateUI->show();
                if (!newText.isEmpty()) {
                    translateUI->translateClipboardText();
                } else {
                    qDebug() << "没有检测到选中的文本";
                }
            } else {
                qDebug() << "获取翻译UI失败";
            }
            
            // 如果需要，可以恢复原始剪贴板内容
             QApplication::clipboard()->setText(originalText);
        });
    });
}

void FloatingBall::copyTextToClipboard()
{
#ifdef Q_OS_MAC
// 使用剪贴板来复制选中的文本
    // 使用 pbpaste 命令获取选中的文本
    QProcess process;
    process.start("pbpaste");
    process.waitForFinished();
    QString selectedText = QString::fromUtf8(process.readAllStandardOutput());

    qDebug() << "获取到的文本:" << selectedText;
#elif defined(Q_OS_WIN)
    // Windows 特定的复制命令
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('C', 0, 0, 0);
    keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
#elif defined(Q_OS_LINUX)
    // Linux 特定的复制命令
    system("xdotool key ctrl+c");
#endif
}

