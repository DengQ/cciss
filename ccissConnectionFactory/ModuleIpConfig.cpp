#include "ModuleIpConfig.h"
#include "widget.h"
ModuleIpConfig::ModuleIpConfig()
{

}

QHostAddress ModuleIpConfig::getIpAddress(){
    return this->addr;
}

void ModuleIpConfig::setIpAddress(QHostAddress addr){
    this->addr = addr;
}

void ModuleIpConfig::setPort(int port){
    this->port = port;
}

int ModuleIpConfig::getPort(){
    return this->port;
}

QString ModuleIpConfig::getName(){
    return this->name;
}

void ModuleIpConfig::setName(QString name){
    this->name = name;
}
