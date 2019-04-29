/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       安川变频器配置
*******************************************************************************/
#ifndef DEVINVERT_H
#define DEVINVERT_H

#include "devserial.h"

class DevInvert : public DevSerial
{
    Q_OBJECT
public:
    explicit DevInvert(DevSerial *parent = 0);
public slots:
    virtual QByteArray getTest(QVariantMap map);
    virtual void testThread(QVariantMap map);
    virtual void stopThread(QVariantMap map);
    virtual void recvAppMap(QVariantMap map);
};

#endif // DEVINVERT_H
