/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪主界面
*******************************************************************************/
#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QLayout>
#include <QThread>
#include <QDateTime>
#include <QMainWindow>
#include <QStackedWidget>
#include <QProgressDialog>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "tcpsocket.h"
#include "udpsocket.h"

#include "appauthor.h"
#include "appsignin.h"
#include "appsystem.h"
#include "appioctrl.h"
#include "appbackup.h"
#include "applogger.h"
#include "appmaster.h"
#include "apppermit.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetmag.h"
#include "typsetinr.h"
#include "typsetacw.h"
#include "typsetimp.h"
#include "typsetind.h"
#include "typsethal.h"
#include "typsetlod.h"
#include "typsetbmf.h"
#include "sqlcreate.h"
#include "sqlimport.h"
#include "sqlrecord.h"
#include "sqlupload.h"
#include "sqlexport.h"
#include "apptester.h"

#include "boxdialog.h"

#include "devplctrl.h"
#include "devinvert.h"
#include "devdriver.h"

#include "main.h"
#ifdef __arm__
#define BTN_WIDTH 240
#define BTN_LENTH 72
#else
#define BTN_WIDTH 240
#define BTN_LENTH 72
#endif
class AppWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AppWindow(QWidget *parent = 0);
    ~AppWindow();
signals:
    void sendUdpMsg(QTmpMap msg);
    void sendNetMsg(QTmpMap msg);
    void sendAppMsg(QTmpMap msg);
private slots:
    int initUI();
    int initTitle();
    int initLayout();
    int initAuthor();
    int initDevice();
    int initScreen();
    int initSqlDir();
    int initTester();
    int initSignin();
    int initSystem();
    int initIoCtrl();
    int initMaster();
    int initPermit();
    int initBackup();
    int initLogger();
    int initConfig();
    int initSetDcr();
    int initSetMag();
    int initSetInr();
    int initSetAcw();
    int initSetImp();
    int initSetInd();
    int initSetHal();
    int initSetLod();
    int initSetBmf();
    int initImport();
    int initExport();
    int initRecord();
    int initUpload();
    int readBackup();
    int readSystem();
    int readConfig();
    int sendSqlite();
    int sendSignin();
    int initSocket();
    int initSerial();
    int initThread();
    int initWidget(int numb, int form, QString name, QString mark, QWidget *app);
    int taskThread();
    int taskLoopSignin();
    int taskClearData();
    int taskCheckPlay();
    int taskCheckCode();
    int taskStartView();
    int taskToolIobrd();
    int taskStartWait();
    int taskStartTest();
    int taskClearCtrl();
    int taskStartSave();
    int taskStartBeep();
    int taskClearBeep();
    int taskResetTest();
    int taskCheckStop();
    int testThread();
    int testClearData();
    int testToolServo();
    int testWaitServo();
    int testStopServo();
    int testToolInvrt();
    int testStopInvrt();
    int testStartSend();
    int testStartTest();
    int getNextItem();
    int taskConfig();
    int recvIoCtrl(int key, int work);
    void showTester();
    void showBarCode();
    void showBoxPop(QString text, int t);
    void saveBackup(QTmpMap msg);
    void saveSqlite(QTmpMap msg);
    void saveConfig(QTmpMap msg);
    void clickButtons();
    bool checkAction(QString msg);
    bool screensSave(QString msg);
    void screensShow(QString msg);
    void recvAppShow(QString msg);
    void recvAppPrep();
    virtual void cloudAntimation();
    virtual void closeEvent(QCloseEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    void loopBoxbar();
    void warnningString(QString dat);
    void recvSqlMsg(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    void recvXmlMap(QVariantMap msg);
    void recvAppMap(QVariantMap msg);
    void recvUdpMsg(QByteArray msg);
    void sendUdpStr(QByteArray msg);
    void recvNewMsg(QString msg);
    void recvStaMsg(QString msg);
    void wait(int ms);
    void calcHALL(QString msg);
    void calcLOAD(QString msg);
    void calcBEMF(QString msg);
    double calcNoun(QList<double> tmp);
private:
    QStackedWidget *stack;
    QVBoxLayout *btnLayout;
    QFrame *btnFrame;
    QList<QPushButton*> buttons;
    BoxDialog *boxbar;

    DevPlctrl mbdktL;
    DevPlctrl mbdktR;
    DevDriver servoL;
    DevDriver servoR;
    DevInvert invert;

    QThread *sql;

    QString verNumb;

    int speed;
    QList<int> speeds;
    int preindex;
    int currMode;
    int currTask;
    int currTest;
    int codeShift;
    int taskShift;
    int testShift;
    int tempShift;
    int currItem;
    int isudp;
    int isok;
    qint64 timeUdp;
    qint64 timeOut;
    qint64 timeTst;
    qint64 timeRsl;
    qint64 timeDet;
    qint64 timeMag;
    qint64 timeImp;
    QElapsedTimer t;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    QTmpMap tmpSave;

    QString barcode;
    QString tmpcode;
    QTimer *scanner;
    quint8 station;
    QVariantMap tmpMap;
    QTmpMap tstMap;
    QList<QVariantMap> bufwin;
    quint32 ioHex;
    bool isChange;
    bool isVacuum;

    QTimer *loading;
};

#endif // APPWINDOW_H
