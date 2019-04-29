/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       松下PLC配置
*******************************************************************************/
#ifndef DEVNLOGIC_H
#define DEVNLOGIC_H

#include "devserial.h"

class DevNLogic : public DevSerial
{
    Q_OBJECT
public:
    explicit DevNLogic(DevSerial *parent = 0);
public slots:
    virtual QByteArray getData(QVariantMap map);
    virtual void testThread(QVariantMap map);
    virtual void stopThread(QVariantMap map);
    virtual void recvAppMap(QVariantMap map);
};

#endif // DEVNLOGIC_H
