#include "ccissTransmissionTask.h"

ccissTransmissionTask::ccissTransmissionTask(QTcpSocket *soc)
{
    this->soc = soc;
}

void ccissTransmissionTask::run(){

    while(true){
        qDebug()<<"ccissTransmissionTask";

        //! TODO

        msleep(500);
    }
}
