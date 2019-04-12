/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       三菱伺服配置
*******************************************************************************/
#include "devmservo.h"

DevMServo::DevMServo(QObject *parent) : QObject(parent)
{
    com = NULL;
    isFree = true;
}

void DevMServo::open(QString name)
{
    if (com == NULL || name != tmp.value("taskname").toString()) {
        com = new QSerialPort(name, this);  // 切换串口时重新创建
        tmp.insert("taskname", name);
    }
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(9600);
        com->setRequestToSend(false);
        com->setDataTerminalReady(true);
        com->setDataBits(QSerialPort::Data8);
        com->setParity(QSerialPort::EvenParity);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
    }
}

void DevMServo::quit(QString name)
{
    if (name == tmp.value("taskname").toString())
        com->close();
}

bool DevMServo::setInit()
{  // 握手
    QByteArray msg = QByteArray::fromHex("013038350230303030303203");
    quint8 crc = 0;
    for (int i=1; i < msg.size(); i++) {
        crc += quint8(msg.at(i));
    }
    QByteArray dat = QString("%1").arg(crc, 2, 16, QChar('0')).toUpper().toUtf8();
    msg.append(dat);
    qDebug() << msg.toHex().toUpper() << dat.toUpper();
    com->readAll();
    com->write(msg);
    wait(50);
    bool ret = com->bytesAvailable() > 0 ? true : false;
    qDebug() << com->readAll().toHex().toUpper();
    return ret;
}

bool DevMServo::setData()
{
    int s = tmp.value("taskdata").toInt();
    QByteArray msg = QByteArray::fromHex("0130383402303633303030");
    QByteArray str = QString("%1").arg(s, 4, 16, QChar('0')).toUpper().toUtf8();
    msg.append(str);
    msg.append(0x03);
    quint8 crc = 0;
    for (int i=1; i < msg.size(); i++) {
        crc += quint8(msg.at(i));
    }
    QByteArray dat = QString("%1").arg(crc, 2, 16, QChar('0')).toUpper().toUtf8();
    msg.append(dat);
    qDebug() << msg.toHex().toUpper() << dat.toUpper();
    com->readAll();
    com->write(msg);
    wait(50);
    bool ret = com->bytesAvailable() > 0 ? true : false;
    qDebug() << com->readAll().toHex().toUpper();
    return ret;
}

bool DevMServo::setTest()
{
    int turn = tmp.value("taskturn").toInt();
    QByteArray msg = QByteArray::fromHex("0130393202363030303030");
    msg.append(QByteArray::fromHex((turn == 0 ? "3041" : "3132")));
    msg.append(QByteArray::fromHex("303703"));
    quint8 crc = 0;
    for (int i=1; i < msg.size(); i++) {
        crc += quint8(msg.at(i));
    }
    QByteArray dat = QString("%1").arg(crc, 2, 16, QChar('0')).toUpper().toUtf8();
    msg.append(dat);
    qDebug() << msg.toHex().toUpper() << dat.toUpper();
    com->readAll();
    com->write(msg);
    wait(50);
    bool ret = com->bytesAvailable() > 0 ? true : false;
    qDebug() << com->readAll().toHex().toUpper();
    return ret;
}

bool DevMServo::setStop()
{
    int turn = tmp.value("taskturn").toInt();
    QByteArray msg = QByteArray::fromHex("0130393202363030303030");
    msg.append(QByteArray::fromHex((turn == 0 ? "3041" : "3132")));
    msg.append(QByteArray::fromHex("303603"));
    quint8 crc = 0;
    for (int i=1; i < msg.size(); i++) {
        crc += quint8(msg.at(i));
    }
    QByteArray dat = QString("%1").arg(crc, 2, 16, QChar('0')).toUpper().toUtf8();
    msg.append(dat);
    qDebug() << msg.toHex().toUpper() << dat.toUpper();
    com->readAll();
    com->write(msg);
    wait(50);
    bool ret = com->bytesAvailable() > 0 ? true : false;
    qDebug() << com->readAll().toHex().toUpper();
    return ret;
}

void DevMServo::recvAppMap(QVariantMap map)
{
    if (isFree) {
        isFree = false;
        QString taskwork = map.value("taskwork").toString();  // 任务名称
        QString taskname = map.value("taskname").toString();  // 串口名称
        if (taskwork == "turn") {
            open(taskname);  // 打开串口
            tmp.insert("taskdata", map.value("taskdata").toInt());
            tmp.insert("taskturn", map.value("taskturn").toInt());
            for (int i=0; i < 3; i++) {
                if (!setInit())
                    continue;
                if (!setData())
                    continue;
                if (!setTest())
                    continue;
                break;
            }
        }
        if (taskwork == "stop") {
            tmp.insert("taskdata", tmp.value("taskdata").toInt() / 3);
            for (int i=0; i < 3; i++) {
                if (!setInit())
                    continue;
                if (!setData())
                    continue;
                if (!setStop())
                    continue;
                break;
            }
            quit(taskname);  // 关闭串口
        }
        isFree = true;
    }
}

void DevMServo::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

