#ifndef CCISSRECEPTIONTASK_H
#define CCISSRECEPTIONTASK_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QObject>
#include <QtNetwork>
#include <QString>
#include <QTcpServer>
#include <QDebug>


class ccissReceptionTask : public QThread
{
public:
    ccissReceptionTask(QTcpSocket *soc);
    void run();
private:
    qintptr socketDescriptor;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    QString fileName;   //存放文件名
    QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区

private slots:
    void updateServerProgress();  //更新进度条，接收数据

};

#endif // CCISSRECEPTIONTASK_H

