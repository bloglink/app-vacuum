/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       松下PLC配置
*******************************************************************************/
#ifndef DEVSERIAL_H
#define DEVSERIAL_H

#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class DevSerial : public QObject
{
    Q_OBJECT
public:
    explicit DevSerial(QObject *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
public slots:
    bool setOpen(QVariantMap map, int buadrate, QSerialPort::Parity parity);
    bool setQuit(QVariantMap map);
    QByteArray setSend(QByteArray msg, int ms);
    QByteArray getInit(QVariantMap map);
    QByteArray getTurn(QVariantMap map);
    QByteArray getData(QVariantMap map);
    QByteArray getTest(QVariantMap map);
    void testThread(QVariantMap map);
    void stopThread(QVariantMap map);
    void recvAppMap(QVariantMap map);
    void showError(QString msg);
    void wait(int ms);
    int crc16(QByteArray hex);
public:
    QSerialPort *com;
    QVariantMap tmp;
    bool isFree;
};

#endif // DEVSERIAL_H
