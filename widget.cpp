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
    //�����ӷ������ɹ�ʱ������connected()�źţ����ǿ�ʼ�����ļ�
    connect(tcpClient,SIGNAL(connected()),this,SLOT(startTransferMsg()));
    //�����ӷ������ɹ�ʱ������connected()�źţ����ǿ�ʼ������Ϣ
    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    //�������ݷ��ͳɹ�ʱ�����Ǹ��½�����
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
    //��ʼ���ѷ����ֽ�Ϊ0
    this->stateLabel->setText(QString::fromLocal8Bit("������..."));
    tcpClient->connectToHost(QHostAddress(this->remoteIPLineEdit->text()),this->remotePortLineEdit->text().toInt());//����
}

void Widget::fileSend(){
    fileSendReady = true;
    this->sendButton->setEnabled(false);
    bytesWritten = 0;
    //��ʼ���ѷ����ֽ�Ϊ0
    this->stateLabel->setText(QString::fromLocal8Bit("������..."));
    tcpClient->connectToHost(QHostAddress(this->remoteIPLineEdit->text()),this->remotePortLineEdit->text().toInt());//����

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
    //�ļ��ܴ�С
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    //QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    //fileName.lastIndexOf('/')���ص��Ǵ����ַ���ʼ �����һ�� / ��λ��
    //fileName.right(8) ��˼���� ��·�����濪ʼ ���ڰ�λ�����ַ���
    sendOut << qint64(0)<<qint8(0)<<qint64(0)<<msg;
    //����д���ܴ�С��Ϣ�ռ䣬����������Ϣ�ռ�
    totalBytes += outBlock.size();
    //������ܴ�С����Ϣ��С��Ϣ+������Ϣ���ܺ�
    sendOut.device()->seek(0);
    //�ص��ֽ�����ʼλ��
    msgtype = 2;
    sendOut<<totalBytes<<msgtype<<qint64((outBlock.size() - sizeof(qint64)*2-sizeof(qint8)));
    //totalBytes���ļ��ܴ�С��������quint64�Ĵ�С+�ļ���+�ļ�ʵ�����ݵĴ�С
    //qint64((outBlock.size() - sizeof(qint64)*2))�õ������ļ�����С
    //Ϊ�˱�֤�ڿͻ����ܽ��յ��������ļ������Ƕ������������ʼд�������ļ��Ĵ�С��Ϣ��
    //�����ͻ��˾Ϳ��Ը��ݴ�С��Ϣ���ж��Ƿ���ܵ����������ļ������ڷ������ˣ������ڷ�������ʱ��Ҫ���ȷ���ʵ���ļ��Ĵ�С��Ϣ��
    //���ǣ��ļ��Ĵ�Сһ��ʼ���޷�Ԥ֪�ģ�����������ʹ����out<<(quint16) 0;��block�Ŀ�ʼ�����һ��quint16��С�Ŀռ䣬
    //Ҳ�������ֽڵĿռ䣬�����ں�������ļ��Ĵ�С��Ϣ��Ȼ��out<<tr(��hello Tcp!!!��);����ʵ�ʵ��ļ����������ַ�����
    //���ļ�������ɺ�������ʹ��out.device()->seek(0);���ص�block�Ŀ�ʼ������ʵ�ʵ��ļ���С��Ϣ��Ҳ���Ǻ���Ĵ��룬
    //����ʵ���ļ��Ĵ�С��out<<(quint16) (block.size() �C sizeof(quint16));
    tcpClient->write(outBlock);
    //������ͷ���ݺ�ʣ�����ݵĴ�С�����ļ�ʵ�����ݵĴ�С
//    this->stateLabel->setText(QString::fromLocal8Bit("������"));
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
    //�ļ��ܴ�С
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    //fileName.lastIndexOf('/')���ص��Ǵ����ַ���ʼ �����һ�� / ��λ��
    //fileName.right(8) ��˼���� ��·�����濪ʼ ���ڰ�λ�����ַ���
    sendOut << qint64(0) << qint8(0)<<qint64(0) << currentFileName;
    //����д���ܴ�С��Ϣ�ռ䣬�ļ�����С��Ϣ�ռ䣬�ļ���---�����ļ�������·����û��Ҫ��·��
    totalBytes += outBlock.size();
    //������ܴ�С���ļ�����С����Ϣ��ʵ���ļ���С���ܺ�
    sendOut.device()->seek(0);
    //�ص��ֽ�����ʼλ��
    msgtype = 1;
    sendOut<<totalBytes<<msgtype<<qint64((outBlock.size() - sizeof(qint64)*2-sizeof(qint8)));
    //totalBytes���ļ��ܴ�С��������quint64�Ĵ�С+�ļ���+�ļ�ʵ�����ݵĴ�С
    //qint64((outBlock.size() - sizeof(qint64)*2))�õ������ļ�����С
    //Ϊ�˱�֤�ڿͻ����ܽ��յ��������ļ������Ƕ������������ʼд�������ļ��Ĵ�С��Ϣ��
    //�����ͻ��˾Ϳ��Ը��ݴ�С��Ϣ���ж��Ƿ���ܵ����������ļ������ڷ������ˣ������ڷ�������ʱ��Ҫ���ȷ���ʵ���ļ��Ĵ�С��Ϣ��
    //���ǣ��ļ��Ĵ�Сһ��ʼ���޷�Ԥ֪�ģ�����������ʹ����out<<(quint16) 0;��block�Ŀ�ʼ�����һ��quint16��С�Ŀռ䣬
    //Ҳ�������ֽڵĿռ䣬�����ں�������ļ��Ĵ�С��Ϣ��Ȼ��out<<tr(��hello Tcp!!!��);����ʵ�ʵ��ļ����������ַ�����
    //���ļ�������ɺ�������ʹ��out.device()->seek(0);���ص�block�Ŀ�ʼ������ʵ�ʵ��ļ���С��Ϣ��Ҳ���Ǻ���Ĵ��룬
    //����ʵ���ļ��Ĵ�С��out<<(quint16) (block.size() �C sizeof(quint16));
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    //������ͷ���ݺ�ʣ�����ݵĴ�С�����ļ�ʵ�����ݵĴ�С
    this->stateLabel->setText(QString::fromLocal8Bit("������"));
    outBlock.resize(0);
    qDebug()<<"#####"<<totalBytes;
    }
}

void Widget::updateClientProgress(qint64 numBytes){
    if(fileSendReady){
    qDebug()<<QString::fromLocal8Bit("#######�ѷ��ͣ�")<<bytesWritten<<QString::fromLocal8Bit("ʣ�ࣺ")<<bytesToWrite;
    bytesWritten += (int)numBytes;
    //�Ѿ��������ݵĴ�С
    if(bytesToWrite > 0) //����Ѿ�����������
    {
        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
      //ÿ�η���loadSize��С�����ݣ���������Ϊ4KB�����ʣ������ݲ���4KB��
      //�ͷ���ʣ�����ݵĴ�С
        bytesToWrite -= (int)tcpClient->write(outBlock);
       //������һ�����ݺ�ʣ�����ݵĴ�С
        outBlock.resize(0);
       //��շ��ͻ�����
    }
    else
    {
        localFile->close(); //���û�з����κ����ݣ���ر��ļ�
    }
    this->sendProgressBar->setMaximum(totalBytes);
    this->sendProgressBar->setValue(bytesWritten);
    //���½�����
    if(bytesWritten == totalBytes) //�������
    {
        this->stateLabel->setText(QString::fromLocal8Bit("�����ļ� %1 �ɹ�").arg(fileName));
        localFile->close();
        tcpClient->close();
    }
    }
//    if(msgSendReady){
//        qDebug()<<QString::fromLocal8Bit("#######�ѷ��ͣ�")<<bytesWritten<<QString::fromLocal8Bit("ʣ�ࣺ")<<bytesToWrite;
//        bytesWritten += (int)numBytes;
//        //�Ѿ��������ݵĴ�С
//        if(bytesToWrite > 0) //����Ѿ�����������
//        {
//            outBlock = msg.toLatin1();
//            bytesToWrite -= (int)tcpClient->write(outBlock);
//           //������һ�����ݺ�ʣ�����ݵĴ�С
//            outBlock.resize(0);
//           //��շ��ͻ�����
//        }
//        else
//        {
//            qDebug()<<"no message send";//���û�з����κ����ݣ���ر��ļ�
//        }
//        if(bytesWritten == totalBytes) //�������
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
    this->stateLabel->setText(QString::fromLocal8Bit("�ͻ��˾���"));
    this->sendButton->setEnabled(true);
}

void Widget::openFile(){
    fileName = QFileDialog::getOpenFileName(this);//�򿪻򱣴��ļ��Ի���
    if(!fileName.isEmpty())
    {
        QMessageBox::information(NULL, tr("fileName"), tr("You selected ") + fileName);
        this->sendButton->setEnabled(true);
        this->stateLabel->setText(QString::fromLocal8Bit("���ļ� %1 �ɹ�").arg(fileName));
    }
    else{
        QMessageBox::information(NULL, ("fileName"), tr("You didn't select any files."));
    }
}



void Widget::constructIHM(){

    this->homeIPLabel = new QLabel(QString::fromLocal8Bit("����IP"));
    this->homeIPLineEdit = new QLineEdit(QString::fromLocal8Bit("127.0.0.1"));
    this->homePortLabel = new QLabel(QString::fromLocal8Bit("�����˿�"));
    this->homePortLineEdit = new QLineEdit(QString::fromLocal8Bit("12345"));

    this->homeAdressWidget = new QWidget();
    this->homeAdressBarLayout = new QHBoxLayout(homeAdressWidget);
    this->homeAdressBarLayout->addWidget(homeIPLabel);
    this->homeAdressBarLayout->addWidget(homeIPLineEdit);
    this->homeAdressBarLayout->addWidget(homePortLabel);
    this->homeAdressBarLayout->addWidget(homePortLineEdit);

    this->remoteIPLabel = new QLabel(QString::fromLocal8Bit("Զ��IP"));
    this->remoteIPLineEdit = new QLineEdit(QString::fromLocal8Bit("127.0.0.1"));
    this->remotePortLabel = new QLabel(QString::fromLocal8Bit("�����˿�"));
    this->remotePortLineEdit = new QLineEdit(QString::fromLocal8Bit("8888"));

    this->remoteAdressWidget = new QWidget();
    this->remoteAdressBarLayout = new QHBoxLayout(remoteAdressWidget);
    this->remoteAdressBarLayout->addWidget(remoteIPLabel);
    this->remoteAdressBarLayout->addWidget(remoteIPLineEdit);
    this->remoteAdressBarLayout->addWidget(remotePortLabel);
    this->remoteAdressBarLayout->addWidget(remotePortLineEdit);

    this->reciveProgressLabel = new QLabel(QString::fromLocal8Bit("���ս�����"));
    this->reciveProgressBar = new QProgressBar();

    this->reciveProgressWidget = new QWidget();
    this->reciveProgressLayout = new QHBoxLayout(reciveProgressWidget);
    this->reciveProgressLayout->addWidget(reciveProgressLabel);
    this->reciveProgressLayout->addWidget(reciveProgressBar);

    this->sendProgressLabel = new QLabel(QString::fromLocal8Bit("���ͽ�����"));
    this->sendProgressBar = new QProgressBar();

    this->sendProgressWidget = new QWidget();
    this->sendProgressLayout = new QHBoxLayout(sendProgressWidget);
    this->sendProgressLayout->addWidget(sendProgressLabel);
    this->sendProgressLayout->addWidget(sendProgressBar);

    //this->startButton = new QPushButton(QString::fromLocal8Bit("����"));
    this->openButton = new QPushButton(QString::fromLocal8Bit("��"));
    this->sendButton = new QPushButton(QString::fromLocal8Bit("����"));
    this->closeButton = new QPushButton(QString::fromLocal8Bit("�ر�"));

    this->startBarWidget = new QWidget();
    this->startWidgetLayout = new QHBoxLayout(startBarWidget);
    //this->startWidgetLayout->addWidget(startButton);
    this->startWidgetLayout->addWidget(openButton);
    this->startWidgetLayout->addWidget(sendButton);
    this->startWidgetLayout->addWidget(closeButton);

    this->msgLabel = new QLabel(QString::fromLocal8Bit("��Ϣ"));
    this->msgLineEdit = new QLineEdit(QString::fromLocal8Bit("cciss"));
    this->msgPushButton = new QPushButton(QString::fromLocal8Bit("����"));

    this->msgWidget = new QWidget();
    this->msgWidgetLayout = new QHBoxLayout(msgWidget);
    this->msgWidgetLayout->addWidget(msgLabel);
    this->msgWidgetLayout->addWidget(msgLineEdit);
    this->msgWidgetLayout->addWidget(msgPushButton);

    this->stateLabel = new QLabel(QString::fromLocal8Bit("׼������"));

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
    this->textLabel = new QLabel(QString::fromLocal8Bit("׼������\n"));
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

