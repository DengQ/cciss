#ifndef CCISSSERVER_H
#define CCISSSERVER_H

#include <QTcpServer>
#include "ccissNetworkEnvironment.h"
#include <QDateTime>

class ccissServer : public QTcpServer{

    Q_OBJECT
public:
    ccissServer(ccissNetworkEnvironment *networkEnvironment);

    void launchServer();

private:
    ccissNetworkEnvironment *networkEnvironment;

protected:
    void incomingConnection(qintptr socket);
};

#endif // CCISSSERVER_H
