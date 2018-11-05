/*******************************************************************************
 * Copyright [2017~2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QQueue>
#include <QTimer>
#include <QObject>
#include <QUdpSocket>
#include <QMessageBox>

#include "main.h"

#define TIME_OUT 1000

class UdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpSocket(QObject *parent = 0);
signals:
    void sendUdpMsg(QByteArray msg);
public slots:
    void initSocket();
private slots:
    void readReady();
    void sendSocket();
    void recvAppMsg(QTmpMap msg);
private:
    quint16 hostPort;
    QHostAddress hostAddr;
    QQueue<QByteArray> recver;
    QQueue<QByteArray> sender;

    QTmpMap tmpSet;
};

#endif // UDPSOCKET_H
