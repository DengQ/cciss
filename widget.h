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
//����IP�Ͷ˿�
    QLabel* homeIPLabel;
    QLineEdit* homeIPLineEdit;
    QLabel* homePortLabel;
    QLineEdit* homePortLineEdit;
//Զ��IP�Ͷ˿�
    QLabel* remoteIPLabel;
    QLineEdit* remoteIPLineEdit;
    QLabel* remotePortLabel;
    QLineEdit* remotePortLineEdit;
//���ս�����
    QLabel* reciveProgressLabel;
    QProgressBar* reciveProgressBar;
//���ͽ�����
    QLabel* sendProgressLabel;
    QProgressBar* sendProgressBar;
//ѡ��ť
    //QPushButton* startButton;
    QPushButton* openButton;
    QPushButton* sendButton;
    QPushButton* closeButton;
//������Ϣ��
    QLabel* msgLabel;
    QLineEdit* msgLineEdit;
    QPushButton* msgPushButton;
//��ǰ״̬
    QLabel* stateLabel;

    QWidget* homeAdressWidget;
    QWidget* remoteAdressWidget;
    QWidget* reciveProgressWidget;
    QWidget* sendProgressWidget;
    QWidget* startBarWidget;
    QWidget* msgWidget;

//���崫�ͱ���
    ccissNetworkEnvironment *networkEnvironment;
    ccissServer *server;
    ccissClient *ccissToPiss;

    QTcpServer* tcpServer;
    QTcpSocket* client;

    QTcpSocket *tcpClient;
    QFile *localFile;  //Ҫ���͵��ļ�
    qint64 totalBytes;  //�����ܴ�С
    qint64 bytesWritten;  //�Ѿ��������ݴ�С
    qint64 bytesToWrite;   //ʣ�����ݴ�С
    qint64 loadSize;   //ÿ�η������ݵĴ�С
    QString fileName;  //�����ļ�·��
    QByteArray outBlock;  //���ݻ������������ÿ��Ҫ���͵�����

    bool fileSendReady;
    bool msgSendReady;
    QString msg;
    qint8 msgtype;

private slots:
    void acceptConnection();
    void startRead();
    void startServer();

    void fileSend();  //�ļ��������ӷ�����
    void startTransferFile();  //�����ļ���С����Ϣ
    void startTransferMsg();    //������Ϣ��С����Ϣ
    void updateClientProgress(qint64 numBytes); //�������ݣ������ļ�������
    void displayError(QAbstractSocket::SocketError); //��ʾ����
    void openFile();  //���ļ�
    void msgSend();   //��Ϣ�������ӷ�����
};

#endif // WIDGET_H
