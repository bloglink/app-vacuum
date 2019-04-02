/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       绝缘配置界面
*******************************************************************************/
#ifndef TYPSETINR_H
#define TYPSETINR_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QShowEvent>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>

#include "main.h"
#include "appbackup.h"
#include "boxqmodel.h"
#include "boxdouble.h"
#include "boxqitems.h"

#define CACHEINR 0x10

#define CHECKINR 0x00  // 电阻测试
#define PORTINR1 0x01  // 端口一
#define PORTINR2 0x02  // 端口二
#define VOLTINR1 0x03  // 电压
#define LOWERINR 0x04  // 电阻下限
#define UPPERINR 0x05  // 电阻上限
#define TIMEINR1 0x06  // 测试时间

class TypSetInr : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetInr(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initButtonBar();
    void initItemDelegate();
    void initSettings();
    void saveSettings();
    void confSettings();
    void autoInput(QModelIndex index);
    void autoCheck();
    void change();
    void recvShowEvent();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *view;
    BoxQModel *mView;
    QVariantMap config;
    QVariantMap tmpMap;
    QGroupBox *buttonL;
    QList<QCheckBox*> checkboxs;

    QStringList volts;
    QStringList ports;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETINR_H
