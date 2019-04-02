#include "devdirver.h"

DevDirver::DevDirver(QObject *parent) : QObject(parent)
{
    com = NULL;
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

void DevDirver::quit()
{
    if (com != NULL && com->isOpen()) {
        com->write(QByteArray::fromHex("B201A504000000006280"));
        qDebug() << "com turn:" << "B201A504000000006280";
        com->close();
    }
}

void DevDirver::turn()
{
    if (com != NULL && com->isOpen()) {
        com->write(QByteArray::fromHex("B201A504000F011E8E7F"));
        qDebug() << "com turn:" << "B201A504000F011E8E7F";
    }
}

