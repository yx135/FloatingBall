#include "AppManager.h"

AppManager& AppManager::getInstance() {
    static AppManager instance;
    return instance;
}

void AppManager::initialize() {
    if (isInitialized) {
        qWarning() << "AppManager already initialized";
        return;
    }

    try {
        eventSystem = std::unique_ptr<EventSystem>(new EventSystem());
        configManager = std::unique_ptr<ConfigManager>(new ConfigManager());
        translateUI = std::unique_ptr<TranslateUI>(new TranslateUI());
        configManager->initialize();

        isInitialized = true;
        qInfo() << "AppManager initialized successfully";
    } catch (const std::exception& e) {
        qCritical() << "Failed to initialize AppManager:" << e.what();
        throw;
    }
}

void AppManager::cleanup() {
    if (!isInitialized) {
        return;
    }

    if (configManager) {
        configManager->cleanup();
    }
    
    configManager.reset();
    eventSystem.reset();
    translateUI.reset();
    isInitialized = false;
    qInfo() << "AppManager cleaned up successfully";
}
Translator* AppManager::getTranslator(QString translator_name) 
{
    if(translator_name=="deeplx")
    {
        translator = std::unique_ptr<Translator>(new Translator_deeplx());
    }
    return translator.get();
}
