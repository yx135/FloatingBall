QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FloatingBall
TEMPLATE = app
CONFIG += c++14

# macOS 特定配置
# macOS 特定配置
macx {
    QMAKE_MAC_SDK = macosx
    QMAKE_CXXFLAGS += -DQ_OS_MAC
}

SOURCES += \
    src/main.cpp \
    src/core/ModuleBase.cpp \
    src/core/AppManager.cpp \
    src/core/EventSystem.cpp \
    src/core/ConfigManager.cpp \
    src/ui/FloatingBall.cpp \
    src/ui/SettingsDialog.cpp \
    src/modules/screenshot/ScreenshotManager.cpp \
    src/modules/screenshot/ScreenshotSelector.cpp \
    src/modules/chat/ChatManager.cpp \
    src/modules/translate/TranslateUI.cpp \
    src/modules/translate/TranslateManager.cpp
HEADERS += \
    src/core/AppManager.h \
    src/core/ModuleBase.h \
    src/core/ConfigManager.h \
    src/core/EventSystem.h \
    src/ui/FloatingBall.h \
    src/ui/SettingsDialog.h \
    src/modules/screenshot/ScreenshotManager.h \
    src/modules/screenshot/ScreenshotSelector.h \
    src/modules/chat/ChatManager.h \
    src/modules/translate/TranslateUI.h \
    src/modules/translate/TranslateManager.h

INCLUDEPATH += \
    src \
    src/core \
    src/ui

# 资源文件
RESOURCES += resources.qrc

# 定义
DEFINES += QT_DEPRECATED_WARNINGS

# 添加以下配置以确保正确链接
CONFIG += sdk_no_version_check
