/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#ifndef APPTESTER_H
#define APPTESTER_H

#include <qmath.h>
#include <QTime>
#include <QLabel>
#include <QTimer>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QScrollBar>
#include <QPushButton>
#include <QHeaderView>
#include <QTableWidget>
#include <QRadioButton>

#include "main.h"
#include "appsignin.h"
#include "appsystem.h"
#include "apppermit.h"
#include "boxqimage.h"
#include "sqlimport.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetmag.h"
#include "typsetinr.h"
#include "typsetacw.h"
#include "typsetimp.h"
#include "typsetpwr.h"
#include "typsetind.h"

class AppTester : public QWidget
{
    Q_OBJECT
public:
    explicit AppTester(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initTestBar();
    void initLogoBar();
    void initWorkBar();
    void initRealBar();
    void initWaveMAG();
    void initWaveIMP();
    void initWaveBMF();
    void initWaveHAL();
    void initTypeBar();
    void initWaveAll();
    void initButtonBar();
    void initHistogram();
    void initWorkState();
    void initWireColor();
    void initOtherParm();
    void initSettings();
    void initSetDCR();
    void initSetMAG();
    void initSetCCW();
    void initSetINR();
    void initSetACW();
    void initSetACW2();
    void initSetDCW();
    void initSetIMP();
    void initSetPWR();
    void initSetIND();
    void initSetHAL();
    void initSetLOD();
    void initSetNLD();
    void initSetBMF();
    void initSetLVS();
    void insertItem(int item, int numb);
    void initQuality();
    void clickType();
    void clickTest();
    void clickStop();
    void clickWave();
    void clickReset();
    void clickButton();
    void updateType();
    void updateWave();
    void updateShow();
    void updateTime();
    void updateTest();
    void recvErrMsg(QTmpMap msg);
    void recvTmpMsg(QTmpMap msg);
    void recvLedMsg(QTmpMap msg);
    void recvFGWave(QTmpMap msg);
    void recvManual(QTmpMap msg);
    void recvNewMsg(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *mView;
    QTableWidget *wView;
    QHBoxLayout *layout;
    QVBoxLayout *blayout;
    QMap<QString, QPushButton*> btns;
    QMap<QString, QLabel*> labels;
    QPushButton *btnHome;
    QPushButton *btnConf;
    QPushButton *btnZero;
    QPushButton *btnTest;
    QPushButton *btnStop;
    QRadioButton *btnL;
    QRadioButton *btnR;
    QFrame *wFrame;
    BoxQImage *allWave;
    QList<BoxQImage*> magWave;
    QList<BoxQImage*> impWave;
    QList<QLabel*> magText;
    QList<QLabel*> impText;
    QList<QLabel*> colors;
    QLabel *workText;
    QLabel *realText;
    QComboBox *typeText;
    QLabel *userText;
    QLabel *signText;
    QLabel *codeText;
    QLabel *testText;
    QLabel *dateText;
    QLabel *timeText;
    QLabel *tempText;
    QLabel *warnText;

    quint32 tmpRow;
    quint32 testOK;
    quint32 testQu;
    quint32 tmpTime;

    QTmpMap tmpMsg;
    QTmpMap tmpSet;
    QList<QTmpMap> tmpView;
    QString tmpStr;
    QElapsedTimer ms;

    BoxQImage *bWave;
    BoxQImage *hWave;
    BoxQImage *bodys;
    QList<QVariantMap> tmpQua;
    QTmpMap tmpNG;
    QTmpMap tmpSave;
    QTmpMap tmpItem;
    QTmpMap tmpParm;
    QVariantMap tmpMap;
    QVariantMap tmpTyp;
    bool isInit;
    QString strv;
};

#endif // APPTESTER_H
