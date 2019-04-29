/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       三菱伺服配置
*******************************************************************************/
#include "devmservo.h"

DevMServo::DevMServo(DevSerial *parent) : DevSerial(parent)
{
    com = NULL;
    isFree = true;
}

QByteArray DevMServo::getInit(QVariantMap map)
{
    QString taskname = map.value("taskname").toString();
    QByteArray msg = QByteArray::fromHex("013038350230303030303203");
    quint8 crc = 0;
    for (int i=1; i < msg.size(); i++) {
        crc += quint8(msg.at(i));
    }
    QByteArray dat = QString("%1").arg(crc, 2, 16, QChar('0')).toUpper().toUtf8();
    msg.append(dat);
    qDebug() << "com init:" << msg.toHex().toUpper() << taskname;
    return msg;
}

QByteArray DevMServo::getData(QVariantMap map)
{
    int taskdata = map.value("taskdata").toInt();
    QByteArray msg = QByteArray::fromHex("0130383402303633303030");
    QByteArray str = QString("%1").arg(taskdata, 4, 16, QChar('0')).toUpper().toUtf8();
    msg.append(str);
    msg.append(0x03);
    quint8 crc = 0;
    for (int i=1; i < msg.size(); i++) {
        crc += quint8(msg.at(i));
    }
    QByteArray dat = QString("%1").arg(crc, 2, 16, QChar('0')).toUpper().toUtf8();
    msg.append(dat);
    qDebug() << "com data:" << msg.toHex().toUpper() << taskdata;
    return msg;
}

QByteArray DevMServo::getTest(QVariantMap map)
{
    int taskdata = map.value("taskdata").toInt();
    int taskturn = map.value("taskturn").toInt();
    QByteArray msg = QByteArray::fromHex("0130393202363030303030");
    msg.append(QByteArray::fromHex((taskturn == 0 ? "3041" : "3132")));
    msg.append(QByteArray::fromHex((taskdata == 0 ? "303603" : "303703")));
    quint8 crc = 0;
    for (int i=1; i < msg.size(); i++) {
        crc += quint8(msg.at(i));
    }
    QByteArray dat = QString("%1").arg(crc, 2, 16, QChar('0')).toUpper().toUtf8();
    msg.append(dat);
    qDebug() << "com turn:" << msg.toHex().toUpper() << taskturn;
    return msg;
}

void DevMServo::testThread(QVariantMap map)
{
    QString taskname = map.value("taskname").toString();
    if (!setOpen(map, 9600, QSerialPort::EvenParity)) {  // 打开串口
        showError(tr("串口%1打开失败").arg(taskname));
        return;
    }
    for (int i=0; i < 3; i++) {
        QByteArray tmp = setSend(getInit(map), 50);  // 设置模式
        qDebug() << "com init:" << tmp;
        if (tmp.size() > 0)
            break;
    }
    for (int i=0; i < 3; i++) {
        QByteArray tmp = setSend(getData(map), 50);  // 设置转速
        qDebug() << "com data:" << tmp;
        if (tmp.size() > 0)
            break;
    }
    for (int i=0; i < 3; i++) {
        QByteArray tmp = setSend(getTest(map), 50);  // 设置启动
        qDebug() << "com test:" << tmp;
        if (tmp.size() > 0)
            break;
    }
}

void DevMServo::stopThread(QVariantMap map)
{
    if (com == NULL)
        return;
    if (com->isOpen()) {
        for (int i=0; i < 3; i++) {
            map.insert("taskdata", 50*(2-i));
            QByteArray tmp = setSend(getData(map), 50);  // 设置减速
            qDebug() << "com test:" << tmp;
            if (tmp.size() > 0)
                break;
        }
        for (int i=0; i < 3; i++) {
            QByteArray tmp = setSend(getTest(map), 50);  // 设置停止
            qDebug() << "com stop:" << tmp;
            if (tmp.size() > 0)
                break;
        }
        setQuit(map);
    }
}

void DevMServo::recvAppMap(QVariantMap map)
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
