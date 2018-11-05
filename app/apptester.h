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
#include <QPushButton>
#include <QHeaderView>
#include <QTableWidget>
#include <QRadioButton>

#include "main.h"
#include "appsystem.h"
#include "apppermit.h"
#include "boxqlabel.h"
#include "boxqimage.h"
#include "sqlimport.h"
#include "appsignin.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetmag.h"
#include "typsetinr.h"
#include "typsetacw.h"
#include "typsetimp.h"

const QString largeTM = "<p style='font-size:11pt;color:#FFFFFF;' align='left'>&nbsp;%1</p>";
const QString largeHD = "<p style='font-size:18pt;color:#FFFF00;' align='left'>&nbsp;%1</p>";
const QString titleAP = "<p style='font-size:20pt;color:#FFFF00;' align='center'>%1</p>";
const QString largeEN = "<p style='font-size:11pt;color:#666666;' align='left'>&nbsp;%1</p>";
const QString largeER = "<p style='font-size:13pt;color:#FFFF00;' align='center'>%1</p>";
const QString judgeOK = "<p style='font-size:72pt;color:#00FF00;'align='center'><b>OK</b></p>";
const QString judgeNG = "<p style='font-size:72pt;color:#FF0000;'align='center'><b>NG</b></p>";
const QString judgeON = "<p style='font-size:72pt;color:#FFFF00;'align='center'><b>%1</b></p>";
const QString judgeDC = "<p style='font-size:72pt;color:#FF0000;'align='center'><b>%1</b></p>";

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
    void initButtonBar();
    void initHistogram();
    void initWireColor();
    void initOtherParm();
    void initSettings();
    void initSetDCR();
    void initSetMAG();
    void initSetCCW();
    void initSetINR();
    void initSetACW();
    void initSetIMP();
    void initSetHAL();
    void initSetLOD();
    void initSetBMF();
    void initQuality();
    void clickTest();
    void clickStop();
    void clickReset();
    void clickButton();
    void updateShow();
    void updateTime();
    void updateTest();
    void recvErrMsg(QTmpMap msg);
    void recvTmpMsg(QTmpMap msg);
    void recvLedMsg(QTmpMap msg);
    void recvNewMsg(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *mView;
    QTableWidget *wView;
    QHBoxLayout *layout;
    QVBoxLayout *blayout;
    QPushButton *btnTest;
    QPushButton *btnZero;
    QFrame *wFrame;
    QList<BoxQLabel*> magWave;
    QList<BoxQLabel*> impWave;
    QList<QLabel*> magText;
    QList<QLabel*> impText;
    QList<QLabel*> colors;
    QLabel *workText;
    QLabel *realText;
    QLabel *typeText;
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
    BoxQLabel *rate;
    QRadioButton *btnL;
    QRadioButton *btnR;

    QPushButton *btnHome;
    QPushButton *btnConf;
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
    QTmpMap tmpItem;
    QTmpMap tmpParm;
};

#endif // APPTESTER_H
