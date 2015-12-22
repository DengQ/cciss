#include "ccissHandShake.h"
#include "widget.h"

/**
 * @brief ccissHandShake::ccissHandShake
 * @param sd
 */
ccissHandShake::ccissHandShake(qintptr sd){
    this->sd = sd;
    this->blockSize = 0;
    this->AIM =0;
    this->transType = 0;
    this->DLC = 0;
    bytesReceived =0;
    totalBytes =0;
}

//! ----------------------------------------------------------------------------
//!
//! \brief ccissHandShake::run
//!
void ccissHandShake::run(){
    qDebug()<<"Hand shake procedure start with"<<this->sd;

    this->soc = new QTcpSocket();
    if(!this->soc->setSocketDescriptor(this->sd)){
        return;
    }

    //connect(this->soc,SIGNAL(readyRead()),this,SLOT(lecture()),Qt::DirectConnection);
    connect(this->soc,SIGNAL(readyRead()),this,SLOT(readFromPiss()));
    connect(soc,SIGNAL(disconnected()),this,SLOT(disconnect()));

    exec();
}

void ccissHandShake::readFromPiss() {
    QDataStream in(soc);
    in.setVersion(QDataStream::Qt_5_5);
    //设置数据流版本，这里要和服务器端相同
    if(soc->bytesAvailable() >= (int)(sizeof(quint16)*3+48)){
        in >> AIM >> transType >> DLC >> CurrentTime;
        qDebug()<<"AIM"<<AIM;
        qDebug()<<"transType"<<transType;
        qDebug()<<"DLC"<<DLC;
        qDebug()<<"CurrentTime"<<CurrentTime;
    if(AIM == 1){
        qDebug()<<"connected whit piss "<<CurrentTime;

        if(transType == 0){
            qDebug()<<"get a QString message";
            if(soc->bytesAvailable() >= DLC ){
                in >> message;//message可以数字化
                qDebug()<<"Message:"<<message;
            }
        }

        if(transType ==1) {
            qDebug()<<"get a text";
//            if(soc->bytesAvailable() >=DLC){
//                in >> message;//message可以数字化
//                qDebug()<<"Data in :"<<message;
//            }

            if(soc->bytesAvailable() >= DLC ){
            localFile = new QFile("text1.txt");
               if(!localFile->open(QFile::WriteOnly))
                  {
                        qDebug() << "open file error!";
                        return;
                  }


            if(bytesReceived < DLC)
            {  //如果接收的数据小于总数据，那么写入文件
                bytesReceived += soc->bytesAvailable();
                inBlock = soc->readAll();
                qDebug()<<"Data in"<<inBlock;
                localFile->write(inBlock);
                inBlock.resize(0);
            }

            if(bytesReceived == DLC)

            { //接收数据完成时
                soc->close();
                localFile->close();
                qDebug()<<"get text ready";
            }
        }
    }
    }
}
}


//! ----------------------------------------------------------------------------
//!
//! \brief ccissHandShake::lecture
//!
void ccissHandShake::lecture(){

    QString msg(soc->readAll());
    qDebug()<<"Data in"<<msg;
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
    qDebug()<<sd<<" Disconnected";
    this->soc->deleteLater();
}
