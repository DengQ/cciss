#include "ccissReceptionTask.h"
#include <QDebug>

/**
 * @brief ccissReceptionTask::ccissReceptionTask
 */
ccissReceptionTask::ccissReceptionTask(QTcpSocket *soc){
    this->soc = soc;
}

//!
//! \brief ccissReceptionTask::run
//!
void ccissReceptionTask::run(){
    while(true){
        qDebug()<<"receptionTask";
        msleep(500);
    }
}
