#include "ccissClient.h"
#include "widget.h"

ccissClient::ccissClient(ccissNetworkEnvironment *networkEnvironment){
    this->networkEnvironment = networkEnvironment;

    //this->connect(this, SIGNAL(connected()), this, SLOT(sendSelfInformation()));
    this->connect(this,SIGNAL(connected()),this,SLOT(startTransfer()));
}

//! --------------------------------------------------------
//!
//! \brief ccissClient::connectera
//! \param addr
//! \param port
//!
void ccissClient::connectera(){

    //! TODO : fetch port and addresse of the piss from networkEnvironment
    qDebug()<<"Connecting To"<<this->networkEnvironment->getIpAddressByModule("piss")<<this->networkEnvironment->getPortByModule("piss");
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

void ccissClient::startTransfer(){
    QByteArray block; //用于暂存我们要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);
    //使用数据流写入数据
    out.setVersion(QDataStream::Qt_5_5);
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString CurrentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    qDebug()<<"Data out CurrentTime"<<CurrentTime;
    //设置数据流的版本，客户端和服务器端使用的版本要相同
    quint16 ccissTopiss =0;
    quint16 transType = 0;//定义string类=0
    //message = tr("I Love You");
    message = tr("jpg");
    quint16 DLC = (message.size()+2)*2;
    //quint16 timeSize=0;
    //out << quint16(0)<< quint16(0)<< quint16(0)<<CurrentTime;
    //out.device()->seek(0);
    //timeSize = (quint16)(block.size() - sizeof(quint16)*3);
    //qDebug()<<"timesize"<<timeSize;
    //quint16 CurrentTimeSize = (CurrentTime.size()+2)*2;
    //qDebug()<<"CurrentTimeSize"<<CurrentTimeSize;
    out << ccissTopiss  <<  transType  << DLC << CurrentTime<<message;

           //<< DLC<<message;
//    quint16 CurrentTimeSize = CurrentTime.size();
//    qDebug()<<"Data out CurrentTimeSize"<<CurrentTimeSize;
//    out.device()->seek(0);
    //out<<(quint16) (block.size() - sizeof(quint16));
    //connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));
    qDebug()<<"out :"<<block;
    this->write(block);
    this->waitForBytesWritten(-1);
    qDebug()<<tr("send %1 successful!!!").arg(message);
    qDebug()<<"ccissTransmissionTask Start";
    //发送数据成功后，显示提示
    //this->close();
}
