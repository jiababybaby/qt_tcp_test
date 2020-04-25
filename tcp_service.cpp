#include "tcp_service.h"
#include <QDebug>
#include <QTcpSocket>
#include <QtNetwork>
MultiThreadTcpSocketServerDemo::MultiThreadTcpSocketServerDemo()
{
    //初始换原始数据
    m_oData << tr("You've been leading a dog's life. Stay off the furniture.")
        << tr("You've got to think about tomorrow.")
        << tr("You will be surprised by a loud noise.")
        << tr("You will feel hungry again in another hour.")
        << tr("You might have mail.")
        << tr("You cannot kill time without injuring eternity.")
        << tr("Computers are not intelligent. They only think they are.");
}

void MultiThreadTcpSocketServerDemo::incomingConnection(qintptr handle)
{
    //获取服务端数据
    QString sWriteData = m_oData.at(qrand() % m_oData.size());
    qDebug() << "MultiThreadTcpSocketServerDemo::incomingConnection" << handle;
    ServerHandleThread* pThread = new ServerHandleThread(handle, sWriteData);
    connect(pThread, &ServerHandleThread::finished, pThread, &ServerHandleThread::deleteLater);
    pThread->start();
}

ServerHandleThread::ServerHandleThread(qintptr handle, const QString& sWriteData):m_sWriteData(sWriteData), m_nHandle(handle)
{

}
void ServerHandleThread::connected(){
    qDebug()<<"connected";
}
void ServerHandleThread::readyRead(){
    qDebug()<<"readyRead";
    QString read_data;
    QByteArray tmp;
    tmp=oTcpSocket->read(9600);
    read_data=QString::fromUtf8(tmp);
    if(*read_data.begin()=="\n"&&*(read_data.end()-1)=="\r")qDebug().noquote()<<tmp;
}
void ServerHandleThread::run()
{
    //1. 建立与客户端通信的TCP套接字
    oTcpSocket=new QTcpSocket;
    if (!oTcpSocket->setSocketDescriptor(m_nHandle))
    {
        qDebug() << "oTcpSocket.setSocketDescriptor error";
        return;
    }
    connect(oTcpSocket, &QIODevice::readyRead, this, &ServerHandleThread::readyRead);
    //2. 向客户端写数据
    qDebug() << "MultiThreadTcpSocketServerDemo::readDataFromClient" << oTcpSocket;
    qDebug() << "MultiThreadTcpSocketServerDemo::writeDataToClient" << m_sWriteData;
    oTcpSocket->write(m_sWriteData.toUtf8());
//    oTcpSocket.disconnectFromHost();
//    oTcpSocket.waitForDisconnected();
    exec();
}

void testMultiThreadTcpSocketServerDemo()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            if(address.toString().contains("127.0.")) continue;

             qDebug() << "ip:" << address.toString();
        }
        else if (address.isNull())  // 主机地址为空
            continue;
    }
    //1. 建立服务器端套接字
    MultiThreadTcpSocketServerDemo* m_pTcpServer = new MultiThreadTcpSocketServerDemo();
    //2. 启动服务端
    if (!m_pTcpServer->listen(QHostAddress::Any, 8899))
    {
        qDebug() << "m_pTcpServer->listen() error";
    }else{
        qDebug() << "port:8899";
    }
}

