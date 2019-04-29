/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       松下PLC配置
*******************************************************************************/
#include "devnlogic.h"

DevNLogic::DevNLogic(DevSerial *parent) : DevSerial(parent)
{
    com = NULL;
    isFree = true;
}

QByteArray DevNLogic::getData(QVariantMap map)
{  // 设置转速:0/转矩:1
    int taskmode = map.value("taskmode").toInt();
    int taskdata = qMin(4000, map.value("taskdata").toInt());
    int turndata = (taskmode == 0) ? taskdata : 0;  // 转速
    int loaddata = (taskmode == 1) ? taskdata : 0;  // 转矩
    QByteArray msg = "%01#WDD0010000101";  // 设置数据
    msg.append(QString("%1").arg(turndata%256, 2, 16, QChar('0')).toUpper().toUtf8());
    msg.append(QString("%1").arg(turndata/256, 2, 16, QChar('0')).toUpper().toUtf8());
    msg.append(QString("%1").arg(loaddata%256, 2, 16, QChar('0')).toUpper().toUtf8());
    msg.append(QString("%1").arg(loaddata/256, 2, 16, QChar('0')).toUpper().toUtf8());
    msg.append("**\r");
    qDebug() << "com data:" << msg;
    return msg;
}

void DevNLogic::testThread(QVariantMap map)
{
    QString taskname = map.value("taskname").toString();
    if (!setOpen(map, 9600, QSerialPort::OddParity)) {  // 打开串口
        showError(tr("串口%1打开失败").arg(taskname));
        return;
    }
    for (int i=0; i < 3; i++) {
        QByteArray tmp = setSend(getInit(map), 50);  // 设置模式
        qDebug() << "com mode:" << tmp;
        if (tmp.size() > 0)
            break;
    }
    for (int i=0; i < 3; i++) {
        QByteArray tmp = setSend(getTurn(map), 50);  // 设置转向
        qDebug() << "com turn:" << tmp;
        if (tmp.size() > 0)
            break;
    }
    for (int i=0; i < 3; i++) {
        QByteArray tmp = setSend(getData(map), 50);  // 设置转速/转矩
        qDebug() << "com data:" << tmp;
        if (tmp.size() > 0)
            break;
    }
    for (int i=0; i < 3; i++) {
        QByteArray tmp = setSend(getTest(map), 50);  // 设置启动/停止
        qDebug() << "com test:" << tmp;
        if (tmp.size() > 0)
            break;
    }
}

void DevNLogic::stopThread(QVariantMap map)
{
    if (com == NULL)
        return;
    if (com->isOpen()) {
        for (int i=0; i < 3; i++) {
            QByteArray tmp = setSend(getTest(map), 50);  // 设置启动/停止
            qDebug() << "com stop:" << tmp;
            if (tmp.size() > 0)
                break;
        }
        setQuit(map);
    }
}

void DevNLogic::recvAppMap(QVariantMap map)
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
