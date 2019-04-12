/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       寰宇驱动器
*******************************************************************************/
#include "devdirver.h"

DevDirver::DevDirver(QObject *parent) : QObject(parent)
{
    com = NULL;
    isFree = true;
}

void DevDirver::open(QString name)
{
    if (com == NULL || name != tmp.value("taskname").toString()) {
        com = new QSerialPort(name, this);  // 切换串口时重新创建
        tmp.insert("taskname", name);
    }
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(2400);
        com->setRequestToSend(false);
        com->setDataTerminalReady(true);
        com->setDataBits(QSerialPort::Data8);
        com->setParity(QSerialPort::NoParity);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
    }
}

void DevDirver::quit(QString name)
{
    if (name == tmp.value("taskname").toString())
        com->close();
}

bool DevDirver::setTest()
{
    QByteArray msg = QByteArray::fromHex("B201A504");
    int tasktime = tmp.value("tasktime").toInt();
    int taskdata = tmp.value("taskdata").toInt();
    msg.append(QByteArray::fromHex(QString("%1").arg(tasktime, 4, 16, QChar('0')).toUtf8()));
    msg.append(QByteArray::fromHex(QString("%1").arg(taskdata, 4, 16, QChar('0')).toUtf8()));
    msg = crc16CCITT(msg);
    com->readAll();
    com->write(msg);
    wait(100);
    bool ret = com->bytesAvailable() > 0 ? true : false;
    qDebug() << "test" << msg.toHex().toUpper() << com->readAll().toHex().toUpper();
    return ret;
}

bool DevDirver::setStop()
{
    QByteArray msg = QByteArray::fromHex("B201A50400000000");
    msg = crc16CCITT(msg);
    com->readAll();
    com->write(msg);
    wait(100);
    bool ret = com->bytesAvailable() > 0 ? true : false;
    qDebug() << "test" << msg.toHex().toUpper() << com->readAll().toHex().toUpper();
    return ret;
}

void DevDirver::recvAppMap(QVariantMap map)
{
    if (isFree) {
        isFree = false;
        QString taskwork = map.value("taskwork").toString();  // 任务名称
        QString taskname = map.value("taskname").toString();  // 串口名称
        if (taskwork == "washtest") {
            open(taskname);  // 打开串口
            tmp.insert("taskdata", map.value("taskdata").toInt());
            tmp.insert("tasktime", map.value("tasktime").toInt());
            for (int i=0; i < 3; i++) {
                if (!setTest())
                    continue;
                break;
            }
        }
        if (taskwork == "washstop") {
            tmp.insert("taskdata", tmp.value("taskdata").toInt() / 3);
            for (int i=0; i < 3; i++) {
                if (!setStop())
                    continue;
                break;
            }
            quit(taskname);  // 关闭串口
        }
        isFree = true;
    }
}

void DevDirver::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

QByteArray DevDirver::crc16CCITT(QByteArray msg)
{
    quint16 crc = 0xFFFF;
    for (int t=1; t < msg.size(); t++) {
        crc = crc ^ (quint8(msg.at(t)) << 8);
        for (int i=0; i < 8; i++) {
            if ((crc & 0x8000) != 0) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }
    msg.append(crc/256);
    msg.append(crc%256);
    return msg;
}

