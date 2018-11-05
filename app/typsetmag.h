/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       反嵌配置界面
*******************************************************************************/
#ifndef TYPSETMAG_H
#define TYPSETMAG_H

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QListView>
#include <QComboBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>

#include "main.h"
#include "boxqmodel.h"
#include "boxdouble.h"
#include "boxqitems.h"
#include "boxqlabel.h"

#define MAG_SIZE 400

#define CACHEMAG 0x10  // 离散数据长度

#define CHECKMAG 0x00  // 反嵌测试
#define PORTMAG1 0x01  // 端口一
#define PORTMAG2 0x02  // 端口二
#define UPPERMAG 0x03  // 反嵌上限
#define AREAMAG1 0x04
#define FREQMAG1 0x05
#define AREAMAG2 0x06
#define FREQMAG2 0x07
#define WAVEMAG1 0x08

class TypSetMag : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetMag(QWidget *parent = 0);

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
    void autoInput();
    void sample();
    void change();
    void recvShowEvent();
    void recvNewMsg(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableWidget *wView;
    QList<QCheckBox*> checks;
    QList<QComboBox*> ports1;
    QList<QComboBox*> ports2;
    QList<QSpinBox*> uppers;
    QList<BoxQLabel*> waves;
    QRadioButton *btnWorkL;
    QRadioButton *btnWorkR;
    QComboBox *turnBox;
    QStringList titles;

    QTmpMap areasL;
    QTmpMap freqsL;
    QTmpMap areasR;
    QTmpMap freqsR;
    QVariantMap config;
    QVariantMap tmpMap;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    QTmpMap wvdata;
    bool isInit;
    bool isSave;
};

#endif // TYPSETMAG_H
