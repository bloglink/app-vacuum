/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       低启配置界面
*******************************************************************************/
#ifndef TYPSETLVS_H
#define TYPSETLVS_H

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

#define LVS_SIZE 1

#define CMAXLVS1 0x00  // 电流上限
#define CMINLVS1 0x01  // 电流下限
#define PMAXLVS1 0x02  // 功率上限
#define PMINLVS1 0x03  // 功率下限

#define CACHELVS 0x10  // 离散数据长度

class TypSetLVS : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetLVS(QWidget *parent = 0);
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
    QSpinBox *passBox;
    QDoubleSpinBox *compBox;
    QSpinBox *voltBox;
    QList<QSpinBox*> spinboxs;

    QStringList turns;
    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETLVS_H
