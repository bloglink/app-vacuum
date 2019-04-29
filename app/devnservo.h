/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       松下伺服配置
*******************************************************************************/
#ifndef DEVNSERVO_H
#define DEVNSERVO_H

#include "devserial.h"

class DevNServo : public DevSerial
{
    Q_OBJECT
public:
    explicit DevNServo(DevSerial *parent = 0);
public slots:
    virtual QByteArray getInit(QVariantMap map);
    virtual QByteArray getData(QVariantMap map);
    virtual QByteArray getTest(QVariantMap map);
    virtual void testThread(QVariantMap map);
    virtual void stopThread(QVariantMap map);
    virtual void recvAppMap(QVariantMap msg);
private:
    QSerialPort *com;
    QVariantMap tmp;
    bool isFree;
};

#endif // DEVNSERVO_H
