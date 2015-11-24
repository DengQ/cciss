#ifndef MODULEIPCONFIG_H
#define MODULEIPCONFIG_H

#include <QHostAddress>


class ModuleIpConfig
{
public:
    ModuleIpConfig();

    QHostAddress getIpAddress();
    int getPort();
    QString getName();

    void setIpAddress(QHostAddress addr);
    void setPort(int port);
    void setName(QString name);

private:
    QHostAddress addr;
    int port;
    QString name;
};

#endif // MODULEIPCONFIG_H
