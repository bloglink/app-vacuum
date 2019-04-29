/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       松下PLC配置
*******************************************************************************/
#include "devserial.h"

DevSerial::DevSerial(QObject *parent) : QObject(parent)
{
    com = NULL;
    isFree = true;
}

bool DevSerial::setOpen(QVariantMap map, int buadrate, QSerialPort::Parity parity)
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
        return true;
    }
    qDebug() << "com open:" << taskname;
    return false;
}

bool DevSerial::setQuit(QVariantMap map)
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

QByteArray DevSerial::setSend(QByteArray msg, int ms)
{  // 发送数据
    if (com == NULL)
        return NULL;
    if (com->isOpen()) {
        com->write(msg);
        wait(ms);
        return com->readAll();
    }
    qDebug() << "com send:" << msg;
    return NULL;
}

QByteArray DevSerial::getInit(QVariantMap map)
{  // 设置模式 0:转速模式 1:转矩模式
    int taskmode = map.value("taskmode").toInt();
    QByteArray msg = (taskmode == 0) ? "%01#WCSR00010**\r" : "%01#WCSR00011**\r";
    qDebug() << "com mode:" << msg;
    return msg;
}

QByteArray DevSerial::getTurn(QVariantMap map)
{  // 设置转向 0:正向转动 1:反向转动
    int taskturn = map.value("taskturn").toInt();
    QByteArray msg = (taskturn == 0) ? "%01#WCSR00030**\r" : "%01#WCSR00031**\r";
    qDebug() << "com turn:" << msg;
    return msg;
}

QByteArray DevSerial::getData(QVariantMap map)
{  // 设置转速/转矩
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

QByteArray DevSerial::getTest(QVariantMap map)
{  // 启动输出
    QString taskwork = map.value("taskwork").toString();
    QString needwork = this->objectName();
    QByteArray msg = (taskwork == needwork) ? "%01#WCSR00001**\r" : "%01#WCSR00000**\r";
    qDebug() << "com test:" << msg;
    return msg;
}

void DevSerial::testThread(QVariantMap map)
{
    QString taskname = map.value("taskname").toString();
    if (!setOpen(map, 9600, QSerialPort::NoParity)) {  // 打开串口
        showError(tr("串口%1打开失败").arg(taskname));
        return;
    }
}

void DevSerial::stopThread(QVariantMap map)
{
    setQuit(map);
}

void DevSerial::recvAppMap(QVariantMap map)
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

void DevSerial::showError(QString msg)
{
    QVariantMap map;
    map.insert("enum", Qt::Key_Word);
    map.insert("text", msg);
    emit sendAppMap(map);
    map.clear();
}

void DevSerial::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

int DevSerial::crc16(QByteArray hex)
{
    quint16 crc = 0xFFFF;
    for (int t=0; t < hex.size(); t++) {
        crc = crc ^ quint8(hex.at(t));
        for (int i=0; i < 8; i++) {
            if (crc & 0x0001) {
                crc = crc >> 1;
                crc = crc ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

