/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       负载配置界面
*******************************************************************************/
#ifndef TYPSETLOD_H
#define TYPSETLOD_H

#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QShowEvent>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>

#include "main.h"
#include "boxqmodel.h"
#include "boxqitems.h"
#include "boxdouble.h"
#include "boxqimage.h"

#define CACHELOD 0x10

class TypSetLod : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetLod(QWidget *parent = 0);

signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initTimeBar();
    void initButtonBar();
    void initItemDelegate();
    void initSettings();
    void saveSettings();
    void confSettings();
    void autoChange();
    void autoInput();
    void drawWave();
    void change();
    void recvShowEvent();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *iView;
    BoxQModel *iMode;
    QTableView *vView;
    BoxQModel *vMode;
    QTableView *pView;
    BoxQModel *pMode;
    QTableView *mView;
    BoxQModel *mMode;
    QTableView *tView;
    BoxQModel *tMode;
    QStringList drivers;
    QStringList sources;
    QStringList speeds;
    QStringList turns;
    QStringList modes;
    QStringList freqs;
    QVariantMap config;
    QVariantMap tmpMap;
    BoxQImage *wave;
    QCheckBox *modeBox;
    QSpinBox *speedBox;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETLOD_H
