/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置界面
*******************************************************************************/
#ifndef TYPSETDCR_H
#define TYPSETDCR_H

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
#include "appmaster.h"
#include "boxqmodel.h"
#include "boxdouble.h"
#include "boxqitems.h"

#define DCR_SIZE 8

#define CHECKDCR 0x00  // 电阻测试
#define PORTDCR1 0x01  // 端口一
#define PORTDCR2 0x02  // 端口二
#define WIREDCR1 0x03  // 线圈材料
#define UNITDCR1 0x04  // 电阻单位
#define UPPERDCR 0x05  // 电阻上限
#define LOWERDCR 0x06  // 电阻下限
#define SSTDDCR1 0x07  // 标准电阻
#define COMPDCR1 0x08  // 线路补偿1
#define COMPDCR2 0x09  // 线路补偿2

#define CACHEDCR 0x10  // 离散数据长度

class TypSetDcr : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetDcr(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initButtonBar();
    void initInputBar(QString text, QString name, int row, int col);
    void initItemDelegate();
    void initSettings();
    void saveSettings();
    void confSettings();
    void autoChange(QModelIndex index);
    void autoInput();
    void autoCal();
    void change();
    void recvShowEvent();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QGridLayout *btnLayout;
    QTableView *view;
    BoxQModel *mView;
    QComboBox *cnvtBox;
    QMap<QString, QDoubleSpinBox*> inputs;

    QStringList wires;
    QStringList units;
    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETDCR_H
