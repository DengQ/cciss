#include "ccissServer.h"
#include "ccissHandShake.h"
#include "widget.h"
ccissServer::ccissServer(ccissNetworkEnvironment *networkEnvironment)
{
    this->networkEnvironment = networkEnvironment;
}

//! -----------------------------------------------------
//!
//! \brief ccissServer::launchServer
//! \param port
//!
void ccissServer::launchServer(){
    if(!this->listen(QHostAddress::Any, this->networkEnvironment->getPortByModule("self"))){
        qDebug()<<"Couldn't launch server";
    }
    else{
        qDebug()<<"waiting for connection from port "<<this->networkEnvironment->getPortByModule("self");
    }
}

//! -----------------------------------------------------
//!
//! \brief ccissServer::incomingConnection
//! \param socket
//!
void ccissServer::incomingConnection(qintptr socket){
    qDebug()<<"client"<<socket<<"arrived";
    ccissHandShake *handShakeTask = new ccissHandShake(socket);
    this->connect(handShakeTask, SIGNAL(finished()), handShakeTask, SLOT(deleteLater()));
    handShakeTask->start();
}
