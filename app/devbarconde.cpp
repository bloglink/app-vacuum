#include "devbarconde.h"

DevBarconde::DevBarconde(QObject *parent) : QObject(parent)
{
    com == NULL;
}

bool DevBarconde::setOpen(QVariantMap map, int buadrate, QSerialPort::Parity parity)
{  // 打开串口
    QString taskname = map.value("taskname").toString();
    QString prevname = tmp.value("taskname").toString();
    if (com == NULL || taskname != prevname) {  // 切换串口时重新创建
        com = new QSerialPort(taskname, this);
        tmp.insert("taskname", taskname);
    }
    if (com->isOpen())
        return true;
    if (com->open(QIODevice::ReadWrite)) {
        com->setParity(parity);
        com->setBaudRate(buadrate);
        com->setRequestToSend(false);
        com->setDataTerminalReady(true);
        com->setDataBits(QSerialPort::Data8);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        connect(com, SIGNAL(readyRead()), this, SLOT(readyRead()));
        return true;
    }
    qDebug() << "com open:" << taskname;
    return false;
}

bool DevBarconde::setQuit(QVariantMap map)
{  // 关闭串口
    if (com == NULL)
        return false;
    if (com->isOpen()) {
        com->close();
        return true;
    }
    QString taskwork = map.value("taskwork").toString();
    qDebug() << "com quit:" << taskwork;
    return false;
}

void DevBarconde::readyRead()
{
    QByteArray msg = com->readAll();
    qDebug() << "com code:" << msg;

    QVariantMap map;
    map.insert("enum", Qt::Key_Call);
    map.insert("taskdata", msg);
    emit sendAppMap(map);
    map.clear();
}

void DevBarconde::recvAppMap(QVariantMap map)
{
    QString taskwork = map.value("taskwork").toString();  // 任务名称
    if (taskwork == this->objectName())
        setOpen(map, 9600, QSerialPort::NoParity);
}

