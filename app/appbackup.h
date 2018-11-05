/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       后台管理
*******************************************************************************/
#ifndef APPBACKUP_H
#define APPBACKUP_H

#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QProcess>
#include <QSettings>
#include <QListView>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>
#include <QHostAddress>
#include <QTextBrowser>
#include <QStandardItem>
#include <QNetworkInterface>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#ifndef __linux__
#include <qt_windows.h>
#endif

#include "main.h"
#include "boxqmodel.h"
#include "boxqitems.h"

const int backType = 0x00;
const int backCode = 0x01;
const int backArea = 0x04;
const int backWork = 0x05;
const int backVolt = 0x06;
const int backAuto = 0x07;
const int backWait = 0x08;
const int backHost = 0x09;
const int backPort = 0x0A;
const int backMode = 0x0B;
const int backNMag = 0x0C;
const int backWave = 0x0D;

class AppBackup : public QWidget
{
    Q_OBJECT
public:
    explicit AppBackup(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initBoxText();
    void initBoxItem();
    void initDevText();
    void initActText();
    void initMacText();
    void initSettings();
    void saveSettings();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QHBoxLayout *layout;
    QTableView *view;
    BoxQModel *mView;
    QTableWidget *offView;
    QTableWidget *actView;
    QList<QLineEdit*> txts;
    QList<QComboBox*> cmbs;
    QTmpMap tmpSet;
    QTextBrowser *text;
    QTmpMap tmpMsg;
};

#endif // APPBACKUP_H
