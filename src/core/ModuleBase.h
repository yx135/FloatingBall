#pragma once
#include <QObject>

class ModuleBase : public QObject {
    Q_OBJECT
public:
    explicit ModuleBase(QObject* parent = nullptr);
    virtual ~ModuleBase();
    
    virtual void initialize() = 0;
    virtual void cleanup() = 0;
};