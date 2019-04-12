/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       寰宇驱动器
*******************************************************************************/
#ifndef DEVDIRVER_H
#define DEVDIRVER_H

#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class DevDirver : public QObject
{
    Q_OBJECT
public:
    explicit DevDirver(QObject *parent = 0);

signals:
    void sendAppMap(QVariantMap msg);
public slots:
    void open(QString name);
    void quit(QString name);
    bool setTest();
    bool setStop();
    void recvAppMap(QVariantMap map);
    void wait(int ms);
    QByteArray crc16CCITT(QByteArray msg);
private:
    QSerialPort *com;
    QVariantMap tmp;
    bool isFree;
};

#endif // DEVDIRVER_H
