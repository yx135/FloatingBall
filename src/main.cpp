#include <QApplication>
#include "ui/FloatingBall.h"
#include "core/AppManager.h"
#include <QDir>
int main(int argc, char *argv[]) {
    
    QApplication app(argc, argv);
    
    // 检查资源是否被正确编译
    QFile styleFile(":/resources/styles/style.qss");
    qDebug() << "样式表文件是否存在:" << styleFile.exists();
    qDebug() << "可用资源:" << QDir(":/").entryList();
    
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        QString style = stream.readAll();
        //qDebug() << "样式表内容:" << style;
        app.setStyleSheet(style);
        styleFile.close();
    } else {
        qWarning() << "无法加载样式表文件，错误:" << styleFile.errorString();
    }
    
    // 高DPI支持
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    // 初始化核心管理器
    AppManager::getInstance().initialize();
    
    FloatingBall w;
    w.show();
    
    return app.exec();
}
