/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       介电强度配置界面
*******************************************************************************/
#ifndef TYPSETACW_H
#define TYPSETACW_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QCheckBox>
#include <QShowEvent>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>

#include "main.h"
#include "appbackup.h"
#include "appsystem.h"
#include "boxqmodel.h"
#include "boxqitems.h"
#include "boxdouble.h"

#define CHECKACW 0x00  // 电阻测试
#define PORTACW1 0x01  // 端口一
#define PORTACW2 0x02  // 端口二
#define VOLTACW1 0x03  // 电压
#define UPPERACW 0x04  // 电阻上限
#define LOWERACW 0x05  // 电阻下限
#define TIMEACW1 0x06  // 测试时间
#define FREQACW1 0x07
#define ADDRACWA 0x08

#define CACHEACW 0x10  // 离散数据长度

class TypSetAcw : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetAcw(QWidget *parent = 0);
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
    void autoChange();
    void autoInput();
    void change();
    void recvShowEvent();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *view;
    BoxQModel *mView;
    QCheckBox *vacuoBox;

    QStringList freqs;
    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETACW_H
