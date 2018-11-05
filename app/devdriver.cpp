/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       伺服通信模块
*******************************************************************************/
#include "devdriver.h"

typedef int (DevDriver::*pClass)(void);
QMap<int, pClass> taskDrv;

DevDriver::DevDriver(QObject *parent) : QObject(parent)
{
    initParm();
}

int DevDriver::open(QString name)
{
    com = new QSerialPort(name, this);
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(9600);
        com->setParity(QSerialPort::NoParity);
        com->setDataBits(QSerialPort::Data8);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        com->setRequestToSend(false);
        connect(com, SIGNAL(readyRead()), this, SLOT(ready()));
        return Qt::Key_Away;
    } else {
        return Qt::Key_Meta;
    }
}

int DevDriver::test()
{
    testShift = Qt::Key_Away;
    return Qt::Key_Away;
}

void DevDriver::initParm()
{
    com = NULL;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(thread()));
    timer->start(10);

    currMap = 0;
    timeOut = 0;
    testShift = Qt::Key_Meta;

    taskDrv.insert(0x00, &DevDriver::getRead);
    taskDrv.insert(0x01, &DevDriver::getHand);
    taskDrv.insert(0x02, &DevDriver::getConf);
    taskDrv.insert(0x03, &DevDriver::getData);
}

void DevDriver::ready()
{
    QByteArray msg = com->readAll();
    isok = true;
    if (msg.size() >= 7) {
        int ss = quint8(msg.at(3)) + quint8(msg.at(4))*256;
        ss = (ss > 0x8000) ? (ss-0x10000) : ss;
        tmpMap.insert("text", QString::number(ss));
        int tt = quint8(msg.at(5)) + quint8(msg.at(6))*256;
        tt = (tt > 0x8000) ? (tt-0x10000) : tt;
        tmpMap.insert("data", QString::number(tt/2000.0));
        tmpMap.insert("enum", Qt::Key_Send);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
    }
    qDebug() << "com recv:" << msg.toHex().toUpper();
}

void DevDriver::write(QByteArray msg)
{
    if (com == NULL || !com->isOpen())
        return;
    com->write(msg);
    qDebug() << "com send:" << msg.toHex().toUpper();
}

int DevDriver::thread()
{
    int ret = Qt::Key_Meta;
    if (taskDrv.contains(currMap)) {
        ret = (this->*taskDrv[currMap])();
    }
    if (ret == Qt::Key_Away) {
        currMap++;
        testShift = Qt::Key_Meta;
        if (currMap >= taskDrv.size()) {
            currMap = 0;
        } else {
            ret = Qt::Key_Meta;
        }
    }
    return ret;
}

int DevDriver::getRead()
{
    timeOut = 0;
    isok = false;
    return testShift;
}

int DevDriver::getHand()
{  // 握手
    int ret = Qt::Key_Meta;
    if (timeOut % 100 == 0)
        write(QByteArray::fromHex("0005"));
    timeOut++;
    if (isok || timeOut > 50) {
        isok = false;
        timeOut = 0;
        ret = Qt::Key_Away;
    }
    return ret;
}

int DevDriver::getConf()
{  // 连接
    int ret = Qt::Key_Meta;
    if (timeOut % 100 == 0)
        write(QByteArray::fromHex("0001926D00"));
    timeOut++;
    if (isok || timeOut > 50) {
        isok = false;
        timeOut = 0;
        ret = Qt::Key_Away;
    }
    return ret;
}


int DevDriver::getData()
{
    int ret = Qt::Key_Meta;
    if (timeOut % 100 == 0)
        write(QByteArray::fromHex("0400"));
    timeOut++;
    if (isok || timeOut > 50) {
        isok = false;
        timeOut = 0;
        ret = Qt::Key_Away;
    }
    return ret;
}

