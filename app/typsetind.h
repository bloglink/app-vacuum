/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电感配置界面
*******************************************************************************/
#ifndef TYPSETIND_H
#define TYPSETIND_H

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
#include "boxqmodel.h"
#include "boxdouble.h"
#include "boxqitems.h"

#define IND_SIZE 8

#define CHECKIND 0x00  // 电感测试
#define PORTIND1 0x01  // 端口一
#define PORTIND2 0x02  // 端口二
#define UNITIND1 0x03  // 电感单位
#define UPPERIND 0x04  // 电感上限
#define LOWERIND 0x05  // 电感下限
#define QMAXIND1 0x06  // Q值上限
#define QMININD1 0x07  // Q值下限
#define STDDIND1 0x08  // 标准电感
#define COMPIND1 0x09  // 线路补偿1
#define COMPIND2 0x0A  // 线路补偿2

#define CACHEIND 0x10  // 离散数据长度

class TypSetInd : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetInd(QWidget *parent = 0);
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
    void autoCal();
    void change();
    void recvShowEvent();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *view;
    BoxQModel *mView;
    QSpinBox *timeBox;
    QDoubleSpinBox *nounBox;
    QComboBox *freqBox;
    QComboBox *modeBox;
    QComboBox *tempBox;
    QDoubleSpinBox *sminBox;
    QDoubleSpinBox *smaxBox;
    QStringList freqs;
    QStringList modes;
    QStringList speed;
    QStringList units;
    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETIND_H
