/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       霍尔配置界面
*******************************************************************************/
#ifndef TYPSETHAL_H
#define TYPSETHAL_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QGroupBox>
#include <QListView>
#include <QLineEdit>
#include <QCheckBox>
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

#define CACHEHAL 0x10

class TypSetHal : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetHal(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initWaveBar();
    void initButtonBar();
    void initItemDelegate();
    void initSettings();
    void saveSettings();
    void confSettings();
    void autoChange();
    void autoInput();
    void drawWave();
    void change();
    void recvAppMsg(QTmpMap msg);
    void recvShowEvent();
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *iView;
    BoxQModel *iMode;
    QTableView *wView;
    BoxQModel *wMode;
    BoxQImage *hWave;
    QSpinBox *line;
    QComboBox *comb;
    QStringList waves;
    QVariantMap config;
    QVariantMap tmpMap;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
};

#endif // TYPSETHAL_H
