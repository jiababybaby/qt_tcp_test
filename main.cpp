#include <QCoreApplication>
#include <QDebug>
#include <tcp_service.h>
#include <iostream>
int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    testMultiThreadTcpSocketServerDemo();
    return a.exec();
}
