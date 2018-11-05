/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       PLC通信模块
*******************************************************************************/
#ifndef DEVPLCTRL_H
#define DEVPLCTRL_H

#include <QMap>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "main.h"

class DevPlctrl : public QObject
{
    Q_OBJECT
public:
    explicit DevPlctrl(QObject *parent = 0);

signals:

public slots:
    int open(QString name);
    int test(QVariantMap msg);
private slots:
    void initParm();
    void ready();
    void write(QByteArray msg);
    int thread();
    int setPlay();
    int setMode();
    int setTurn();
    int setData();
    int setTest();
    int setStop();
    int setParm();
    void recvAppMsg(QTmpMap msg);

private:
    QSerialPort *com;
    QByteArray tmpByte;
    int taskShift;
    bool isok;
    int timeOut;
    int currMap;
    QVariantMap tmpMap;
    QTimer *timer;
};

#endif // DEVPLCTRL_H
