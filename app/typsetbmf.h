/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       反电动势配置界面
*******************************************************************************/
#ifndef TYPSETBMF_H
#define TYPSETBMF_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QGroupBox>
#include <QShowEvent>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>

#include "main.h"
#include "boxqmodel.h"
#include "boxqitems.h"
#include "boxdouble.h"

#define CACHEBMF 0x10

class TypSetBmf : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetBmf(QWidget *parent = 0);
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
    QTableView *iView;
    BoxQModel *iMode;
    QTableView *pView;
    BoxQModel *pMode;
    QStringList comps;
    QStringList turns;
    QStringList sques;
    QVariantMap config;
    QVariantMap tmpMap;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETBMF_H
