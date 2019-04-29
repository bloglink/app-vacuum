/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       安川变频器配置
*******************************************************************************/
#include "devinvert.h"

DevInvert::DevInvert(DevSerial *parent) : DevSerial(parent)
{
    com = NULL;
    isFree = true;
}

QByteArray DevInvert::getTest(QVariantMap map)
{
    int taskdata = map.value("taskdata").toDouble()*100;  // 频率
    int taskturn = map.value("taskturn").toInt();  // 转向
    QString freq = QString("%1").arg(taskdata, 4, 16, QChar('0'));
    QString turn = taskdata == 0 ? "0000" : ((taskturn == 0) ? "0001" : "0002");
    QByteArray msg = QByteArray::fromHex("02100001000204");
    msg.append(QByteArray::fromHex(turn.toUtf8()));
    msg.append(QByteArray::fromHex(freq.toUtf8()));
    int crc = crc16(msg);
    msg.append(crc%256);
    msg.append(crc/256);
    qDebug() << "com data:" << msg.toHex().toUpper() << taskdata << taskturn;
    return msg;
}

void DevInvert::testThread(QVariantMap map)
{
    QString taskname = map.value("taskname").toString();
    if (!setOpen(map, 9600, QSerialPort::NoParity)) {  // 打开串口
        showError(tr("串口%1打开失败").arg(taskname));
        return;
    }
    setSend(getTest(map), 50);
}

void DevInvert::stopThread(QVariantMap map)
{
    if (com == NULL)
        return;
    if (com->isOpen()) {
        setSend(getTest(map), 50);
        setQuit(map);
    }
}

void DevInvert::recvAppMap(QVariantMap map)
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
