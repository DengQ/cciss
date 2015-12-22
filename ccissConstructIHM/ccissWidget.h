#ifndef CCISSWIDGET_H
#define CCISSWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QByteArray>
#include <QLineEdit>

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QHostAddress>

class ccissWidget : public QWidget
{
    Q_OBJECT
public:
    ccissWidget(QWidget *parent = 0);
    ~ccissWidget();
    void constructIHM();


private:
    QVBoxLayout* ccissMainWindowLayout;
    QHBoxLayout* titleBarLayout;
    QHBoxLayout* toolBarLayout;
    QHBoxLayout* operationWindowLayout;
    QHBoxLayout* stateBarLayout;

    QWidget* titleBarWidget;
    QWidget* toolBarWidget;
    QWidget* operationWindowWidget;
    QWidget* stateBarWidget;


signals:

public slots:
};

#endif // CCISSWIDGET_H
