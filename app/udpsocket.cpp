/*******************************************************************************
 * Copyright [2017~2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#include "udpsocket.h"

UdpSocket::UdpSocket(QObject *parent) : QUdpSocket(parent)
{
    hostPort = 6000;
#ifdef __linux__
    hostAddr = "192.168.1.53";
#else
    hostAddr = "192.168.1.56";
#endif
}

void UdpSocket::initSocket()
{
    if (this->state() == QUdpSocket::BoundState)
        return;

#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    this->bind(QHostAddress::Any, txPort);
#else
    this->bind(QHostAddress::AnyIPv4, hostPort);
#endif
    connect(this, SIGNAL(readyRead()), this, SLOT(readReady()));

    this->writeDatagram("6000", hostAddr, hostPort);
    this->waitForBytesWritten();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSocket()));
    timer->start(10);

    qDebug() << "udp init:" << hostAddr.toString() << hostPort;
}

void UdpSocket::readReady()
{
    while (this->hasPendingDatagrams()) {
        QByteArray msg;
        QHostAddress address;
        quint16 port;
        msg.resize(this->pendingDatagramSize());
        this->readDatagram(msg.data(), msg.size(), &address, &port);
        if (address == hostAddr)
            recver.append(msg);
        msg.clear();
    }
}

void UdpSocket::sendSocket()
{
    while (!recver.isEmpty()) {
        QByteArray msg = recver.dequeue();
        if (!msg.startsWith("0x31") && !msg.startsWith("6032"))
            qDebug() << "udp recv:" << msg;
        emit sendUdpMsg(msg);
    }
    while (!sender.isEmpty()) {
        QByteArray msg = sender.dequeue();
        this->writeDatagram(msg, hostAddr, hostPort);
        if (!msg.startsWith("6000") && !msg.startsWith("6030"))
            qDebug() << "udp send:" << msg;
        this->waitForBytesWritten();
    }
}

void UdpSocket::recvAppMsg(QTmpMap msg)
{
    QByteArray dat;
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Save:
        if (this->state() == QUdpSocket::BoundState) {
            dat = msg.value(Qt::Key_1).toByteArray();
            sender.append(dat);
        }
        break;
    default:
        break;
    }
}
