/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       无锡寰宇洗衣机定子驱动程序
*******************************************************************************/
#include "devwasher.h"

DevWasher::DevWasher(DevSerial *parent) : DevSerial(parent)
{
    com = NULL;
    isFree = true;
}

QByteArray DevWasher::getTest(QVariantMap map)
{
    QByteArray msg = QByteArray::fromHex("B201A504");
    int tasktime = map.value("tasktime").toInt();
    int taskdata = map.value("taskdata").toInt();
    msg.append(QByteArray::fromHex(QString("%1").arg(tasktime, 4, 16, QChar('0')).toUtf8()));
    msg.append(QByteArray::fromHex(QString("%1").arg(taskdata, 4, 16, QChar('0')).toUtf8()));
    int crc = crc16(msg);
    msg.append(crc/256);
    msg.append(crc%256);
    qDebug() << "com test:" << msg.toHex().toUpper();
    return msg;
}

void DevWasher::testThread(QVariantMap map)
{
    QString taskname = map.value("taskname").toString();
    if (!setOpen(map, 2400, QSerialPort::NoParity)) {  // 打开串口
        showError(tr("串口%1打开失败").arg(taskname));
        return;
    }
    for (int i=0; i < 3; i++) {
        QByteArray tmp = setSend(getTest(map), 150);
        qDebug() << "com recv:" << tmp.toHex().toUpper();
        if (tmp.size() > 0)
            return;
    }
    showError(tr("串口%1启动失败").arg(taskname));
}

void DevWasher::stopThread(QVariantMap map)
{
    if (com == NULL)
        return;
    if (com->isOpen())
        setQuit(map);
}

void DevWasher::recvAppMap(QVariantMap map)
{
    if (isFree) {
        isFree = false;
        QString taskwork = map.value("taskwork").toString();  // 任务名称
        if (taskwork == this->objectName())
            testThread(map);
        if (taskwork == "stop")
            stopThread(map);
        isFree = true;
    }
}

int DevWasher::crc16(QByteArray hex)
{
    quint16 crc = 0xFFFF;
    for (int t=1; t < hex.size(); t++) {
        crc = crc ^ (quint8(hex.at(t)) << 8);
        for (int i=0; i < 8; i++) {
            if ((crc & 0x8000) != 0) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

