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

#include "devserial.h"

class DevMServo : public DevSerial
{
    Q_OBJECT
public:
    explicit DevMServo(DevSerial *parent = 0);
public slots:
    virtual QByteArray getInit(QVariantMap map);
    virtual QByteArray getData(QVariantMap map);
    virtual QByteArray getTest(QVariantMap map);
    virtual void testThread(QVariantMap map);
    virtual void stopThread(QVariantMap map);
    virtual void recvAppMap(QVariantMap map);
};

#endif // DEVMSERVO_H
