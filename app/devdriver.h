/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       伺服通信模块
*******************************************************************************/
#ifndef DEVDRIVER_H
#define DEVDRIVER_H

#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "main.h"

class DevDriver : public QObject
{
    Q_OBJECT
public:
    explicit DevDriver(QObject *parent = 0);

signals:
    void sendAppMap(QVariantMap msg);
public slots:
    int open(QString name);
    int test();
private slots:
    void initParm();
    void ready();
    void write(QByteArray msg);
    int thread();
    int getRead();
    int getHand();
    int getConf();
    int getData();
private:
    QSerialPort *com;
    QVariantMap tmpMap;
    QByteArray tmpByte;
    int timeOut;
    bool isok;
    int testShift;
    int currMap;
};

#endif // DEVDRIVER_H
