#include "ScreenshotManager.h"
#include "ScreenshotSelector.h"
#include <QScreen>
#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

ScreenshotManager::ScreenshotManager(QObject* parent) : ModuleBase(parent) {
}

void ScreenshotManager::initialize() {
}

void ScreenshotManager::cleanup() {
}

QWidget* ScreenshotManager::getMainWindow() const {
    return qobject_cast<QWidget*>(parent());
}

void ScreenshotManager::startScreenshot() {
    qDebug() << "takeScreenshot function started";

    this->getMainWindow()->hide();

    QTimer::singleShot(500, this, [this]() {
        qDebug() << "Timer callback started";
        QScreen *screen = QGuiApplication::primaryScreen();

        if (screen) {
            ScreenshotSelector *selector = new ScreenshotSelector();
            selector->setGeometry(screen->geometry());

            connect(selector, &ScreenshotSelector::screenshotTaken, this, [this, screen, selector](const QRect &selectionRect) {
                if (!selectionRect.isNull()) {
                    QPixmap screenshot = screen->grabWindow(0, selectionRect.x(), selectionRect.y(),
                                                            selectionRect.width(), selectionRect.height());
                    // 将截图添加到剪贴板
            QClipboard *clipboard = QGuiApplication::clipboard();
            clipboard->setPixmap(screenshot);
            qDebug() << "截图已复制到剪贴板";

                    QString fileName = QFileDialog::getSaveFileName(nullptr, "保存截图",
                                                                    QDir::homePath() + "/screenshot.png",
                                                                    "Images (*.png *.jpg)");
                    if (!fileName.isEmpty()) {
                        if (screenshot.save(fileName)) {
                            qDebug() << "Screenshot saved successfully to:" << fileName;
                        } else {
                            qDebug() << "Failed to save screenshot to:" << fileName;
                        }
                    }
                }
                this->getMainWindow()->show();
                selector->deleteLater();
            });

            selector->show();
        }
    });

    qDebug() << "takeScreenshot function ended";
}
