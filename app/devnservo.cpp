/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       松下伺服配置
*******************************************************************************/
#include "devnservo.h"

DevNServo::DevNServo(DevSerial *parent) : DevSerial(parent)
{
    com = NULL;
    isFree = true;
}

QByteArray DevNServo::getInit(QVariantMap map)
{  // 询问是否可以通信,下发05回复04
    QString taskname = map.value("taskname").toString();
    QByteArray msg = QByteArray::fromHex("0005");
    qDebug() << "com init:" << msg.toHex() << taskname;
    return msg;
}

QByteArray DevNServo::getData(QVariantMap map)
{  // 下发要读取的数据地址,回复06和05
    QString taskname = map.value("taskname").toString();
    QByteArray msg = QByteArray::fromHex("0001926D00");
    qDebug() << "com data:" << msg.toHex().toUpper() << taskname;
    return msg;
}

QByteArray DevNServo::getTest(QVariantMap map)
{  // 下发读取准备好,回复转速转矩
    QString taskname = map.value("taskname").toString();
    QByteArray msg = QByteArray::fromHex("0400");
    qDebug() << "com data:" << msg.toHex() << taskname;
    return msg;
}

void DevNServo::testThread(QVariantMap map)
{
    QString taskname = map.value("taskname").toString();
    if (!setOpen(map, 9600, QSerialPort::NoParity)) {  // 打开串口
        showError(tr("串口%1打开失败").arg(taskname));
        return;
    }
    for (int i=0; i < 3; i++) {
        QByteArray msg;
        msg = setSend(getInit(map), 50);
        if (msg.isEmpty())
            continue;
        msg = setSend(getData(map), 50);
        if (msg.isEmpty())
            continue;
        msg = setSend(getTest(map), 50);
        if (msg.isEmpty())
            continue;
        if (msg.size() >= 5) {
            int taskdata = quint8(msg.at(3)) + quint8(msg.at(4))*256;
            taskdata = (taskdata > 0x8000) ? (taskdata-0x10000) : taskdata;
            QVariantMap tmp;
            tmp.insert("enum", Qt::Key_Send);
            tmp.insert("taskdata", taskdata);
            emit sendAppMap(tmp);
            tmp.clear();
            qDebug() << "app read:" << taskdata << msg.toHex().toUpper();
        }
    }
}

void DevNServo::stopThread(QVariantMap map)
{
    if (com == NULL)
        return;
    if (com->isOpen()) {
        setQuit(map);
    }
}

void DevNServo::recvAppMap(QVariantMap map)
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
