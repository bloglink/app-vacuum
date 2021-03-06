/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据初始化
*******************************************************************************/
#ifndef SQLCREATE_H
#define SQLCREATE_H

#include <QDir>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QSqlDatabase>

#include "main.h"

#include "apppermit.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetmag.h"
#include "typsetinr.h"
#include "typsetacw.h"
#include "typsetimp.h"
#include "typsethal.h"
#include "typsetlod.h"
#include "typsetbmf.h"

#define STEP 0x0010
#define PSIZE 0x0100

class SqlCreate : public QObject
{
    Q_OBJECT
public:
    explicit SqlCreate(QObject *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
public slots:
    void initSqlDir();
private slots:
    void initTmpDat();
    void openBackup(bool isExist);
    void openSystem(bool isExist);
    void openConfig(bool isExist);
    void openRecord(bool isExist);
    void initBack(QSqlQuery query);
    void initACWB(QSqlQuery query);
    void initIMPB(QSqlQuery query);
    void initMisc(QSqlQuery query);
    void initSyst(QSqlQuery query);
    void initInfo(QSqlQuery query);
    void initLoad(QSqlQuery query);
    void initSign(QSqlQuery query);
    void initShow(QSqlQuery query);
    void initUser(QSqlQuery query);
    void initConf(QSqlQuery query);
    void initDCR(QSqlQuery query);
    void initMAG(QSqlQuery query);
    void initINR(QSqlQuery query);
    void initACW(QSqlQuery query);
    void initIMP(QSqlQuery query);
    void initPWR(QSqlQuery query);
    void initIND(QSqlQuery query);
    void initHAL(QSqlQuery query);
    void initLOD(QSqlQuery query);
    void initNLD(QSqlQuery query);
    void initBMF(QSqlQuery query);
    void initLVS(QSqlQuery query);
private:
    QTmpMap tmpSet;
};

#endif // SQLCREATE_H
