﻿#include "ccissTransmissionTask.h"
#include <QFileDialog>
#include <QHostAddress>
#include "widget.h"
ccissTransmissionTask::ccissTransmissionTask(QTcpSocket *soc)
{
   // tcpClient = new QTcpSocket(this);
    this->tcpClient = soc;

    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
}

void ccissTransmissionTask::run(){
        qDebug()<<"ccissTransmissionTask Start";
    exec();
}

//void ccissTransmissionTask::openFile()   //打开文件
//{
//    qDebug()<<"openFile";
//    fileName ="t.txt";
//    //fileName = QFileDialog::getOpenFileName(this);
//    if(!fileName.isEmpty())
//    {
//        qDebug()<<"Open File Success!";
//   }
//}

void ccissTransmissionTask::startTransfer(){
    qDebug()<<"startTransfer";
    //tcpClient->write("Second time");


//    localFile = new QFile(fileName);
//    if(!localFile->open(QFile::ReadOnly))
//    {
//        qDebug() << "open file error!";
//        return;
//    }
//    totalBytes = localFile->size();
//    //文件总大小
//    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
//    sendOut.setVersion(QDataStream::Qt_5_4);
//    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
//    sendOut << qint64(0) << qint64(0) << currentFileName;
//    //依次写入总大小信息空间，文件名大小信息空间，文件名---仅是文件名不含路径，没必要含路径
//    totalBytes += outBlock.size();
//    //这里的总大小是文件名大小等信息和实际文件大小的总和
//    sendOut.device()->seek(0);
//    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
//    //totalBytes是文件总大小，即两个quint64的大小+文件名+文件实际内容的大小
//    //qint64((outBlock.size() - sizeof(qint64)*2))得到的是文件名大小
//    bytesToWrite = totalBytes - tcpClient->write(outBlock);
//    //发送完头数据后剩余数据的大小，即文件实际内容的大小
//    qDebug()<<"File Send Thread Connected!";
//    outBlock.resize(0);
//    qDebug()<<"#####"<<totalBytes;

//    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
//    //当有数据发送成功时，我们更新进度
//    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

}

//void ccissTransmissionTask::updateClientProgress(qint64 numBytes) //更新进度条，实现文件的传送
//{

//    qDebug()<<"#######已发送："<<bytesWritten<<"剩余："<<bytesToWrite;
//    bytesWritten += (int)numBytes;
//    //已经发送数据的大小
//    if(bytesToWrite > 0) //如果已经发送了数据
//    {
//        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
//      //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
//      //就发送剩余数据的大小
//        bytesToWrite -= (int)tcpClient->write(outBlock);
//       //发送完一次数据后还剩余数据的大小
//        outBlock.resize(0);
//       //清空发送缓冲区
//    }
//    else
//    {
//        localFile->close(); //如果没有发送任何数据，则关闭文件
//    }
//    if(bytesWritten == totalBytes) //发送完毕
//    {

//        localFile->close();
//        tcpClient->close();
//    }
//}

//void ccissTransmissionTask::displayError(QAbstractSocket::SocketError) //显示错误
//{
//    qDebug() << tcpClient->errorString();
//    tcpClient->close();
//    qDebug()<<"Client Ready!";
//}
