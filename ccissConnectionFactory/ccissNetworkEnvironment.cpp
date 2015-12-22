#include "ccissNetworkEnvironment.h"
#include "widget.h"
/**
 * @brief ccissNetworkEnvironment::ccissNetworkEnvironment
 */
ccissNetworkEnvironment::ccissNetworkEnvironment(){

}

QHostAddress ccissNetworkEnvironment::selfDetctIP(){
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             return address;
    }
}

//! ----------------------------------------------------------------------------
//!
//! \brief ccissNetworkEnvironment::init
//!
void ccissNetworkEnvironment::init(QString module, int port){
    if(module == "self"){
        ModuleIpConfig* selfIpConfig = new ModuleIpConfig();
        selfIpConfig->setPort(port);
        selfIpConfig->setIpAddress(this->selfDetctIP());
        selfIpConfig->setName(module);
        this->modules.append(selfIpConfig);
    }
}

//! ----------------------------------------------------------------------------
//!
//! \brief ccissNetworkEnvironment::init
//! \param module
//! \param port
//! \param addr
//!
void ccissNetworkEnvironment::init(QString module, int port, QString addr){
    QHostAddress address(addr);

    if(module == "piss"){
        ModuleIpConfig* pissIpConfig = new ModuleIpConfig();
        pissIpConfig->setIpAddress(address);
        pissIpConfig->setPort(port);
        pissIpConfig->setName(module);
        this->modules.append(pissIpConfig);
    }
}



//! ----------------------------------------------------------------------------
//!
//! \brief ccissNetworkEnvironment::getIpAddress
//! \return
//!
QHostAddress ccissNetworkEnvironment::getIpAddressByModule(QString moduleName){
    for(unsigned char cpt = 0; cpt < this->modules.size(); cpt ++){
        if(this->modules.at(cpt)->getName() == moduleName){
            return this->modules.at(cpt)->getIpAddress();
        }
    }
}

//! ----------------------------------------------------------------------------
//!
//! \brief ccissNetworkEnvironment::getPort
//! \return
//!
int ccissNetworkEnvironment::getPortByModule(QString moduleName){
    for(unsigned char cpt = 0; cpt < this->modules.size(); cpt ++){
        if(this->modules.at(cpt)->getName() == moduleName){
            return this->modules.at(cpt)->getPort();
        }
    }
}

void ccissNetworkEnvironment::setModuleName(QString moduleName){
    this->moduleName = moduleName ;
}

QString ccissNetworkEnvironment::getModuleName(){
    return this->moduleName;
}
