#ifndef CCISSCLIENT_H
#define CCISSCLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QString>

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

public slots:
    void sendSelfInformation();

};

#endif // CCISSCLIENT_H
