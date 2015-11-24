#ifndef CCISSHANDSHAKE_H
#define CCISSHANDSHAKE_H

#include <QThread>
#include <QTcpSocket>
#include "ccissReceptionTask.h"


class ccissHandShake : public QThread
{
    Q_OBJECT
public:
    ccissHandShake(qintptr sd);/*socket descriptor*/
    void run();

private:
    qintptr sd;
    QTcpSocket *soc;

public slots:
    void lecture();
    void disconnect();



};

#endif // CCISSHANDSHAKE_H
