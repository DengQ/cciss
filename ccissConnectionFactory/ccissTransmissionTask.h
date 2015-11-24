#ifndef CCISSTRANSMISSIONTASK_H
#define CCISSTRANSMISSIONTASK_H

#include <QThread>
#include <QTcpSocket>

class ccissTransmissionTask : public QThread
{
public:
    ccissTransmissionTask(QTcpSocket *soc);

    void run();

private:
    QTcpSocket *soc;
};

#endif // CCISSTRANSMISSIONTASK_H
