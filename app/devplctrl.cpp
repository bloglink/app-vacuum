/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       PLC通信模块
*******************************************************************************/
#include "devplctrl.h"

typedef int (DevPlctrl::*pClass)(void);
QMap<int, pClass> taskplc;

DevPlctrl::DevPlctrl(QObject *parent) : QObject(parent)
{
    initParm();
}

int DevPlctrl::open(QString name)
{
    com = new QSerialPort(name, this);
    if (com->open(QIODevice::ReadWrite)) {
        com->setBaudRate(9600);
        com->setParity(QSerialPort::OddParity);
        com->setDataBits(QSerialPort::Data8);
        com->setStopBits(QSerialPort::OneStop);
        com->setFlowControl(QSerialPort::NoFlowControl);
        com->setDataTerminalReady(true);
        com->setRequestToSend(false);
        connect(com, SIGNAL(readyRead()), this, SLOT(ready()));
        timer->start(20);
        return Qt::Key_Away;
    } else {
        return Qt::Key_Meta;
    }
}

int DevPlctrl::test(QVariantMap msg)
{
    if (com == NULL)
        return Qt::Key_Away;
    tmpMap = msg;
    taskplc.clear();
    if (msg.value("data").isNull()) {
        taskplc.insert(0x00, &DevPlctrl::setPlay);
        taskplc.insert(0x01, &DevPlctrl::setStop);
        taskplc.insert(0x02, &DevPlctrl::setParm);
    } else {
        taskplc.insert(0x00, &DevPlctrl::setPlay);
        taskplc.insert(0x01, &DevPlctrl::setMode);
        taskplc.insert(0x02, &DevPlctrl::setTurn);
        taskplc.insert(0x03, &DevPlctrl::setData);
        taskplc.insert(0x04, &DevPlctrl::setTest);
        taskplc.insert(0x05, &DevPlctrl::setParm);
    }
    taskShift = Qt::Key_Away;
    return Qt::Key_Away;
}

void DevPlctrl::initParm()
{
    com = NULL;
    currMap = 0;
    timeOut = 0;
    isok = false;
    taskShift = Qt::Key_Meta;
    taskplc.insert(0x00, &DevPlctrl::setPlay);
    taskplc.insert(0x01, &DevPlctrl::setStop);
    taskplc.insert(0x02, &DevPlctrl::setParm);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(thread()));
}

void DevPlctrl::ready()
{
    tmpByte.append(com->readAll());
    if (tmpByte.endsWith("\r") && tmpByte.contains("$")) {
        qDebug() << "com recv:" << tmpByte;
        tmpByte.clear();
        isok = true;
    }
}

void DevPlctrl::write(QByteArray msg)
{
    if (com == NULL || !com->isOpen())
        return;
    com->write(msg);
    qDebug() << "com send:" << msg;
}

int DevPlctrl::thread()
{
    int ret = Qt::Key_Meta;
    if (taskplc.contains(currMap)) {
        ret = (this->*taskplc[currMap])();
    }
    if (ret == Qt::Key_Away) {
        currMap++;
        if (currMap >= taskplc.size()) {
            currMap = 0;
        } else {
            ret = Qt::Key_Meta;
        }
    }
    return ret;
}

int DevPlctrl::setPlay()
{
    return taskShift;
}

int DevPlctrl::setMode()
{ // 目标站号0x01,指命代码WCS,触点代码R,触点编号0x0001,触点数据0/1
    int ret = Qt::Key_Meta;
    if (timeOut % 100 == 0) {
        QByteArray msg = "%01#WCSR0001";
        msg.append(QString::number(tmpMap.value("mode").toInt()));
        msg.append("**\r");
        write(msg);
    }
    timeOut++;
    if (isok || timeOut > 300) {
        timeOut = 0;
        isok = false;
        ret = Qt::Key_Away;
    }
    return ret;
}

int DevPlctrl::setTurn()
{ // 目标站号0x01,指命代码WCS,触点代码R,触点编号0x0003,触点数据0/1
    int ret = Qt::Key_Meta;
    if (timeOut % 100 == 0) {
        QByteArray msg = "%01#WCSR0003";
        msg.append(QString::number(tmpMap.value("turn").toInt()));
        msg.append("**\r");
        write(msg);
    }
    timeOut++;
    if (isok || timeOut > 300) {
        timeOut = 0;
        isok = false;
        ret = Qt::Key_Away;
    }
    return ret;
}

int DevPlctrl::setData()
{ // 目标站号0x01,指命代码WD,数据代码D,起始数据编码00100,结束数据编码00101
    int ret = Qt::Key_Meta;
    if (timeOut % 100 == 0) {
        int mode = tmpMap.value("mode").toInt();
        int s = (mode == 0) ? tmpMap.value("data").toDouble()*2.006 : 0;
        int t = (mode == 1) ? tmpMap.value("data").toDouble()*1.000 : 0;
        s = qMin(4000, s);
        QByteArray msg = "%01#WDD0010000101";
        msg.append(QString("%1").arg(s%256, 2, 16, QChar('0')).toUpper().toUtf8());
        msg.append(QString("%1").arg(s/256, 2, 16, QChar('0')).toUpper().toUtf8());
        msg.append(QString("%1").arg(t%256, 2, 16, QChar('0')).toUpper().toUtf8());
        msg.append(QString("%1").arg(t/256, 2, 16, QChar('0')).toUpper().toUtf8());
        msg.append("**\r");  // 设置转速时转矩设置为0
        write(msg);
    }
    timeOut++;
    if (isok || timeOut > 300) {
        timeOut = 0;
        isok = false;
        ret = Qt::Key_Away;
    }
    return ret;
}

int DevPlctrl::setTest()
{  // 目标站号0x01,指命代码WCS,触点代码R,触点编号0x0000,触点数据0/1
    int ret = Qt::Key_Meta;
    if (timeOut % 100 == 0) {
        QByteArray msg = "%01#WCSR00001**\r";
        write(msg);
    }
    timeOut++;
    if (isok || timeOut > 300) {
        timeOut = 0;
        isok = false;
        ret = Qt::Key_Away;
    }
    return ret;
}

int DevPlctrl::setStop()
{  // 目标站号0x01,指命代码WCS,触点代码R,触点编号0x0000,触点数据0/1
    int ret = Qt::Key_Meta;
    if (timeOut % 100 == 0) {
        QByteArray msg = "%01#WCSR00000**\r";
        write(msg);
    }
    timeOut++;
    if (isok || timeOut > 300) {
        timeOut = 0;
        isok = false;
        ret = Qt::Key_Away;
    }
    return ret;
}

int DevPlctrl::setParm()
{
    taskShift = Qt::Key_Meta;
    return Qt::Key_Away;
}

void DevPlctrl::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Call:
        if (msg.value(Qt::Key_2).toInt() == DATADC)
            write("%01#WCSR00000**\r");
        break;
    default:
        break;
    }
}

