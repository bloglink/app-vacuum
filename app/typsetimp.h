/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       匝间配置界面
*******************************************************************************/
#ifndef TYPSETIMP_H
#define TYPSETIMP_H

#include <QTimer>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QGroupBox>
#include <QCheckBox>
#include <QShowEvent>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>

#include "main.h"
#include "appbackup.h"
#include "appsystem.h"
#include "boxqmodel.h"
#include "boxqitems.h"
#include "boxdouble.h"
#include "boxqlabel.h"

#define IMP_SIZE 400

#define CACHEIMP 0x10  // 离散数据长度

#define CHECKIMP 0x00  // 反嵌测试
#define PORTIMP1 0x01  // 端口一
#define PORTIMP2 0x02  // 端口二
#define VOLTIMP1 0x03  // 电压
#define TIMEIMP1 0x04
#define AREAIMP1 0x05
#define AREAIMP2 0x06
#define DIFFIMP1 0x07
#define DIFFIMP2 0x08
#define FLUTIMP1 0x09
#define PHSEIMP1 0x0A
#define FREQIMP1 0x0B
#define FREQIMP2 0x0C
#define FROMIMP1 0x0D
#define STOPIMP1 0x0E

class TypSetImp : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetImp(QWidget *parent = 0);
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
    void lineUpdate();
    void autoInput();
    void clickView();
    void swapVacuo();
    void swapWave();
    void change();
    void sample();
    void sampling();
    void sampleAdd();
    void sampleDel();
    void sampleCalc();
    void sampleOver();
    void sampleWait();
    void sampleSwitch();
    void recvWarn();
    void recvOver();
    void recvParm(QString dat);
    void recvWave(QStringList ws);
    void recvNewMsg(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    void recvShowEvent();
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableView *view;
    BoxQModel *mView;
    QTableWidget *wView;
    QList<BoxQLabel*> waves;
    QCheckBox *modeBox;
    QCheckBox *vacuoBox;
    QCheckBox *earthBox;
    QCheckBox *powerBox;
    QCheckBox *modeBox2;
    QLabel *textBox;
    QSpinBox *stepBox;
    QRadioButton *btnWorkL;
    QRadioButton *btnWorkR;
    QList<QTmpMap> waveCopys;
    QMap<QString, QCheckBox*> boxs;
    QMap<QString, QPushButton*> btns;

    QVariantMap tmpMap;
    QVariantMap config;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    QTmpMap wvdata;
    bool isInit;
    QVariantMap buftmp;
    int isChange;
    QTimer *timer;
    QVector<int> x;
    QVariantMap tmp;
};

#endif // TYPSETIMP_H
