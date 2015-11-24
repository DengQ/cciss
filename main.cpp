#include "widget.h"
#include <QApplication>
#include "ccissNetworkEnvironment.h"
#include "ccissServer.h"
#include "ccissClient.h"

int main(int argc, char *argv[]){

    QApplication a(argc, argv);

    //! initialisation of the parameter concern about the network environment
    ccissNetworkEnvironment *networkEnvironment = new ccissNetworkEnvironment();
    networkEnvironment->init("self", 12345);
    networkEnvironment->init("piss", 1234, "172.20.35.18");

    //! create tcp server to wait for the connexion from igss_platform part
    ccissServer *server = new ccissServer(networkEnvironment);
    server->launchServer();
    
    //! create tcp client to connect igss_platform
    ccissClient *ccissToPiss = new ccissClient(networkEnvironment);
    ccissToPiss->connectera();

    Widget *w = new Widget();
    w->show();

    return a.exec();
}
