#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QByteArray>
#include <QLineEdit>

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QHostAddress>
#include <QDateTime>

#include "widget.h"
#include <QApplication>
#include "ccissNetworkEnvironment.h"
#include "ccissServer.h"
#include "ccissClient.h"
#include "ccissWidget.h"

class ccissClient;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void constructIHM();
    void setConnections();
    //void startServer();
private:
    QHBoxLayout* ccissWidgetLayoutAll;
    QVBoxLayout* textVboxLayout;
    QWidget* textWidget;
    QPushButton* startServerPushButton;
    QLabel* textLabel;

    QVBoxLayout* ccissWidgetLayout;
    QWidget* ccissWidget;
    QHBoxLayout* homeAdressBarLayout;
    QHBoxLayout* remoteAdressBarLayout;
    QHBoxLayout* reciveProgressLayout;
    QHBoxLayout* sendProgressLayout;
    QHBoxLayout* startWidgetLayout;
    QHBoxLayout* msgWidgetLayout;
    QHBoxLayout* stateWidgetLayout;
//本机IP和端口
    QLabel* homeIPLabel;
    QLineEdit* homeIPLineEdit;
    QLabel* homePortLabel;
    QLineEdit* homePortLineEdit;
//远程IP和端口
    QLabel* remoteIPLabel;
    QLineEdit* remoteIPLineEdit;
    QLabel* remotePortLabel;
    QLineEdit* remotePortLineEdit;
//接收进度条
    QLabel* reciveProgressLabel;
    QProgressBar* reciveProgressBar;
//发送进度条
    QLabel* sendProgressLabel;
    QProgressBar* sendProgressBar;
//选择按钮
    //QPushButton* startButton;
    QPushButton* openButton;
    QPushButton* sendButton;
    QPushButton* closeButton;
//发送消息框
    QLabel* msgLabel;
    QLineEdit* msgLineEdit;
    QPushButton* msgPushButton;
//当前状态
    QLabel* stateLabel;

    QWidget* homeAdressWidget;
    QWidget* remoteAdressWidget;
    QWidget* reciveProgressWidget;
    QWidget* sendProgressWidget;
    QWidget* startBarWidget;
    QWidget* msgWidget;

//定义传送变量
    ccissNetworkEnvironment *networkEnvironment;
    ccissServer *server;
    ccissClient *ccissToPiss;

    QTcpServer* tcpServer;
    QTcpSocket* client;

    QTcpSocket *tcpClient;
    QFile *localFile;  //要发送的文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //保存文件路径
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据

    bool fileSendReady;
    bool msgSendReady;
    QString msg;
    qint8 msgtype;

private slots:
    void acceptConnection();
    void startRead();
    void startServer();

    void fileSend();  //文件请求连接服务器
    void startTransferFile();  //发送文件大小等信息
    void startTransferMsg();    //发送消息大小等信息
    void updateClientProgress(qint64 numBytes); //发送数据，更新文件进度条
    void displayError(QAbstractSocket::SocketError); //显示错误
    void openFile();  //打开文件
    void msgSend();   //消息请求连接服务器
};

#endif // WIDGET_H
