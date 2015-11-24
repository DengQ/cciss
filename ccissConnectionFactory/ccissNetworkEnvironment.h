#ifndef CCISSNETWORKENVIRONMENT_H
#define CCISSNETWORKENVIRONMENT_H

#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
#include "ModuleIpConfig.h"

class ccissNetworkEnvironment
{
public:
    ccissNetworkEnvironment();

    void init(QString module, int port);
    void init(QString module, int port, QString addr);

    QHostAddress selfDetctIP();
    void setModuleName(QString moduleName);

    QHostAddress getIpAddressByModule(QString moduleName);
    int getPortByModule(QString moduleName);
    QString getModuleName();


private:
    QString moduleName;
    QList <ModuleIpConfig *> modules;
};

#endif // CCISSNETWORKENVIRONMENT_H
