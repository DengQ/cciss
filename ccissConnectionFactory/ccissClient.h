#ifndef CCISSCLIENT_H
#define CCISSCLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QDateTime>

#include "ccissNetworkEnvironment.h"
#include "ccissTransmissionTask.h"


class ccissClient : public QTcpSocket
{
    Q_OBJECT
public:
    ccissClient(ccissNetworkEnvironment *networkEnvironment);

    void connectera();

private:
    qintptr sd;
    ccissNetworkEnvironment *networkEnvironment;

    QString message;
    quint16 totalBytes;
    QString CurrentTime;
public slots:
    void sendSelfInformation();
    void startTransfer();

};

#endif // CCISSCLIENT_H
