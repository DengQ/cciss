#include "ccissClient.h"

ccissClient::ccissClient(ccissNetworkEnvironment *networkEnvironment){
    this->networkEnvironment = networkEnvironment;

    this->connect(this, SIGNAL(connected()), this, SLOT(sendSelfInformation()));

}

//! --------------------------------------------------------
//!
//! \brief ccissClient::connectera
//! \param addr
//! \param port
//!
void ccissClient::connectera(){

    //! TODO : fetch port and addresse of the piss from networkEnvironment
    qDebug()<<this->networkEnvironment->getIpAddressByModule("piss")<<this->networkEnvironment->getPortByModule("piss");
    this->connectToHost(this->networkEnvironment->getIpAddressByModule("piss"),
                        this->networkEnvironment->getPortByModule("piss"));
}

//! --------------------------------------------------------
//!
//! \brief ccissClient::sendSelfInformation
//!
void ccissClient::sendSelfInformation(){

    this->write("cciss");

    if(this->waitForBytesWritten()){
        ccissTransmissionTask * transmissionTask = new ccissTransmissionTask(this);
        transmissionTask->start();
    }
}
