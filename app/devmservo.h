/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       三菱伺服配置
*******************************************************************************/
#ifndef DEVMSERVO_H
#define DEVMSERVO_H

#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class DevMServo : public QObject
{
    Q_OBJECT
public:
    explicit DevMServo(QObject *parent = 0);

signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void open(QString name);
    void quit(QString name);
    bool setInit();
    bool setData();
    bool setTest();
    bool setStop();
    void recvAppMap(QVariantMap map);
    void wait(int ms);
private:
    QSerialPort *com;
    QVariantMap tmp;
    bool isFree;
};

#endif // DEVMSERVO_H
