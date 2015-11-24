#include "ccissHandShake.h"

/**
 * @brief ccissHandShake::ccissHandShake
 * @param sd
 */
ccissHandShake::ccissHandShake(qintptr sd){
    this->sd = sd;
}

//! ----------------------------------------------------------------------------
//!
//! \brief ccissHandShake::run
//!
void ccissHandShake::run(){
    qDebug()<<"Hand shake procedure start"<<this->sd;

    this->soc = new QTcpSocket();
    if(!this->soc->setSocketDescriptor(this->sd)){
        return;
    }

    connect(this->soc,SIGNAL(readyRead()),this,SLOT(lecture()),Qt::DirectConnection);
    connect(soc,SIGNAL(disconnected()),this,SLOT(disconnect()));

    exec();
}

//! ----------------------------------------------------------------------------
//!
//! \brief ccissHandShake::lecture
//!
void ccissHandShake::lecture(){
    qDebug()<<"cissHandShake::lecture";

    QString msg(soc->readAll());

    if(msg.contains("piss")){
        ccissReceptionTask *receptionTask = new ccissReceptionTask(this->soc);
        receptionTask->start();
    }
}

//! ----------------------------------------------------------------------------
//!
//! \brief ccissHandShake::disconnect
//!
void ccissHandShake::disconnect(){
    this->soc->deleteLater();
}
