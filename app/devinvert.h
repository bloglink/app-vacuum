/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       变频器通信模块
*******************************************************************************/
#ifndef DEVINVERT_H
#define DEVINVERT_H

#include <QMap>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QtSerialPort/QSerialPort>

#include "main.h"

class DevInvert : public QObject
{
    Q_OBJECT
public:
    explicit DevInvert(QObject *parent = 0);

signals:
    void sendAppMap(QVariantMap msg);
public slots:
    int open(QString name);
    int test(QVariantMap msg);
private slots:
    void initParm();
    void setFreq();
    void putTest();

    void ready();
    void write(QByteArray msg);
    quint16 crc16(QByteArray hex);
private:
    QSerialPort *com;
    QVariantMap tmpMap;
};

#endif // DEVINVERT_H
