/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导入
*******************************************************************************/
#ifndef SQLIMPORT_H
#define SQLIMPORT_H

#include <qmath.h>
#include <QUuid>
#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QSqlQuery>
#include <QSqlError>
#include <QElapsedTimer>

#include <QSerialPort>

#include "sqlsnowid.h"
#include "main.h"
#include "typsetimp.h"
#include "typsetdcr.h"
#include "typsetacw.h"

#define TEMPDATE 0x01  // 测试日期
#define TEMPPLAY 0x02  // 开始时间
#define TEMPSTOP 0x03  // 完成时间
#define TEMPTYPE 0x04  // 当前型号
#define TEMPCODE 0x05  // 条码状态
#define TEMPUSER 0x06  // 当前用户
#define TEMPWORK 0x07  // 当前工位
#define TEMPTEMP 0x08  // 当前温度
#define TEMPISOK 0x09  // 测试判定

#define SD_SIZE 80000

class SqlImport : public QObject
{
    Q_OBJECT
public:
    explicit SqlImport(QObject *parent = 0);

signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUpload();
    void saveRecord(QTmpMap msg);
    void saveSerial(QTmpMap msg);
    void saveUpload(QTmpMap msg);
    void saveOracle(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
private:
    QTmpMap tmpQuan;
    QTmpMap tmpOKNG;
    QElapsedTimer t;
    QTmpMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpDat;
    QString prev;
    int loop;
    QList<int> keys;
    QSerialPort *com;
};

#endif // SQLIMPORT_H
