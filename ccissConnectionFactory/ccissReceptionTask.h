#ifndef CCISSRECEPTIONTASK_H
#define CCISSRECEPTIONTASK_H

#include <QThread>
#include <QTcpSocket>

class ccissReceptionTask : public QThread
{
public:
    ccissReceptionTask(QTcpSocket *soc);
    void run();
private:
    QTcpSocket *soc;
};

#endif // CCISSRECEPTIONTASK_H
