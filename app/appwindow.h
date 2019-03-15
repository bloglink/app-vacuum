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
#include <QLibrary>
#include <QDateTime>
#include <QMainWindow>
#include <QInputDialog>
#include <QStackedWidget>
#include <QProgressDialog>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QUdpSocket>
#include <QDataStream>
#include <QDomElement>
#include <QDomText>
#include <QProgressBar>

#include "libexport.h"
#include "appauthor.h"
#include "appsignin.h"
#include "appsystem.h"
#include "appbackup.h"
#include "apppermit.h"
#include "apprepair.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetmag.h"
#include "typsetinr.h"
#include "typsetacw.h"
#include "typsetimp.h"
#include "typsetpwr.h"
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

#include "main.h"
#ifdef __arm__
#define BTN_WIDTH 240
#define BTN_LENTH 72
#else
#define BTN_WIDTH 240
#define BTN_LENTH 72
#endif

#define TIME_OUT 1000

class AppWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AppWindow(QWidget *parent = 0);
    ~AppWindow();
signals:
    void sendAppMsg(QTmpMap msg);
    void sendReady(QByteArray msg);
private slots:
    int initUI();
    int initCreate();
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
    int initRepair();
    int initConfig();
    int initSetDcr();
    int initSetMag();
    int initSetInr();
    int initSetAcw();
    int initSetDcw();
    int initSetImp();
    int initSetPwr();
    int initSetInd();
    int initSetHal();
    int initSetLod();
    int initSetNld();
    int initSetBmf();
    int initSetLvs();
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
    int initLibWidget(int numb, int form, QString name, QString mark, QString lib);
    int taskThread();
    int taskLoopSignin();
    int taskClearData();
    int taskCheckPlay();
    int taskCheckCode();
    int taskStartView();
    int taskToolIobrd();
    int taskStartWait();
    int taskOpenServo();
    int taskStartTest();
    int taskClearCtrl();
    int taskStopMotor();
    int taskStopServo();
    int taskQuitServo();
    int taskClearWait();
    int taskCheckSave();
    int taskStartSave();
    int taskClearWarn();
    int taskStartBeep();
    int taskClearBeep();
    int taskWaitReset();
    int taskResetTest();
    int taskCheckStop();
    int testThread();
    int testClearData();
    int testToolIocan();
    int testStopIocan();
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
    QVariantMap taskSerial(QString taskname, QString taskwork, QVariant taskdata);
    void showTester();
    void showBarCode();
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
    void display(int item, int numb, int mode, QString str, int work);
    void loopBoxbar();
    void warnningString(QString dat);
    void recvSqlMsg(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    void recvLibMsg(QVariantMap msg);
    void recvXmlMap(QVariantMap msg);
    void recvAppMap(QVariantMap msg);
    void recvSocket(QByteArray msg);
    void sendUdpStr(QByteArray msg);
    void recvNewMsg(QString msg);
    void recvStaMsg(QString msg);
    void recvErrMsg(QString msg);
    void recvPwrMsg(QString msg);
    void wait(int ms);
    void calcHALL(QString msg);
    void calcLOAD(QString msg, int ext);
    void calcBEMF(QString msg);
    double calcNoun(QList<double> tmp);
    void test();
private:
    QStackedWidget *stack;
    QVBoxLayout *btnLayout;
    QFrame *btnFrame;
    QList<QPushButton*> buttons;
    QProgressDialog *boxbar;
    QMessageBox *boxsave;

    QThread *sql;

    int preindex;
    int currTask;
    int codeShift;
    int taskShift;
    int testShift;
    int tempShift;
    int prevShift;
    int currItem;
    int isok;
    qint64 timeRsl;
    qint64 timeMag;
    qint64 timeImp;
    QElapsedTimer t;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    QTmpMap tmpSave;
    QMap<int, int> tmpItem;

    QString barcode;
    QString tmpcode;
    QTimer *scanner;
    quint8 station;
    QList<QVariantMap> bufwin;
    quint32 ioHexL;
    quint32 ioHexR;
    quint32 prvHex;
    int turnBuff;
    quint16 ioSave;
    QString strLoad;
    QList<int> buffrate;
    QVariantMap taskparm;
    QVariantMap testparm;
    QVariantMap itemparm;
    QList<double> tempvolt;
};

#endif // APPWINDOW_H
