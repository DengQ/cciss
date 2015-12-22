#ifndef CCISSHANDSHAKE_H
#define CCISSHANDSHAKE_H

#include <QThread>
#include <QTcpSocket>
#include "ccissReceptionTask.h"
#include <QDateTime>
#include <QFile>

class ccissHandShake : public QThread
{
    Q_OBJECT
public:
    ccissHandShake(qintptr sd);/*socket descriptor*/
    void run();

private:
    qintptr sd;
    QTcpSocket *soc;
    qint16 blockSize;   //存放文件的大小信息
    QString message;  //存放从服务器接收到的字符串
//for QString
    quint16 AIM;
    quint16 transType;
    QString CurrentTime;
    quint16 DLC;
//for text
    qint16 totalBytes;  //存放总大小信息
    qint16 bytesReceived;  //已收到数据的大小
    qint16 fileNameSize;  //文件名的大小信息
    QString fileName;   //存放文件名
    QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区
public slots:
    void lecture();
    void disconnect();
    void readFromPiss();



};

#endif // CCISSHANDSHAKE_H
