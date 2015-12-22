#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //! initialisation of the parameter concern about the network environment
    networkEnvironment = new ccissNetworkEnvironment();
    networkEnvironment->init("self", 12345);
    networkEnvironment->init("piss", 10703, "127.0.0.1");
    //! create tcp server to wait for the connexion from igss_platform part
    server = new ccissServer(networkEnvironment);
    //! create tcp client to connect igss_platform
    ccissToPiss = new ccissClient(networkEnvironment);

//    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString CurrentTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
//    qDebug()<<"Data out CurrentTime"<<CurrentTime<<"size"<<CurrentTime.size()<<"sizeof(quint16) size"<<sizeof(quint16);

    tcpServer = new QTcpServer();
    client = new QTcpSocket();

    this->constructIHM();
    this->setConnections();

    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    fileSendReady = false;
    msgSendReady = false;
    tcpClient = new QTcpSocket(this);

    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    this->sendButton->setEnabled(false);
//    this->hostLineEdit->setText("127.0.0.1");
//    this->portLineEdit->setText("8888");

    connect(tcpClient,SIGNAL(connected()),this,SLOT(startTransferFile()));
    //当连接服务器成功时，发出connected()信号，我们开始传送文件
    connect(tcpClient,SIGNAL(connected()),this,SLOT(startTransferMsg()));
    //当连接服务器成功时，发出connected()信号，我们开始传送消息
    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    //当有数据发送成功时，我们更新进度条
}

void Widget::startServer(){

    this->startServerPushButton->setEnabled(false);
    this->textLabel->setText(textLabel->text()+"start server\n");
//    if(!tcpServer->listen(QHostAddress::Any,8888)){
//        qDebug()<<"Could not start server";
//    }
//    else {
//        qDebug()<<"Listen to port";
//    }

//    connect(this->tcpServer, SIGNAL(newConnection()),this, SLOT(acceptConnection()));

    server->launchServer();

    ccissToPiss->connectera();
}

void Widget::acceptConnection(){

    client = tcpServer->nextPendingConnection();

    connect(this->client, SIGNAL(readyRead()),this, SLOT(startRead()));
}

void Widget::startRead(){
    QByteArray Data = client->readAll();
    qDebug()<<"Data in: "<<Data;

    client->write("I am Server");
    client->waitForBytesWritten(-1);
}

Widget::~Widget()
{

}

void Widget::setConnections(){
    connect(this->sendButton,SIGNAL(clicked()),this,SLOT(fileSend()));
    connect(this->openButton,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(this->closeButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(this->msgPushButton,SIGNAL(clicked()),this,SLOT(msgSend()));
    connect(this->startServerPushButton,SIGNAL(clicked()),this,SLOT(startServer()));
}

void Widget::msgSend(){
    msgSendReady = true;
    bytesWritten = 0;
    //初始化已发送字节为0
    this->stateLabel->setText(QString::fromLocal8Bit("连接中..."));
    tcpClient->connectToHost(QHostAddress(this->remoteIPLineEdit->text()),this->remotePortLineEdit->text().toInt());//连接
}

void Widget::fileSend(){
    fileSendReady = true;
    this->sendButton->setEnabled(false);
    bytesWritten = 0;
    //初始化已发送字节为0
    this->stateLabel->setText(QString::fromLocal8Bit("连接中..."));
    tcpClient->connectToHost(QHostAddress(this->remoteIPLineEdit->text()),this->remotePortLineEdit->text().toInt());//连接

}
void Widget::startTransferMsg(){
    if(msgSendReady){
//    localFile = new QFile(fileName);
//    if(!localFile->open(QFile::ReadOnly))
//    {
//        qDebug() << "open file error!";
//        return;
//    }
    msg = this->msgLineEdit->text();
    totalBytes = msg.size();
    //文件总大小
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    //QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    //fileName.lastIndexOf('/')返回的是从首字符开始 到最后一个 / 的位数
    //fileName.right(8) 意思就是 从路径右面开始 到第八位数的字符串
    sendOut << qint64(0)<<qint8(0)<<qint64(0)<<msg;
    //依次写入总大小信息空间，发送类型信息空间
    totalBytes += outBlock.size();
    //这里的总大小是消息大小消息+类型信息的总和
    sendOut.device()->seek(0);
    //回到字节流起始位置
    msgtype = 2;
    sendOut<<totalBytes<<msgtype<<qint64((outBlock.size() - sizeof(qint64)*2-sizeof(qint8)));
    //totalBytes是文件总大小，即两个quint64的大小+文件名+文件实际内容的大小
    //qint64((outBlock.size() - sizeof(qint64)*2))得到的是文件名大小
    //为了保证在客户端能接收到完整的文件，我们都在数据流的最开始写入完整文件的大小信息，
    //这样客户端就可以根据大小信息来判断是否接受到了完整的文件。而在服务器端，我们在发送数据时就要首先发送实际文件的大小信息，
    //但是，文件的大小一开始是无法预知的，所以我们先使用了out<<(quint16) 0;在block的开始添加了一个quint16大小的空间，
    //也就是两字节的空间，它用于后面放置文件的大小信息。然后out<<tr(“hello Tcp!!!”);输入实际的文件，这里是字符串。
    //当文件输入完成后我们在使用out.device()->seek(0);返回到block的开始，加入实际的文件大小信息，也就是后面的代码，
    //它是实际文件的大小：out<<(quint16) (block.size() – sizeof(quint16));
    tcpClient->write(outBlock);
    //发送完头数据后剩余数据的大小，即文件实际内容的大小
//    this->stateLabel->setText(QString::fromLocal8Bit("已连接"));
    outBlock.resize(0);
    qDebug()<<"#####totalBytes"<<totalBytes;
    this->stateLabel->setText(this->msgLineEdit->text()+" is Send");
    tcpClient->close();
    }
}

void Widget::startTransferFile(){
    if(fileSendReady){
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = localFile->size();
    //文件总大小
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    //fileName.lastIndexOf('/')返回的是从首字符开始 到最后一个 / 的位数
    //fileName.right(8) 意思就是 从路径右面开始 到第八位数的字符串
    sendOut << qint64(0) << qint8(0)<<qint64(0) << currentFileName;
    //依次写入总大小信息空间，文件名大小信息空间，文件名---仅是文件名不含路径，没必要含路径
    totalBytes += outBlock.size();
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    sendOut.device()->seek(0);
    //回到字节流起始位置
    msgtype = 1;
    sendOut<<totalBytes<<msgtype<<qint64((outBlock.size() - sizeof(qint64)*2-sizeof(qint8)));
    //totalBytes是文件总大小，即两个quint64的大小+文件名+文件实际内容的大小
    //qint64((outBlock.size() - sizeof(qint64)*2))得到的是文件名大小
    //为了保证在客户端能接收到完整的文件，我们都在数据流的最开始写入完整文件的大小信息，
    //这样客户端就可以根据大小信息来判断是否接受到了完整的文件。而在服务器端，我们在发送数据时就要首先发送实际文件的大小信息，
    //但是，文件的大小一开始是无法预知的，所以我们先使用了out<<(quint16) 0;在block的开始添加了一个quint16大小的空间，
    //也就是两字节的空间，它用于后面放置文件的大小信息。然后out<<tr(“hello Tcp!!!”);输入实际的文件，这里是字符串。
    //当文件输入完成后我们在使用out.device()->seek(0);返回到block的开始，加入实际的文件大小信息，也就是后面的代码，
    //它是实际文件的大小：out<<(quint16) (block.size() – sizeof(quint16));
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    //发送完头数据后剩余数据的大小，即文件实际内容的大小
    this->stateLabel->setText(QString::fromLocal8Bit("已连接"));
    outBlock.resize(0);
    qDebug()<<"#####"<<totalBytes;
    }
}

void Widget::updateClientProgress(qint64 numBytes){
    if(fileSendReady){
    qDebug()<<QString::fromLocal8Bit("#######已发送：")<<bytesWritten<<QString::fromLocal8Bit("剩余：")<<bytesToWrite;
    bytesWritten += (int)numBytes;
    //已经发送数据的大小
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
      //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
      //就发送剩余数据的大小
        bytesToWrite -= (int)tcpClient->write(outBlock);
       //发送完一次数据后还剩余数据的大小
        outBlock.resize(0);
       //清空发送缓冲区
    }
    else
    {
        localFile->close(); //如果没有发送任何数据，则关闭文件
    }
    this->sendProgressBar->setMaximum(totalBytes);
    this->sendProgressBar->setValue(bytesWritten);
    //更新进度条
    if(bytesWritten == totalBytes) //发送完毕
    {
        this->stateLabel->setText(QString::fromLocal8Bit("传送文件 %1 成功").arg(fileName));
        localFile->close();
        tcpClient->close();
    }
    }
//    if(msgSendReady){
//        qDebug()<<QString::fromLocal8Bit("#######已发送：")<<bytesWritten<<QString::fromLocal8Bit("剩余：")<<bytesToWrite;
//        bytesWritten += (int)numBytes;
//        //已经发送数据的大小
//        if(bytesToWrite > 0) //如果已经发送了数据
//        {
//            outBlock = msg.toLatin1();
//            bytesToWrite -= (int)tcpClient->write(outBlock);
//           //发送完一次数据后还剩余数据的大小
//            outBlock.resize(0);
//           //清空发送缓冲区
//        }
//        else
//        {
//            qDebug()<<"no message send";//如果没有发送任何数据，则关闭文件
//        }
//        if(bytesWritten == totalBytes) //发送完毕
//        {
//            this->stateLabel->setText(this->msgLineEdit->text()+" is Send");
//            tcpClient->close();
//        }
//    }
}

void Widget::displayError(QAbstractSocket::SocketError){
    qDebug() << tcpClient->errorString();
    tcpClient->close();
    this->sendProgressBar->reset();
    this->stateLabel->setText(QString::fromLocal8Bit("客户端就绪"));
    this->sendButton->setEnabled(true);
}

void Widget::openFile(){
    fileName = QFileDialog::getOpenFileName(this);//打开或保存文件对话框
    if(!fileName.isEmpty())
    {
        QMessageBox::information(NULL, tr("fileName"), tr("You selected ") + fileName);
        this->sendButton->setEnabled(true);
        this->stateLabel->setText(QString::fromLocal8Bit("打开文件 %1 成功").arg(fileName));
    }
    else{
        QMessageBox::information(NULL, ("fileName"), tr("You didn't select any files."));
    }
}



void Widget::constructIHM(){

    this->homeIPLabel = new QLabel(QString::fromLocal8Bit("本机IP"));
    this->homeIPLineEdit = new QLineEdit(QString::fromLocal8Bit("127.0.0.1"));
    this->homePortLabel = new QLabel(QString::fromLocal8Bit("监听端口"));
    this->homePortLineEdit = new QLineEdit(QString::fromLocal8Bit("12345"));

    this->homeAdressWidget = new QWidget();
    this->homeAdressBarLayout = new QHBoxLayout(homeAdressWidget);
    this->homeAdressBarLayout->addWidget(homeIPLabel);
    this->homeAdressBarLayout->addWidget(homeIPLineEdit);
    this->homeAdressBarLayout->addWidget(homePortLabel);
    this->homeAdressBarLayout->addWidget(homePortLineEdit);

    this->remoteIPLabel = new QLabel(QString::fromLocal8Bit("远程IP"));
    this->remoteIPLineEdit = new QLineEdit(QString::fromLocal8Bit("127.0.0.1"));
    this->remotePortLabel = new QLabel(QString::fromLocal8Bit("监听端口"));
    this->remotePortLineEdit = new QLineEdit(QString::fromLocal8Bit("8888"));

    this->remoteAdressWidget = new QWidget();
    this->remoteAdressBarLayout = new QHBoxLayout(remoteAdressWidget);
    this->remoteAdressBarLayout->addWidget(remoteIPLabel);
    this->remoteAdressBarLayout->addWidget(remoteIPLineEdit);
    this->remoteAdressBarLayout->addWidget(remotePortLabel);
    this->remoteAdressBarLayout->addWidget(remotePortLineEdit);

    this->reciveProgressLabel = new QLabel(QString::fromLocal8Bit("接收进度条"));
    this->reciveProgressBar = new QProgressBar();

    this->reciveProgressWidget = new QWidget();
    this->reciveProgressLayout = new QHBoxLayout(reciveProgressWidget);
    this->reciveProgressLayout->addWidget(reciveProgressLabel);
    this->reciveProgressLayout->addWidget(reciveProgressBar);

    this->sendProgressLabel = new QLabel(QString::fromLocal8Bit("发送进度条"));
    this->sendProgressBar = new QProgressBar();

    this->sendProgressWidget = new QWidget();
    this->sendProgressLayout = new QHBoxLayout(sendProgressWidget);
    this->sendProgressLayout->addWidget(sendProgressLabel);
    this->sendProgressLayout->addWidget(sendProgressBar);

    //this->startButton = new QPushButton(QString::fromLocal8Bit("监听"));
    this->openButton = new QPushButton(QString::fromLocal8Bit("打开"));
    this->sendButton = new QPushButton(QString::fromLocal8Bit("发送"));
    this->closeButton = new QPushButton(QString::fromLocal8Bit("关闭"));

    this->startBarWidget = new QWidget();
    this->startWidgetLayout = new QHBoxLayout(startBarWidget);
    //this->startWidgetLayout->addWidget(startButton);
    this->startWidgetLayout->addWidget(openButton);
    this->startWidgetLayout->addWidget(sendButton);
    this->startWidgetLayout->addWidget(closeButton);

    this->msgLabel = new QLabel(QString::fromLocal8Bit("消息"));
    this->msgLineEdit = new QLineEdit(QString::fromLocal8Bit("cciss"));
    this->msgPushButton = new QPushButton(QString::fromLocal8Bit("发送"));

    this->msgWidget = new QWidget();
    this->msgWidgetLayout = new QHBoxLayout(msgWidget);
    this->msgWidgetLayout->addWidget(msgLabel);
    this->msgWidgetLayout->addWidget(msgLineEdit);
    this->msgWidgetLayout->addWidget(msgPushButton);

    this->stateLabel = new QLabel(QString::fromLocal8Bit("准备就绪"));

    this->ccissWidget = new QWidget();
    this->ccissWidgetLayout = new QVBoxLayout(ccissWidget);
    this->ccissWidgetLayout->addWidget(homeAdressWidget);
    this->ccissWidgetLayout->addWidget(remoteAdressWidget);
    this->ccissWidgetLayout->addWidget(reciveProgressWidget);
    this->ccissWidgetLayout->addWidget(sendProgressWidget);
    this->ccissWidgetLayout->addWidget(startBarWidget);
    this->ccissWidgetLayout->addWidget(msgWidget);
    this->ccissWidgetLayout->addWidget(stateLabel);

    this->startServerPushButton = new QPushButton(QString::fromLocal8Bit("Start Server"));
    this->textLabel = new QLabel(QString::fromLocal8Bit("准备就绪\n"));
    this->textLabel->setFixedWidth(150);
    this->textLabel->setAlignment(Qt::AlignTop);
    this->textLabel->setStyleSheet("border: 1px solid skyblue");

    this->textWidget = new QWidget();
    this->textVboxLayout = new QVBoxLayout(textWidget);
    this->textVboxLayout->addWidget(startServerPushButton);
    this->textVboxLayout->addWidget(textLabel);

    this->ccissWidgetLayoutAll = new QHBoxLayout(this);
    this->ccissWidgetLayoutAll->addWidget(ccissWidget);
    this->ccissWidgetLayoutAll->addWidget(textWidget);
}

