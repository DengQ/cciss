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
    qint16 blockSize;   //����ļ��Ĵ�С��Ϣ
    QString message;  //��Ŵӷ��������յ����ַ���
//for QString
    quint16 AIM;
    quint16 transType;
    QString CurrentTime;
    quint16 DLC;
//for text
    qint16 totalBytes;  //����ܴ�С��Ϣ
    qint16 bytesReceived;  //���յ����ݵĴ�С
    qint16 fileNameSize;  //�ļ����Ĵ�С��Ϣ
    QString fileName;   //����ļ���
    QFile *localFile;   //�����ļ�
    QByteArray inBlock;   //���ݻ�����
public slots:
    void lecture();
    void disconnect();
    void readFromPiss();



};

#endif // CCISSHANDSHAKE_H
