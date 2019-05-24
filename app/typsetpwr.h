/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电参配置界面
*******************************************************************************/
#ifndef TYPSETPWR_H
#define TYPSETPWR_H

#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "main.h"
#include "appbackup.h"
#include "boxqmodel.h"
#include "boxdouble.h"
#include "boxqitems.h"
#include "libdelege.h"

#define PWR_SIZE 6

#define CHECKPWR 0x00  // 电参测试
#define CMAXPWR1 0x01  // 电流上限
#define CMINPWR1 0x02  // 电流下限
#define PMAXPWR1 0x03  // 功率上限
#define PMINPWR1 0x04  // 功率下限
#define VMAXPWR1 0x05  // 容压上限
#define VMINPWR1 0x06  // 容压下限
#define TURNPWR1 0x07  // 转向
#define TIMEPWR1 0x08  // 测试时间

#define CACHEPWR 0x10  // 离散数据长度

class TypSetPwr : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetPwr(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initHallBar();
    void initButtonBar();
    void initItemDelegate();
    void initSettings();
    void saveSettings();
    void confSettings();
    void autoInput();
    void autoHall();
    void change();
    void recvShowEvent();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *view;
    BoxQModel *mView;
    QTableView *hall;
    BoxQModel *mHall;
    QCheckBox *passBox;
    QSpinBox *vmaxBox;
    QSpinBox *voltBox;
    QSpinBox *countBox;
    QComboBox *pullBox;
    QComboBox *readBox;

    QStringList grades;
    QStringList turns;
    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETPWR_H
