/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       无锡寰宇洗衣机定子驱动程序
*******************************************************************************/
#ifndef DEVWASHER_H
#define DEVWASHER_H

#include <QObject>
#include "devserial.h"

class DevWasher : public DevSerial
{
    Q_OBJECT
public:
    explicit DevWasher(DevSerial *parent = 0);
public slots:
    virtual QByteArray getTest(QVariantMap map);
    virtual void testThread(QVariantMap map);
    virtual void stopThread(QVariantMap map);
    virtual void recvAppMap(QVariantMap map);
    virtual int crc16(QByteArray hex);
};

#endif // DEVWASHER_H
