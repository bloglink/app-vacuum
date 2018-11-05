/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       变频器通信模块
*******************************************************************************/
#include "devinvert.h"

DevInvert::DevInvert(QObject *parent) : QObject(parent)
{
    initParm();
}

int DevInvert::open(QString name)
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

int DevInvert::test(QVariantMap msg)
{
    tmpMap = msg;
    setFreq();
    QTimer::singleShot(500, this, SLOT(putTest()));
    return Qt::Key_Away;
}

void DevInvert::initParm()
{
    com = NULL;
}

void DevInvert::setFreq()
{  // 地址0xxx,功能码0x10,寄存器0x0001,长度0x0002,字节数0x04
    QString msg;
    msg.append("02100001000204");
    int turn = tmpMap.value("turn").toInt();
    msg.append((turn == 0) ? "0001" : "0002");
    int freq = tmpMap.value("freq").toInt();
    msg.append(QString("%1").arg(freq, 4, 16, QChar('0')));
    write(QByteArray::fromHex(msg.toUtf8()));
}

void DevInvert::putTest()
{
    write(QByteArray::fromHex("020300800001"));
}

void DevInvert::ready()
{
    QByteArray msg = com->readAll();
    if (msg.size() >= 5 && msg.startsWith(QByteArray::fromHex("0203"))) {
        if (msg.mid(3, 2) != QByteArray::fromHex("0000")) {
            QString str = tr("变频器故障:%1").arg(QString(msg.mid(3, 2).toHex().toUpper()));
            tmpMap.insert("enum", Qt::Key_Stop);
            tmpMap.insert("text", str);
            emit sendAppMap(tmpMap);
            tmpMap.clear();
        }
    }
}

void DevInvert::write(QByteArray msg)
{
    if (com == NULL || !com->isOpen())
        return;
    quint16 crc = crc16(msg);
    msg.append(crc%256);
    msg.append(crc/256);
    qDebug() << "com send:" << msg.toHex().toUpper();
    com->write(msg);
}

quint16 DevInvert::crc16(QByteArray hex)
{
    quint16 crc = 0xFFFF;
    for (int t=0; t < hex.size(); t++) {
        crc = crc ^ quint8(hex.at(t));
        for (int i=0; i < 8; i++) {
            if ((crc & 0x0001) > 0) {
                crc = crc >> 1;
                crc = crc ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

