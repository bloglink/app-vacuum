/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       日常维护
*******************************************************************************/
#ifndef APPREPAIR_H
#define APPREPAIR_H

#include <QTime>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QCheckBox>
#include <QShowEvent>
#include <QLCDNumber>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "main.h"

class AppRepair : public QWidget
{
    Q_OBJECT
public:
    explicit AppRepair(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initTimeBar();
    void initButtonBar();
    void initSettings();
    void saveSettings();
    void testRepair();
    void stopRepair();
    void updateTime();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QLabel *warn;
    QLCDNumber *timeLess;
    QCheckBox *boxAuto;
    QVariantMap tmpMap;
    QTime time;
    QTimer *timer;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // APPREPAIR_H
