/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪主界面
*******************************************************************************/
#include "appwindow.h"

typedef int (AppWindow::*pClass)(void);
QList<pClass> taskBuf;
QList<pClass> testBuf;

QLibrary *libSerial;

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    t.start();
    initUI();
}

AppWindow::~AppWindow()
{
    sql->quit();
    sql->wait();
}

int AppWindow::initUI()
{
    initCreate();
    initLayout();
    initSqlDir();
    initAuthor();
    QTimer::singleShot(100, this, SLOT(initScreen()));
    return Qt::Key_Away;
}

int AppWindow::initCreate()
{
    new libExport(this);

    char s_month[5];
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    int month, day, year;
    sscanf((__DATE__), "%s %d %d", s_month, &day, &year);
    month = (strstr(month_names, s_month)-month_names)/3+1;

    static const QDate makeDate(year, month, day);
    static const QTime makeTime = QTime::fromString(__TIME__, "hh:mm:ss");
    static const QDateTime mDateTime(makeDate, makeTime);
    QString ver = QString("V-2.3.%1").arg(mDateTime.toString("yyMMdd-hhmm"));
    this->setWindowTitle(tr("电机综合测试仪%1").arg(ver));
    taskparm.insert("tasksoft", ver);
    qWarning() << QString("app create:V-2.3.%1").arg(mDateTime.toString("yyMMdd-hhmm"));
    return Qt::Key_Away;
}

int AppWindow::initLayout()
{
    preindex = 0;
    stack = new QStackedWidget(this);
    btnLayout = new QVBoxLayout;
    btnLayout->setContentsMargins(0, 9, 0, 9);
    btnLayout->setSpacing(0);

    btnFrame = new QFrame(this);
    btnFrame->setLayout(btnLayout);
    btnFrame->setFixedWidth(BTN_WIDTH);
    btnFrame->hide();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(btnFrame);
    layout->addWidget(stack);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);

    this->setCentralWidget(frame);
    return Qt::Key_Away;
}

int AppWindow::initAuthor()
{
    AppAuthor *app = new AppAuthor(this);
    return initWidget(nAuthor, 0, "author", tr("返回主页"), app);
}

int AppWindow::initScreen()
{
    boxbar = new QProgressDialog(this);
    boxbar->setCancelButton(NULL);
    boxbar->setWindowFlags(Qt::FramelessWindowHint);
    boxbar->setFixedSize(240, 72);
    boxbar->move(this->width()/2 - 120, this->height()/2 - 24);
    boxbar->hide();

    QProgressDialog *tmpbox = new QProgressDialog(this);
    tmpbox->setCancelButton(NULL);
    tmpbox->setWindowFlags(Qt::FramelessWindowHint);
    tmpbox->setFixedSize(240, 72);
    tmpbox->move(this->width()/2 - 120, this->height()/2 - 24);
    tmpbox->show();

    QMap<int, pClass> initMap;
    QStringList names;
    initMap[names.size()] = &AppWindow::initTester;
    names << tr("正在初始化测试界面");
    initMap[names.size()] = &AppWindow::initLogger;
    names << tr("正在初始化调试信息");
    initMap[names.size()] = &AppWindow::initSignin;
    names << tr("正在初始化登录界面");
    initMap[names.size()] = &AppWindow::initSystem;
    names << tr("正在初始化系统设置");
    initMap[names.size()] = &AppWindow::initIoCtrl;
    names << tr("正在初始化动作设置");
    initMap[names.size()] = &AppWindow::initMaster;
    names << tr("正在初始化用户管理");
    initMap[names.size()] = &AppWindow::initPermit;
    names << tr("正在初始化权限管理");
    initMap[names.size()] = &AppWindow::initBackup;
    names << tr("正在初始化后台设置");
    initMap[names.size()] = &AppWindow::initRepair;
    names << tr("正在初始化日常保养");
    initMap[names.size()] = &AppWindow::initConfig;
    names << tr("正在初始化型号管理");
    initMap[names.size()] = &AppWindow::initSetDcr;
    names << tr("正在初始化电阻配置");
    initMap[names.size()] = &AppWindow::initSetMag;
    names << tr("正在初始化反嵌配置");
    initMap[names.size()] = &AppWindow::initSetInr;
    names << tr("正在初始化绝缘配置");
    initMap[names.size()] = &AppWindow::initSetAcw;
    names << tr("正在初始化交耐配置");
    initMap[names.size()] = &AppWindow::initSetDcw;
    names << tr("正在初始化直耐配置");
    initMap[names.size()] = &AppWindow::initSetImp;
    names << tr("正在初始化匝间配置");
    initMap[names.size()] = &AppWindow::initSetPwr;
    names << tr("正在初始化电参配置");
    initMap[names.size()] = &AppWindow::initSetInd;
    names << tr("正在初始化电感配置");
    initMap[names.size()] = &AppWindow::initSetLck;
    names << tr("正在初始化堵转配置");
    initMap[names.size()] = &AppWindow::initSetLvs;
    names << tr("正在初始化低启配置");
    initMap[names.size()] = &AppWindow::initSetHal;
    names << tr("正在初始化霍尔配置");
    initMap[names.size()] = &AppWindow::initSetLod;
    names << tr("正在初始化负载配置");
    initMap[names.size()] = &AppWindow::initSetNld;
    names << tr("正在初始化空载配置");
    initMap[names.size()] = &AppWindow::initSetBmf;
    names << tr("正在初始化BEMF配置");
    initMap[names.size()] = &AppWindow::initSetLph;
    names << tr("正在初始化低启配置");
    initMap[names.size()] = &AppWindow::initImport;
    names << tr("正在初始化数据存储");
    initMap[names.size()] = &AppWindow::initExport;
    names << tr("正在初始化数据导出");
    initMap[names.size()] = &AppWindow::initRecord;
    names << tr("正在初始化数据管理");
    initMap[names.size()] = &AppWindow::initUpload;
    names << tr("正在初始化上传管理");
    initMap[names.size()] = &AppWindow::readBackup;
    names << tr("正在读取后台数据库");
    initMap[names.size()] = &AppWindow::readSystem;
    names << tr("正在读取配置数据库");
    initMap[names.size()] = &AppWindow::readConfig;
    names << tr("正在读取型号数据库");
    initMap[names.size()] = &AppWindow::sendSqlite;
    names << tr("正在分发数据库配置");
    initMap[names.size()] = &AppWindow::initSocket;
    names << tr("正在初始化网络连接");
    initMap[names.size()] = &AppWindow::initSerial;
    names << tr("正在初始化串口连接");
    initMap[names.size()] = &AppWindow::sendSignin;
    names << tr("正在初始化登录验证");
    initMap[names.size()] = &AppWindow::initThread;
    names << tr("正在初始化系统线程");

    for (int i=0; i < names.size(); i++) {
        tmpbox->setLabelText(names.at(i));
        wait(10);
        (this->*initMap[i])();
        tmpbox->setValue((i+1)*100/initMap.size());
    }
    tmpbox->hide();
    btnLayout->addStretch();
    return Qt::Key_Away;
}

int AppWindow:: initSqlDir()
{  // 创建并打开数据库
    SqlCreate *app = new SqlCreate(this);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    app->initSqlDir();
    return Qt::Key_Away;
}

int AppWindow::initTester()
{
    AppTester *app = new AppTester(this);
    return initWidget(nTester, 0, "tester", tr("测试界面"), app);
}

int AppWindow::initLogger()
{
    return initLibWidget(nLogger, 1, "logger", tr("调试信息"), "./lib/libLogger");
}

int AppWindow::initSignin()
{
    AppSignin *app = new AppSignin(this);
    return initWidget(nSignin, 1, "signin", tr("用户登录"), app);
}

int AppWindow::initSystem()
{
    AppSystem *app = new AppSystem(this);
    return initWidget(nSystem, 1, "system", tr("系统设置"), app);
}

int AppWindow::initIoCtrl()
{
    return initLibWidget(nIoctrl, 1, "ioctrl", tr("动作设置"), "./lib/libOutput");
}

int AppWindow::initMaster()
{
    return initLibWidget(nMaster, 1, "master", tr("用户管理"), "./lib/libMaster");
}

int AppWindow::initPermit()
{
    AppPermit *app = new AppPermit(this);
    return initWidget(nPermit, 1, "permit", tr("权限管理"), app);
}

int AppWindow::initBackup()
{
    AppBackup *app = new AppBackup(this);
    return initWidget(nBackup, 1, "backup", tr("后台管理"), app);
}

int AppWindow::initRepair()
{
    AppRepair *app = new AppRepair(this);
    return initWidget(nRepair, 1, "repair", tr("日常保养"), app);
}

int AppWindow::initConfig()
{
    TypConfig *app = new TypConfig(this);
    initWidget(nConfig, 2, "config", tr("型号管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetDcr()
{
    TypSetDcr *app = new TypSetDcr(this);
    return initWidget(nSetDCR, 2, "setdcr", tr("电阻配置"), app);
}

int AppWindow::initSetMag()
{
    TypSetMag *app = new TypSetMag(this);
    return initWidget(nSetMAG, 2, "setmag", tr("反嵌配置"), app);
}

int AppWindow::initSetInr()
{
    TypSetInr *app = new TypSetInr(this);
    return initWidget(nSetINR, 2, "setinr", tr("绝缘配置"), app);
}

int AppWindow::initSetAcw()
{
    TypSetAcw *app = new TypSetAcw(this);
    return initWidget(nSetACW, 2, "setacw", tr("交耐配置"), app);
}

int AppWindow::initSetDcw()
{
    TypSetAcw *app = new TypSetAcw(this);
    return initWidget(nSetDCW, 2, "setdcw", tr("直耐配置"), app);
}

int AppWindow::initSetImp()
{
    TypSetImp *app = new TypSetImp(this);
    return initWidget(nSetIMP, 2, "setimp", tr("匝间配置"), app);
}

int AppWindow::initSetPwr()
{
    TypSetPwr *app = new TypSetPwr(this);
    return initWidget(nSetPWR, 2, "setpwr", tr("电参配置"), app);
}

int AppWindow::initSetInd()
{
    TypSetInd *app = new TypSetInd(this);
    return initWidget(nSetIND, 2, "setind", tr("电感配置"), app);
}

int AppWindow::initSetLck()
{
    TypSetLVS *app = new TypSetLVS(this);
    return initWidget(nSetLCK, 2, "setlck", tr("堵转配置"), app);
}

int AppWindow::initSetLvs()
{
    TypSetLVS *app = new TypSetLVS(this);
    return initWidget(nSetLVS, 2, "setlvs", tr("低启配置"), app);
}

int AppWindow::initSetHal()
{
    TypSetHal *app = new TypSetHal(this);
    return initWidget(nSetHAL, 2, "sethal", tr("霍尔配置"), app);
}

int AppWindow::initSetLod()
{
    TypSetLod *app = new TypSetLod(this);
    return initWidget(nSetLOD, 2, "setlod", tr("负载配置"), app);
}

int AppWindow::initSetNld()
{
    TypSetLod *app = new TypSetLod(this);
    return initWidget(nSetNLD, 2, "setnld", tr("空载配置"), app);
}

int AppWindow::initSetBmf()
{
    TypSetBmf *app = new TypSetBmf(this);
    return initWidget(nSetEMF, 2, "setbmf", tr("BEMF配置"), app);
}

int AppWindow::initSetLph()
{
    TypSetLod *app = new TypSetLod(this);
    return initWidget(0x0F, 2, "setlph", tr("缺相配置"), app);
}

int AppWindow::initImport()
{
    sql = new QThread(this);
    sql->start();
    SqlImport *app = new SqlImport;
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    app->moveToThread(sql);

    return Qt::Key_Away;
}

int AppWindow::initExport()
{
    SqlExport *app = new SqlExport;
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    app->moveToThread(sql);

    return Qt::Key_Away;
}

int AppWindow::initRecord()
{
    SqlRecord *app = new SqlRecord(this);
    return initWidget(nRecord, 3, "record", tr("数据管理"), app);
}

int AppWindow::initUpload()
{
    SqlUpload *app = new SqlUpload(this);
    return initWidget(nUpload, 3, "upload", tr("数据上传"), app);
}

int AppWindow::readBackup()
{
    QString name = "aip_backup";
    QSqlQuery query(QSqlDatabase::database("backup"));
    query.exec("select * from aip_backup");
    while (query.next()) {
        int uuid = query.value(0).toInt();
        tmpSet[uuid] = query.value(1).toString();
    }
    query.clear();
    qDebug() << "app read:" << name;
    return Qt::Key_Away;
}

int AppWindow::readSystem()
{
    QString name = "aip_system";
    QSqlQuery query(QSqlDatabase::database("system"));
    query.exec("select * from aip_system");
    while (query.next()) {
        int uuid = query.value(0).toInt();
        tmpSet.insert(uuid, query.value(1).toString());
    }
    query.clear();
    qDebug() << "app read:" << name;
    return Qt::Key_Away;
}

int AppWindow::readConfig()
{
    QSqlQuery query(QSqlDatabase::database("config"));
    query.exec("select name from sqlite_master where type='table' order by name");
    QTmpMap tmptyp;
    while (query.next()) {
        QString t = query.value(0).toString();
        QString numb = t.mid(1, 4);
        tmptyp.insert(numb.toInt(), t.mid(6, NAME_SIZE));
    }
    int r = tmpSet.value(DataFile).toInt();
    QString name = tmptyp.value(r).toString();
    tmpSet.insert(DataType, name);
    query.exec(QString("select * from '%1'").arg(tr("T%1_%2").arg(r, 4, 10, QChar('0')).arg(name)));
    while (query.next()) {
        int uuid = query.value(0).toInt();
        tmpSet.insert(uuid, query.value(1).toString());
    }
    query.clear();
    qDebug() << "app read:" << name;
    return Qt::Key_Away;
}

int AppWindow::sendSqlite()
{
    tmpSet.insert(Qt::Key_0, Qt::Key_Copy);
    tmpSet.insert(DataSoft, taskparm.value("tasksoft").toString());
    emit sendAppMsg(tmpSet);

    for (int i=0; i < bufwin.size(); i++) {
        tmpMsg.insert(i, bufwin.at(i));
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Memo);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    return Qt::Key_Away;
}

int AppWindow::sendSignin()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Game);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    int temp = tmpSet.value((3000 + Qt::Key_0)).toInt();  // 临时数据地址
    if (tmpSet.value(temp + TEMPSIGN).toInt() != 1) {
        tmpMsg.insert(Qt::Key_0, Qt::Key_View);
        tmpMsg.insert(Qt::Key_1, "signin");
        recvAppMsg(tmpMsg);
        tmpMsg.clear();
    }
    return Qt::Key_Away;
}

int AppWindow::initSocket()
{
    QLibrary *libSocket = new QLibrary("./lib/libSocket");
    if (!libSocket->load())
        qWarning() << libSocket->errorString();
    typedef QUdpSocket* (*CSocket)();
    CSocket fun = (CSocket)libSocket->resolve("init");
    QUdpSocket *socket = fun();
    connect(socket, SIGNAL(readReady(QByteArray)), this, SLOT(recvSocket(QByteArray)));
    connect(this, SIGNAL(sendReady(QByteArray)), socket, SIGNAL(sendReady(QByteArray)));

    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    QHostAddress host(tmpSet.value(back + backHost).toString());
    taskparm.insert("taskhost", host.toIPv4Address());
    socket->writeDatagram("6000", host, 6000);

    return Qt::Key_Away;
}

int AppWindow::initSerial()
{
    DevMServo *mservo = new DevMServo;
    mservo->setObjectName("turn");
    connect(mservo, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), mservo, SLOT(recvAppMap(QVariantMap)));
    mservo->moveToThread(sql);

    DevNServo *nservo = new DevNServo;
    nservo->setObjectName("read");
    connect(nservo, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), nservo, SLOT(recvAppMap(QVariantMap)));
    nservo->moveToThread(sql);

    DevNLogic *nlogic = new DevNLogic;
    nlogic->setObjectName("load");
    connect(nlogic, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), nlogic, SLOT(recvAppMap(QVariantMap)));
    mservo->moveToThread(sql);

    DevInvert *invert = new DevInvert;
    invert->setObjectName("loop");
    connect(invert, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), invert, SLOT(recvAppMap(QVariantMap)));
    invert->moveToThread(sql);

    DevWasher *washer = new DevWasher;
    washer->setObjectName("wash");
    connect(washer, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), washer, SLOT(recvAppMap(QVariantMap)));
    washer->moveToThread(sql);

    DevBarconde *devbar = new DevBarconde;
    devbar->setObjectName("code");
    connect(devbar, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), devbar, SLOT(recvAppMap(QVariantMap)));
    washer->moveToThread(sql);

    int taskback = tmpSet.value(1000 + Qt::Key_0).toInt();
    QString taskname = tmpSet.value(taskback + 0x20 + 0x0D).toString();
    qDebug() << taskname;
    if (!taskname.isEmpty()) {
        QVariantMap map;
        map.insert("taskname", taskname);
        map.insert("taskwork", "code");
        emit sendAppMap(map);

        map.clear();
    }

    noisetime = 0;
    noiseTimer = new QTimer(this);
    connect(noiseTimer, SIGNAL(timeout()), this, SLOT(noiseThread()));

    libSerial = new QLibrary("./lib/libSerial");
    if (!libSerial->load())
        qWarning() << libSerial->errorString();
    return Qt::Key_Away;
}

int AppWindow::initThread()
{
    taskBuf << &AppWindow::taskClearData << &AppWindow::taskCheckPlay
            << &AppWindow::taskCheckCode << &AppWindow::taskStartView
            << &AppWindow::taskToolIobrd << &AppWindow::taskStartWait
            << &AppWindow::taskStartTest
            << &AppWindow::taskClearCtrl
            << &AppWindow::taskStopServo
            << &AppWindow::taskClearWait << &AppWindow::taskCheckSave
            << &AppWindow::taskStartSave
            << &AppWindow::taskClearWarn << &AppWindow::taskStartBeep
            << &AppWindow::taskClearBeep << &AppWindow::taskWaitReset
            << &AppWindow::taskResetTest;

    testBuf << &AppWindow::testClearData << &AppWindow::testToolIocan
            << &AppWindow::testToolServo << &AppWindow::testToolInvrt
            << &AppWindow::testStartSend << &AppWindow::testWaitServo
            << &AppWindow::testStartTest
            << &AppWindow::testStopServo << &AppWindow::testStopIocan;

    QStringList items;
    items << "Conf" << "DCR" << "MAG" << "IR" << "ACW" << "DCW" << "IMP"
          << "PWR" << "IND" << "LCK" << "LVS" << "HALL" << "LOAD"
          << "NOLOAD" << "BEMF" << "LPH" << "ACW";
    for (int i=0; i < items.size(); i++) {
        taskparm.insert(QString::number(i), items.at(i));
    }

    QTimer *tasks = new QTimer(this);
    connect(tasks, SIGNAL(timeout()), this, SLOT(taskThread()));
    tasks->start(10);

    ioHexL = 0;
    ioHexR = 0;
    prvHex = 0;
    ioSave = 0;
    count = 0;
    currTask = 0;
    station = 0x13;
    isSampling = false;
    prevShift = Qt::Key_Meta;
    boxsave = NULL;
    return Qt::Key_Away;
}

int AppWindow::initWidget(int numb, int form, QString name, QString mark, QWidget *app)
{
    QVariantMap tmpMap;
    tmpMap.insert("numb", numb);
    tmpMap.insert("form", form);
    tmpMap.insert("name", name);
    tmpMap.insert("mark", mark);
    bufwin.append(tmpMap);
    tmpMap.clear();

    app->setObjectName(name);
    stack->addWidget(app);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));

    QPushButton *btnTester = new QPushButton(mark, this);
    btnTester->setFlat(true);
    btnTester->setCheckable(true);
    btnTester->setFixedSize(BTN_WIDTH, BTN_LENTH);
    btnTester->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnTester);
    btnLayout->addWidget(btnTester);
    btnTester->setObjectName(name);
    connect(btnTester, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));
    return Qt::Key_Away;
}

int AppWindow::initLibWidget(int numb, int form, QString name, QString mark, QString lib)
{
    QLibrary *tmplib = new QLibrary(lib);
    if (!tmplib->load()) {
        qWarning() << tmplib->errorString();
        return 0;
    }
    typedef QWidget* (*LibWidget)();
    LibWidget fun = (LibWidget)tmplib->resolve("init");
    QWidget *app = fun();
    app->setObjectName(name);
    stack->addWidget(app);
    connect(app, SIGNAL(sendAppMsg(QVariantMap)), this, SLOT(recvLibMsg(QVariantMap)));

    QVariantMap tmpMap;
    tmpMap.insert("numb", numb);
    tmpMap.insert("form", form);
    tmpMap.insert("name", name);
    tmpMap.insert("mark", mark);
    bufwin.append(tmpMap);
    tmpMap.clear();

    QPushButton *btnTester = new QPushButton(mark, this);
    btnTester->setFlat(true);
    btnTester->setCheckable(true);
    btnTester->setFixedSize(BTN_WIDTH, BTN_LENTH);
    btnTester->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnTester);
    btnLayout->addWidget(btnTester);
    btnTester->setObjectName(name);
    connect(btnTester, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));
    return Qt::Key_Away;
}

int AppWindow::taskThread()
{
    taskLoopSignin();

    tmpMsg.insert(Qt::Key_0, Qt::Key_Time);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    int ret = Qt::Key_Meta;
    if (currTask < taskBuf.size())
        ret = (this->*taskBuf[currTask])();
    if (ret == Qt::Key_Away) {
        currTask++;
        currTask = (currTask >= taskBuf.size()) ? 0 : currTask;
    }
    return ret;
}

int AppWindow::taskLoopSignin()
{  // 每2.5秒发一次登录或心跳，无回应报警
    int taskhand = taskparm.value("taskhand").toInt();
    int tasktime = taskparm.value("tasktime").toInt();
    taskhand = (tasktime == TIME_OUT) ? 0 : taskhand;
    tasktime = (tasktime == TIME_OUT) ? 0 : tasktime;
    if (tasktime%(TIME_OUT/4) == 0) {
        sendUdpStr((taskhand == 0) ? "6000" : "6030");
        tmpMsg.insert(Qt::Key_0, Qt::Key_WLAN);
        tmpMsg.insert(Qt::Key_1, taskhand);
        tmpMsg.insert(Qt::Key_2, tasktime * 100 / TIME_OUT);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
    taskparm.insert("taskhand", taskhand);
    taskparm.insert("tasktime", tasktime + 1);
    return Qt::Key_Away;
}

int AppWindow::taskClearData()
{
    isok = DATAOK;
    leek = 0;
    currTask = 0;
    currItem = 0;
    turnBuff = 0;
    barcode.clear();
    tmpItem.clear();
    tmpPwr.clear();
    codeShift = Qt::Key_Meta;
    taskShift = Qt::Key_Meta;
    testShift = Qt::Key_Meta;
    tempShift = Qt::Key_Meta;
    testparm.clear();
    return Qt::Key_Away;
}

int AppWindow::taskCheckPlay()
{
    int ret = Qt::Key_Meta;
    taskShift = (prevShift == Qt::Key_Play) ? Qt::Key_Play : taskShift;  // 预启动
    prevShift = Qt::Key_Meta;
    if (taskShift == Qt::Key_Play) {
        if (stack->currentWidget()->objectName() != "tester" && !isSampling) {
            taskClearData();
        } else {
            ret = Qt::Key_Away;
            int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
            tmpSet.insert(addr + TEMPPLAY, QTime::currentTime());
            int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
            int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
            if (mode == 3) {
                sendUdpStr("6077 0");  // 清理二代IO板状态
            }
            t.restart();
            if (taskparm.value("pumpstop").toInt() == 1)
                recvAppPrep();
            this->setFocus();
            recvNoise(1);
        }
    }
    return ret;
}

int AppWindow::taskCheckCode()
{
    int ret = Qt::Key_Meta;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int syst = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置
    int rfid = tmpSet.value(syst + SystRFID).toInt();
    int testtime = testparm.value("testtime").toInt() + 1;
    testparm.insert("testtime", testtime);
    if (rfid & 0x01) {  // 产线模式,控石产线专用
        QString str = tr("警告:条码未检到");
        if (mode == 3 && testtime % 100 == 1) {
            str = tr("警告:RFID未检到");
            sendUdpStr("6075");  // 读取RFID
        }
        if (codeShift == Qt::Key_Away) {
            testtime = 0;
            ret = Qt::Key_Away;
        } else {
            if (testtime > 300) { // 重复3次后未检到RFID,显示警告,下发停止
                warnningString(str);
                currTask = taskBuf.indexOf(&AppWindow::taskStartSave);
                sendUdpStr(tr("6022 %1").arg(station).toUtf8());
                taskShift = Qt::Key_Stop;
            }
        }
    } else {
        ret = Qt::Key_Away;
    }
    if (rfid & 0x02) {
        QString code = barcode.isEmpty() ? tmpcode : barcode;
        QString f = tmpSet.value(DataType).toString();
        if (!code.startsWith(f)) {
            QString str = tr("警告:条码不正确");
            warnningString(str);
            currTask = taskBuf.indexOf(&AppWindow::taskStartSave);
            sendUdpStr(tr("6022 %1").arg(station).toUtf8());
            taskShift = Qt::Key_Stop;
        }
        qDebug() << code << f;
    }
    return ret;
}

int AppWindow::taskStartView()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int spec = tmpSet.value(back + backTest).toString().toInt(NULL, 16);  // 特殊配置
    QString s = (mode == 1) ? "6020" : "6066";
    int work = (spec & 0x80) ? WORKL : station;
    sendUdpStr(QString("%1 %2").arg(s).arg(work).toUtf8());
    tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
    tmpMsg.insert(Qt::Key_2, DATAON);
    tmpMsg.insert(Qt::Key_1, "LEDY");
    tmpMsg.insert(Qt::Key_4, station);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    currItem = getNextItem();
    tmpSet[tmpSet.value((3000 + Qt::Key_0)).toInt() + TEMPISOK] = DATAOK;
    qDebug() << "app view:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskToolIobrd()
{  // 常规无刷测试夹紧->下压;无刷整机内置驱动切换接触器,外置驱动器无动作
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
    int driv = tmpSet.value(conf + ADDRDRIV).toInt();
    if (mode != 1) {  // 无刷模式,内置驱动
        int downaddr = ((station == WORKL) ? 0x00 : 0x01) + back + 0x40;  // 下压动作左/右地址
        int grabaddr = ((station == WORKL) ? 0x02 : 0x03) + back + 0x40;  // 夹紧动作左/右地址
        int saveaddr = ((driv == 0) ? 0x0A : 0x0B) + back + 0x40;  // 内驱/外驱保持
        int down = tmpSet.value(downaddr).toString().toInt(NULL, 16);  // 下压动作
        int grab = tmpSet.value(grabaddr).toString().toInt(NULL, 16);  // 夹紧动作
        int save = tmpSet.value(saveaddr).toString().toInt(NULL, 16);  // 内驱保持
        int test = down + grab + save;
        ioHexS = test;
        if (test != 0)
            sendUdpStr(tr("6036 %1").arg(test).toUtf8());
    }  // 外置驱动无动作
    return Qt::Key_Away;
}

int AppWindow::taskStartWait()
{
    int ret = Qt::Key_Meta;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int syst = tmpSet.value((2000 + Qt::Key_1)).toInt();  // 系统设置地址
    int wait = tmpSet.value(syst + SystWait).toDouble() * 100;
    int testview = testparm.value("testview").toInt() + 1;
    testparm.insert("testview", testview);
    if (testview > wait) {
        ret = Qt::Key_Away;
        if (mode == 3) {
            sendUdpStr(tr("6077 %1").arg(YY08).toUtf8());  // 二代IO板状态改为测试中
        }
    }
    return ret;
}

int AppWindow::taskStartTest()
{
    int ret = testThread();
    return ret;
}

int AppWindow::taskClearCtrl()
{  // 测试完成,清理输出状态
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int spec = tmpSet.value(back + backTest).toString().toInt(NULL, 16);  // 特殊配置
    int time = tmpSet.value(back + backWait).toInt();
    int itemctrl = itemparm.value("itemctrl").toInt() + 1;
    itemparm.insert("itemctrl", itemctrl);
    if (mode == 3) {  // G201712034深圳控石等待轴移除
        if (itemctrl % 100 == 1) {
            sendUdpStr(tr("6077 %1").arg(YY14 | YY08).toUtf8());
        }
        if (itemctrl > 500) {
            warnningString(tr("警告:轴移除超时!"));
            return Qt::Key_Away;
        }
        return (ioHexL & XX23) ? Qt::Key_Away : Qt::Key_Meta;
    }
    if (mode != 1 && (spec & 0x08)) {  // G201901037无锡寰宇刹车功能,下压+夹紧+测试+刹车
        int downaddr = ((station == WORKL) ? 0x00 : 0x01) + back + 0x40;  // 气动弹线左/右地址
        int grabaddr = ((station == WORKL) ? 0x02 : 0x03) + back + 0x40;  // 夹紧动作左/右地址
        int voltaddr = ((station == WORKL) ? 0x06 : 0x07) + back + 0x40;  // 低压动作左/右地址
        int saveaddr = 0x0A + back + 0x40;
        int down = tmpSet.value(downaddr).toString().toInt(NULL, 16);  // 下压动作
        int grab = tmpSet.value(grabaddr).toString().toInt(NULL, 16);  // 夹紧动作
        int save = tmpSet.value(saveaddr).toString().toInt(NULL, 16);  // 测试保持
        int test = tmpSet.value(voltaddr).toString().toInt(NULL, 16);  // 低压动作
        int stop = (station == WORKL) ? 0x008000 : 0x010000;  // 左Y13,右Y15
        if (itemctrl % 100 == 1)
            sendUdpStr(tr("6036 %1").arg(down + grab + save + stop + test).toUtf8());
        if (itemctrl < time*100)
            return Qt::Key_Meta;
    }
    return Qt::Key_Away;
}

int AppWindow::taskStopServo()
{
    QVariantMap tmp;
    tmp.insert("taskwork", "stop");
    emit sendAppMap(tmp);
    tmp.clear();
    return Qt::Key_Away;
}

int AppWindow::taskClearWait()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int test = tmpSet.value(back + backTest).toInt();
    if (test & 0x02)
        return Qt::Key_Away;
    int teststat = testparm.value("teststat").toInt();
    int testwait = testparm.value("testwait").toInt() + 1;
    testparm.insert("testwait", testwait);
    if (testwait > 500)
        warnningString(tr("异常:等待停止超时"));
    return (teststat == 2 || testwait > 500) ? Qt::Key_Away : Qt::Key_Meta;
}

int AppWindow::taskCheckSave()
{
    int conf = tmpSet.value(2000 + Qt::Key_3).toInt();
    int mode = tmpSet.value(conf + 0x00).toInt();
    if (mode == 4 && taskShift != Qt::Key_Stop) {
        if (boxsave == NULL) {
            boxsave = new QMessageBox(this);
            boxsave->addButton(QMessageBox::Yes);
            boxsave->addButton(QMessageBox::No);
            boxsave->setText(tr("测试不合格,是否上传?"));
        }
        if (boxsave->isHidden())
            testparm.remove("tasksave");
        if (isok == DATANG) {
            int ret = boxsave->exec();
            testparm.insert("tasksave", (ret == QMessageBox::Yes) ? 0 : 1);
        }
    }
    if (mode == 5 && taskShift != Qt::Key_Stop) {
        if (boxsave == NULL) {
            boxsave = new QMessageBox(this);
            boxsave->addButton(QMessageBox::Yes);
            boxsave->addButton(QMessageBox::No);
            boxsave->setText(tr("测试不合格,是否保存?"));
        }
        if (boxsave->isHidden())
            testparm.remove("tasksave");
        if (isok == DATANG) {
            int ret = boxsave->exec();
            testparm.insert("tasksave", (ret == QMessageBox::Yes) ? 0 : 1);
            if (ret == QMessageBox::No)
                taskShift = Qt::Key_Stop;
        }
    }
    if (isSampling) {
        currTask = taskBuf.indexOf(&AppWindow::taskWaitReset);
    }
    return Qt::Key_Away;
}

int AppWindow::taskStartSave()
{
    if (1) {
        int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
        int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
        if (mode == 3) {  // 产线模式,控石产线专用
            bool isSetnvt = (testparm.value("testloop").toInt() == 0) ? false : true;
            quint32 cmd = 0;
            cmd += (isSetnvt) ? YY15 : 0x00;
            cmd |= (isok == DATAOK && !barcode.isEmpty() && !isSetnvt) ? YY0E : YY10;
            cmd |= barcode.isEmpty() ? YY09 : 0x00;  // RFID未读到
            cmd |= (tmpItem.value(nSetNLD) == DATAOK) ? 0x00 : YY11;
            cmd |= (tmpItem.value(nSetHAL) == DATAOK) ? 0x00 : YY12;
            cmd |= (tmpItem.value(nSetLOD) == DATAOK) ? 0x00 : YY13;
            sendUdpStr(tr("6077 %1").arg(cmd).toUtf8());
        }
        if (mode != 1)  // 无刷模式
            sendUdpStr(tr("6080 %1").arg((isok == DATAOK) ? "1" : "0").toUtf8());  // 下发总测试结果
    }
    if (taskShift == Qt::Key_Stop || testparm.value("tasksave").toInt() != 0)
        return Qt::Key_Away;
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    int user = tmpSet.value(DataUser).toInt();
    QString temp = tmpSet.value(addr + TEMPTEMP).toString();
    temp = temp.remove(tr("温度:"));
    temp = temp.remove("°C");
    tmpMsg = tmpSave;
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);  // 存储数据
    tmpMsg.insert(Qt::Key_1, "aip_record");
    tmpMsg.insert(addr + TEMPDATE, QDate::currentDate().toString("yyyy-MM-dd"));  // 日期
    tmpMsg.insert(addr + TEMPPLAY, tmpSet.value(addr + TEMPPLAY));  // 测试开始时间
    tmpMsg.insert(addr + TEMPSTOP, QTime::currentTime());  // 测试结束时间
    tmpMsg.insert(addr + TEMPTYPE, tmpSet.value(DataType).toString());  // 型号
    tmpMsg.insert(addr + TEMPCODE, barcode.isEmpty() ? tmpcode : barcode);  // 条码
    tmpMsg.insert(addr + TEMPUSER, tmpSet.value(user).toString());  // 用户
    tmpMsg.insert(addr + TEMPWORK, (station == 0x13) ? "L" : "R");  // 工位
    tmpMsg.insert(addr + TEMPTEMP, temp);  // 温度
    tmpMsg.insert(addr + TEMPISOK, (isok == DATAOK) ? "OK" : "NG");  // 结果
    if (testparm.value("tasksave").toInt() != 1)
        emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    qDebug() <<"app save:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskClearWarn()
{
    int temp = tmpSet.value(3000 + Qt::Key_0).toInt();  // 临时数据地址
    int sign = tmpSet.value(temp + TEMPSIGN).toInt();  // 是否已登录
    int real = tmpSet.value(DataUser).toInt();  // 当前用户地址
    int gear = (sign == 1) ? tmpSet.value(real + mRole).toInt() : 4;  // 当前权限等级
    if (gear < 3)
        return Qt::Key_Away;
    if (testparm.value("testwarn").toInt() == 1) {
        sendUdpStr("6094 1");  // 启用防呆
        bool ok = false;
        QString user = QInputDialog::getText(this, tr(""), tr("请输入用户"),
                                             QLineEdit::Normal, "admin", &ok);
        if (!ok || user.isEmpty()) {
            return Qt::Key_Meta;
        }
        QString text = QInputDialog::getText(this, tr(""), tr("请输入密码"),
                                             QLineEdit::Password, "", &ok);
        if (!ok) {
            return Qt::Key_Meta;
        }
        for (int i=0; i < 20; i++) {
            int addr = tmpSet.value(2000 + Qt::Key_5).toInt() + (i + 1) * 5;  // 隐藏超级用户
            QString name = tmpSet.value(addr + mName).toString();
            QString pass = tmpSet.value(addr + mPass).toString();
            QString role = tmpSet.value(addr + mRole).toString();
            if (user == name && role.toInt() < 3 && text == pass) {
                sendUdpStr("6094 0");  // 停用防呆
                return Qt::Key_Away;
            }
        }
        return Qt::Key_Meta;
    }
    return Qt::Key_Away;
}

int AppWindow::taskStartBeep()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
    int driv = tmpSet.value(conf + ADDRDRIV).toInt();
    if (mode != 1) {  // 无刷模式,内置驱动,松开夹紧气缸
        int downaddr = ((station == WORKL) ? 0x00 : 0x01) + back + 0x40;  // 下压动作左/右地址
        int grabaddr = ((station == WORKL) ? 0x02 : 0x03) + back + 0x40;  // 夹紧动作左/右地址
        int beepaddr = ((station == WORKL) ? 0x08 : 0x09) + back + 0x40;  // 气动弹线左/右地址
        int testaddr = ((isok == DATAOK) ? 0x0D : 0x0E) + back + 0x40;
        int saveaddr = ((driv == 0) ? 0x0A : 0x0B) + back + 0x40;  // 内驱/外驱保持
        int down = tmpSet.value(downaddr).toString().toInt(NULL, 16);  // 下压动作
        int grab = tmpSet.value(grabaddr).toString().toInt(NULL, 16);  // 夹紧动作
        int save = tmpSet.value(saveaddr).toString().toInt(NULL, 16);  // 内驱保持
        int beep = tmpSet.value(beepaddr).toString().toInt(NULL, 16);  // 内驱保持
        int test = tmpSet.value(testaddr).toString().toInt(NULL, 16);  // 合格/不良保持
        down = (grab == 0) ? 0 : down;  // 没有夹紧动作则下压不复位
        sendUdpStr(tr("6036 %1").arg(down + save + beep + test).toUtf8());
    }  // 外置驱动无动作
    int syst = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置地址
    int beep = tmpSet.value(syst + SystBeep).toInt() * 10 + 9;  // 报警时间
    tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
    tmpMsg.insert(Qt::Key_1, isok == DATAOK ? "LEDG" : "LEDR");
    tmpMsg.insert(Qt::Key_2, (taskShift == Qt::Key_Stop) ? DATADC : isok);
    tmpMsg.insert(Qt::Key_3, beep);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    qDebug() <<"app beep:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskClearBeep()
{
    int ret = Qt::Key_Meta;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
    int driv = tmpSet.value(conf + ADDRDRIV).toInt();
    int syst = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置地址
    int tt = tmpSet.value(syst + (isok == DATAOK ? SystTime : SystWarn)).toDouble()*100;
    int testbeep = testparm.value("testbeep").toInt() + 1;
    testparm.insert("testbeep", testbeep);
    if (testbeep >= tt) {
        if (mode != 1) {  // 无刷模式,内置驱动
            int saveaddr = ((driv == 0) ? 0x0A : 0x0B) + back + 0x40;  // 内驱/外驱保持
            int freeaddr = 0x0C + back + 0x40;
            int testaddr = ((isok == DATAOK) ? 0x0D : 0x0E) + back + 0x40;
            int test = tmpSet.value(testaddr).toString().toInt(NULL, 16);  // 合格/不良保持
            int free = tmpSet.value(freeaddr).toString().toInt(NULL, 16);  // 空闲时保持
            int save = tmpSet.value(saveaddr).toString().toInt(NULL, 16);  // 内驱保持
            free = (driv == 0) ? free : 0;
            sendUdpStr(tr("6036 %1").arg(save + free + test).toUtf8());
        }  // 外置驱动无动作
        ret = Qt::Key_Away;
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_3, 0);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        qDebug() <<"app wait:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0')) << tt;
    }
    return ret;
}

int AppWindow::taskWaitReset()
{
    int syst = tmpSet.value(2000 + Qt::Key_1).toInt();
    int warn = tmpSet.value(syst + SystItem).toInt();
    if (isok == DATANG && warn == 2) {
        if (testparm.value("testwarn").isNull()) {
            sendUdpStr("6094 1");  // 启用防呆
            testparm.insert("testwarn", 2);
            boxbar->setLabelText(tr("等待不合格品放置"));
            boxbar->setValue(2);
            boxbar->show();
        }
        int testwarn = testparm.value("testwarn").toInt() + 1;
        testparm.insert("testwarn", testwarn);
        boxbar->setValue((testwarn/100)%99);
        if ((prvHex & 0x01) && ((ioHexL & 0x01) == 0)) {  // X00接光电开关
            prvHex = ioHexL;
            sendUdpStr("6094 0");  // 停用防呆
            boxbar->hide();
            return Qt::Key_Away;
        }
        prvHex = ioHexL;
        return Qt::Key_Meta;
    }
    return Qt::Key_Away;
}

int AppWindow::taskResetTest()
{
    if (taskShift == Qt::Key_Stop) {
        isSampling = false;
        return Qt::Key_Away;
    }
    int addr = tmpSet.value(1000 + Qt::Key_0).toInt();
    int test = tmpSet.value(addr + backAuto).toInt();
    int time = tmpSet.value(addr + backWait).toInt();
    int testredo = testparm.value("testredo").toInt() + 1;
    testparm.insert("testredo", testredo);
    if ((test >= 1 || isSampling) && testredo >= time*100) {
        if (isSampling) {
            count++;
            isSampling = (count >= 15) ? false : isSampling;
            tmpMsg.insert(Qt::Key_0, Qt::Key_Away);
            tmpMsg.insert(Qt::Key_1, count);
            tmpMsg.insert(Qt::Key_2, taskparm.value("taskvolt").toDouble());
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
            if (count >= 15) {
                count = 0;
                return Qt::Key_Away;
            }
        }
        taskClearData();
        currTask = taskBuf.indexOf(&AppWindow::taskCheckPlay);
        taskShift = Qt::Key_Play;
        station = (test == 1) ? WORKL : station;
        station = (test == 2) ? WORKR : station;
        station = (test == 3) ? ((station == WORKL) ? WORKR : WORKL) : station;
        t.restart();
    }
    return (test >= 1 || isSampling) ? Qt::Key_Meta : Qt::Key_Away;
}

int AppWindow::taskCheckStop()
{
    int addr = tmpSet.value(3000 + Qt::Key_0).toInt();  // 综合测试结果
    int taskplay = taskBuf.indexOf(&AppWindow::taskCheckPlay);
    int taskover = taskBuf.indexOf(&AppWindow::taskStartTest) + 1;
    int tasksave = taskBuf.indexOf(&AppWindow::taskStartSave);
    int taskwait = taskBuf.indexOf(&AppWindow::taskStartWait);
    int taskitem = getNextItem();
    bool teststop = currTask < taskover && currTask > taskplay;
    teststop = (testShift == Qt::Key_Away && taskitem == 0) ? false : teststop;
    if (currTask <= taskplay || teststop) {
        sendUdpStr(tr("6022 %1").arg(station).toUtf8());
        tmpMsg.insert(Qt::Key_0, Qt::Key_Stop);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
    if (teststop) {
        testparm.insert("teststop", 1);
        taskShift = Qt::Key_Stop;
        isok = DATANG;
        currTask = (currTask <= taskwait) ? tasksave : taskover;
        tmpSet.insert(addr + TEMPISOK, DATANG);
        recvNoise(2);
    }
    qDebug() << "app stop:" << teststop << currTask << taskplay << taskover;
    return Qt::Key_Away;
}

int AppWindow::testThread()
{
    int ret = Qt::Key_Meta;
    int tasknumb = testparm.value("tasknumb").toInt();
    if (tasknumb < testBuf.size()) {
        if ((this->*testBuf[tasknumb])() == Qt::Key_Away) {
            tasknumb++;
            tasknumb = (tasknumb >= testBuf.size()) ? 0 : tasknumb;  // 测试流程
            if (tasknumb == 0) {
                currItem = getNextItem();
                if (currItem == 0)
                    ret = Qt::Key_Away;
            }
            testparm.insert("tasknumb", tasknumb);
        }
    }
    return ret;
}

int AppWindow::testClearData()
{
    itemparm.clear();
    testShift = Qt::Key_Meta;
    return Qt::Key_Away;
}

int AppWindow::testToolIocan()
{
    int itemwait = itemparm.value("itemtool").toInt() + 1;
    itemparm.insert("itemtool", itemwait);
    if (currItem == nSetNLD || currItem == nSetLPH) {
        int addr = tmpSet.value(4000 + Qt::Key_D).toInt();  // 空载配置地址
        int time = tmpSet.value(addr + CACHELOD*0x02).toDouble() * 100;
        if (itemwait < time)
            return Qt::Key_Meta;
    }
    if (itemwait < 10)
        return Qt::Key_Meta;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int spec = tmpSet.value(back + backTest).toString().toInt(NULL, 16);  // 特殊配置
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合设置地址
    int driv = tmpSet.value(conf + ADDRDRIV).toInt();
    if (driv != 0 && (spec&0x80))  // 卧龙外驱不动作
        return Qt::Key_Away;
    bool isDriv = (currItem == nSetINR || currItem == nSetACW);  // 高压项目
    if (mode != 1) {  // 无刷模式,内置驱动
        int downaddr = ((station == WORKL) ? 0x00 : 0x01) + back + 0x40;  // 气动弹线左/右地址
        int grabaddr = ((station == WORKL) ? 0x02 : 0x03) + back + 0x40;  // 夹紧动作左/右地址
        int highaddr = ((station == WORKL) ? 0x04 : 0x05) + back + 0x40;  // 高压动作左/右地址
        int voltaddr = ((station == WORKL) ? 0x06 : 0x07) + back + 0x40;  // 低压动作左/右地址
        int testaddr = (isDriv) ? highaddr : voltaddr;
        int saveaddr = ((driv == 0) ? 0x0A : 0x0B) + back + 0x40;  // 内驱/外驱保持
        int down = tmpSet.value(downaddr).toString().toInt(NULL, 16);  // 下压动作
        int grab = tmpSet.value(grabaddr).toString().toInt(NULL, 16);  // 夹紧动作
        int test = tmpSet.value(testaddr).toString().toInt(NULL, 16);
        int save = tmpSet.value(saveaddr).toString().toInt(NULL, 16);
        if (test != 0) {
            ioHexS = test + down + grab + save;
            sendUdpStr(tr("6036 %1").arg(ioHexS).toUtf8());
        }
    }
    if (currItem == nSetMAG) {
        int addr = tmpSet.value(4000 + Qt::Key_2).toInt();  // 反嵌配置地址
        int swit = tmpSet.value(addr + 5).toInt();  // Y型切换
        int test = (station == WORKL) ? 0x01 : 0x02;
        if (swit != 0) {
            sendUdpStr(tr("6036 %1").arg(test).toUtf8());
            wait(500);
        }
    }
    if (currItem == nSetIMP) {
        int addr = tmpSet.value(4000 + Qt::Key_6).toInt();  // 反嵌配置地址
        int swit = tmpSet.value(addr + 4).toInt();  // Y型切换
        int test = (station == WORKL) ? 0x01 : 0x02;
        if (swit != 0) {
            sendUdpStr(tr("6036 %1").arg(test).toUtf8());
            wait(500);
        }
    }
    return Qt::Key_Away;
}

int AppWindow::testStopIocan()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int spec = tmpSet.value(back + backTest).toString().toInt(NULL, 16);  // 特殊配置
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合设置地址
    int driv = tmpSet.value(conf + ADDRDRIV).toInt();
    if (driv != 0 && (spec&0x80))  // 卧龙外驱不动作
        return Qt::Key_Away;
    bool isDriv = (currItem == nSetINR || currItem == nSetACW);  // 当前项目为高压项目
    int nextitem = getNextItem();
    bool isNext = (nextitem == nSetINR || nextitem == nSetACW);  // 下一项目为高压项目
    bool isMove = (isDriv && !isNext) || (!isDriv && isNext);
    if (mode != 1 && isMove) {  // 高压变低压动作
        int downaddr = ((station == WORKL) ? 0x00 : 0x01) + back + 0x40;  // 气动弹线左/右地址
        int grabaddr = ((station == WORKL) ? 0x02 : 0x03) + back + 0x40;  // 夹紧动作左/右地址
        int highaddr = ((station == WORKL) ? 0x04 : 0x05) + back + 0x40;  // 高压动作左/右地址
        int voltaddr = ((station == WORKL) ? 0x06 : 0x07) + back + 0x40;  // 低压动作左/右地址
        int testaddr = (isDriv) ? highaddr : voltaddr;
        int saveaddr = ((driv == 0) ? 0x0A : 0x0B) + back + 0x40;  // 内驱/外驱保持
        int down = tmpSet.value(downaddr).toString().toInt(NULL, 16);  // 下压动作
        int grab = tmpSet.value(grabaddr).toString().toInt(NULL, 16);  // 夹紧动作
        int test = tmpSet.value(testaddr).toString().toInt(NULL, 16);
        int save = tmpSet.value(saveaddr).toString().toInt(NULL, 16);
        if (test != 0) {
            ioHexS = down + grab + save;
            sendUdpStr(tr("6036 %1").arg(ioHexS).toUtf8());
        }
    }
    if (mode != 1) {
        isDriv = (currItem == nSetMAG) || (currItem == nSetIMP);  // 当前项目为Y型切换
        isNext = (nextitem == nSetMAG) || (nextitem == nSetIMP);  // 下一项目为Y型切换
        if (isDriv && !isNext) {
            sendUdpStr(tr("6036 %1").arg(0x00).toUtf8());
        }
    }

    return Qt::Key_Away;
}

int AppWindow::testToolServo()
{  // 伺服驱动器启动
    int taskback = tmpSet.value(1000 + Qt::Key_0).toInt();
    int taskspec = tmpSet.value(taskback + backTest).toString().toInt(NULL, 16);  // 特殊配置
    int taskwork = station;  // 测试工位
    int taskaddr = (taskwork == WORKL) ? 0x06 : 0x07;  // 伺服驱动串口地址
    QString taskname = tmpSet.value(taskback + 0x20 + taskaddr).toString();
    if (!taskname.isEmpty()) {
        if ((currItem == nSetPWR) && (taskspec & 0x40)) {  // 无锡寰宇驱动器启动
            int taskconf = tmpSet.value(4000 + Qt::Key_7).toInt();
            int taskdata = tmpSet.value(taskconf + 0x06).toInt();
            int tasktime = tmpSet.value(taskconf + 0x07).toDouble() * 10;
            QVariantMap map;
            map.insert("taskname", taskname);
            map.insert("taskwork", "wash");
            map.insert("taskdata", taskdata);
            map.insert("tasktime", tasktime);
            emit sendAppMap(map);
            map.clear();
            qDebug() << "app wash:" << taskname << taskdata;
        }
        if (currItem == nSetLOD) {  // 测试负载,先设置扭矩为0
            QVariantMap map;
            map.insert("taskname", taskname);
            map.insert("taskwork", "load");
            map.insert("taskmode", 1);
            emit sendAppMap(map);
            map.clear();
        }
        if (currItem == nSetEMF) {  // 测试反电动势,三菱伺服使用关键字turn,松下PLC使用load
            int taskconf = tmpSet.value(4000 + Qt::Key_E).toInt();  // 反电动势配置地址
            int taskdata = tmpSet.value(taskconf + 0x10).toDouble();
            int taskturn = tmpSet.value(taskconf + 0x11).toDouble();
            QVariantMap map;
            map.insert("taskname", taskname);
            map.insert("taskwork", (taskspec & 0x04) ? "turn" : "load");
            map.insert("taskdata", taskdata*((taskspec & 0x04) ? 1 : 2.001));
            map.insert("taskturn", taskturn);
            emit sendAppMap(map);
            map.clear();
        }
    }
    return Qt::Key_Away;
}

int AppWindow::testWaitServo()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int itemwait = itemparm.value("itemwait").toInt() + 1;
    itemparm.insert("itemwait", itemwait);
    if (currItem == 0x0C) { // 负载
        int addr = tmpSet.value(4000 + Qt::Key_C).toInt();  // 负载配置地址
        int rate = tmpSet.value(addr + 0x0F).toInt();  // 批量模式
        int rstd = tmpSet.value(addr + 0x0E).toInt();  // 设定转速
        int time = 0;
        for (int i=0; i < 4; i++) {
            time += tmpSet.value(addr + CACHELOD*0x03).toDouble() * 100;
        }
        if (itemwait < time)
            return Qt::Key_Meta;
        double rmax = tmpSet.value(addr + 0x06).toDouble();
        double rmin = tmpSet.value(addr + 0x07).toDouble();
        double temp = tmpSet.value(addr + 0x13).toDouble();  // 设置扭矩
        double comp = tmpSet.value(addr + 0x23).toDouble();  // 补偿扭矩
        rstd = (rstd == 0) ? (rmax + rmin) / 2 : rstd;  // 正常测试使用标准转速
        int conf = tmpSet.value(2000 + Qt::Key_7).toInt();
        int speed = tmpSet.value(conf + 0x01).toDouble();
        if (isSampling)  // 预热采样使用采样转速
            rstd = speed;
        double torque = tmpSet.value(conf + 0x02).toDouble();
        QString str = tmpSet.value(addr + 0x23).toString();
        QStringList cs = str.split(",");
        if (cs.size() >= 2) {
            comp = cs.at((station == WORKL) ? 0 : 1).toDouble();
        }
        temp = qMax(temp - comp, 0.0);  // 实际下发扭矩
        double tor = temp;
        tor = (tor <= 0.035) ? 0 : tor;
        tor = (tor < 0.065 && tor > 0.035) ? tor*0.77 : tor;
        tor = (tor >= 0.05) ? tor/0.94 : tor;

        if (!taskparm.value("taskrate").isNull() && rate != 0) {
            int conf = tmpSet.value(2000 + Qt::Key_7).toInt();
            double max = tmpSet.value(conf + 0x03).toInt();
            double ekp = tmpSet.value(conf + 0x04).toInt();
            double rpm = taskparm.value("taskrate").toInt();
            double tmp = taskparm.value("taskvolt").toDouble();
            if (tstrpm.size() > 10 && abs(rpm - rstd) > max) {
                rpm = tstrpm.last();  // 转速超出限制,使用上一次的值
                rpm = abs(rpm - rstd) > max ? rstd : rpm;  // 上一次也超出限制,使用固定值
            }
            tstrpm.append(rpm);
            if (tstrpm.size() >= 30) {
                for (int t=0; t < 15; t++)
                    tstrpm.removeFirst();
                QList<double> rpms = tstrpm;
                qSort(rpms.begin(), rpms.end());
                rpm = rpms.at(7);
            }
            if (tstrpm.size() >= 2 && tstrpm.size() <= 10) {
                ekp += ((rpm - rstd > 5) ? 0.1 : 0);
                ekp -= ((rstd - rpm > 5) ? 0.1 : 0);
                ekp = qMin(ekp, 0.7);
                ekp = qMax(ekp, 0.2);
            }
            if (tstrpm.size() <= 15) {
                double ek0 = (rpm - rstd);  // 当前误差
                double cmp = 0.001*(ekp*ek0);
                tmp += cmp;
                taskparm.insert("taskvolt", tmp);
            }
        }
        // 非采样批量模式,使用上次的采样值
        if (!isSampling && taskparm.value("taskvolt").isNull() && rate != 0) {  //
            taskparm.insert("taskvolt", torque);
        }
        double volt = taskparm.value("taskvolt").toDouble();
        tor = (rate == 0) ? tor : volt;
        tor = qMax(0.0, tor);
        int taskdata = ((mode == 3) ? tor*1368 : tor*2500);  // 控石伺服与常规不同
        int taskaddr = (station == WORKL) ? 0x06 : 0x07;
        QString com = tmpSet.value(back + 0x20 + taskaddr).toString();
        if (!com.isEmpty()) {
            QVariantMap map;
            map.insert("taskname", com);
            map.insert("taskwork", "load");
            map.insert("taskmode", 1);
            map.insert("taskdata", taskdata);
            emit sendAppMap(map);
            map.clear();
            qDebug() << "load" << com << temp << tor;
        }
    }
    return Qt::Key_Away;
}

void AppWindow::clearBatch()
{
    taskparm.remove("taskvolt");
    taskparm.remove("taskrate");
    tstrpm.clear();
    count = 0;
}

int AppWindow::testStopServo()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int spec = tmpSet.value(back + backTest).toString().toInt(NULL, 16);  // 特殊配置
    QVariantMap map;
    if ((spec & 0x40) && (currItem == nSetEMF)) {  // 无锡寰宇三菱伺服反电动势停止
        int addr = (station == WORKL) ? 0x04 : 0x05;
        if (!tmpSet.value(back + 0x20 + addr).toString().isEmpty()) {
            QString com = tmpSet.value(back + 0x20 + addr).toString();
            map.insert("taskname", com);
            map.insert("taskwork", "stop");
            map.insert("taskdata", 0);
            map.insert("taskturn", 0);
            emit sendAppMap(map);
            map.clear();
            qDebug() << "stop" << com;
        }
        return Qt::Key_Away;
    }
    QVariantMap tmp;
    tmp.insert("taskwork", "stop");
    emit sendAppMap(tmp);
    tmp.clear();
    return Qt::Key_Away;
}

int AppWindow::testToolInvrt()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 型号设置地址
    int driv = tmpSet.value(conf + ADDRDRIV).toInt();  // 内外置驱动设置
    if (driv && (currItem == nSetLOD || currItem == nSetNLD || currItem == nSetLPH)) {  // 外驱
        int addr = tmpSet.value(4000 + Qt::Key_C + currItem - nSetLOD).toInt();  // 负载配置地址
        int freq = tmpSet.value(addr + 0x10 + 0x05).toInt();  // 外驱频率
        int turn = tmpSet.value(addr + 0x20 + 0x05).toInt() - 1; // 外驱转向
        sendUdpStr(QString("6083 %1 %2").arg(WORKL).arg(1).toUtf8());
        QString com = tmpSet.value(back + 0x20 + 0x09).toString();
        if (!com.isEmpty()) {
            QVariantMap map;
            map.insert("taskname", com);
            map.insert("taskwork", "loop");
            map.insert("taskdata", freq);
            map.insert("taskturn", turn);
            emit sendAppMap(map);
            map.clear();
            qDebug() << com << freq << turn;
        }
        int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合设置地址
        int driv = tmpSet.value(conf + ADDRDRIV).toInt();
        int vvcc = (driv == 1) ? tmpSet.value(addr + 0x11).toInt()*1000 : 0;
        int powr = tmpSet.value(addr + 0x24).toInt();
        sendUdpStr(tr("6074 %1 %2 1 %3 %4").arg(1).arg(1).arg(powr).arg(vvcc).toUtf8());
    }
    return Qt::Key_Away;
}

int AppWindow::testStartSend()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Play);
    tmpMsg.insert(Qt::Key_1, currItem);
    tmpMsg.insert(Qt::Key_4, station);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    QString item = taskparm.value(QString::number(currItem)).toString();
    sendUdpStr(QString("6006 %1").arg(item).toUtf8());

    return Qt::Key_Away;
}

int AppWindow::testStartTest()
{
    int testvolt = itemparm.value("testvolt").toInt();
    if (currItem == nSetNLD && testvolt == 200) {
        int addr = tmpSet.value(4000 + Qt::Key_D).toInt();  // 空载配置地址
        double max = tmpSet.value(addr + 6*2 + 0).toDouble();
        double min = tmpSet.value(addr + 6*2 + 1).toDouble();
        if (max != 0) {
            sendUdpStr(QString("6101 6 7 1 %1 %2 0.5").arg(max).arg(min).toUtf8());
        }
    }
    itemparm.insert("testvolt", testvolt + 1);
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int spec = tmpSet.value(back + backTest).toString().toInt(NULL, 16);  // 特殊配置
    if (currItem == nSetPWR) {
        int addr = (station == WORKL) ? 0x06 : 0x07;
        int conf = tmpSet.value(4000 + Qt::Key_7).toInt();
        int real = tmpSet.value(conf + 0x06).toInt();
        int time = tmpSet.value(conf + CACHEPWR*TIMEPWR1 + CACHEPWR).toDouble() * 100;
        if (spec & 0x40) {  // 无锡寰宇驱动器,每2s启动一次
            if (!tmpSet.value(back + 0x20 + addr).toString().isEmpty() && testvolt % 200 == 0) {
                QString com = tmpSet.value(back + 0x20 + addr).toString();
                QVariantMap map;
                map.insert("taskname", com);
                map.insert("taskwork", "wash");
                map.insert("taskdata", real);
                map.insert("tasktime", 0);
                if (time - testvolt > 400)
                    emit sendAppMap(map);
                map.clear();
                qDebug() << "pwr" << com << real << testvolt << time;
            }
        } else {
            int syst = tmpSet.value(2000 + Qt::Key_1).toInt();
            int pwwr = tmpSet.value(syst + 0x0E).toInt();
            if (testvolt == 0) {
                sendUdpStr(QString("6101 %1").arg(pwwr).toUtf8());
            }
        }
    }
    if (currItem == nSetLOD) {
        int addr = (station == WORKL) ? 0x04 : 0x05;
        QString com = tmpSet.value(back + 0x20 + addr).toString();
        if (!com.isEmpty()) {
            QVariantMap map;
            map.insert("taskname", com);
            map.insert("taskwork", "read");
            if (testvolt % 50 == 5)
                emit sendAppMap(map);
            map.clear();
        }
    }
    return (testShift == Qt::Key_Away) ? Qt::Key_Away : Qt::Key_Meta;
}

int AppWindow::testCheckWarn()
{
    if (currItem != 0) {
        int syst = tmpSet.value(2000 + Qt::Key_1).toInt();
        int warn = tmpSet.value(syst + SystItem).toInt();
        if (warn != 0) {
            int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
            QStringList tmpPupop = tmpSet.value(conf + 0x05).toString().split(",");
            if (tmpPupop.contains(QString::number(currItem)) && isok == DATANG) {
                taskCheckStop();
                testparm.insert("testwarn", 1);
            }
        }
    }
    return Qt::Key_Away;
}

int AppWindow::getNextItem()
{
    timeRsl = 0;
    timeMag = 0;
    timeImp = 0;
    int r = tmpSet.value(4000 + Qt::Key_0).toInt();
    QStringList testItems = tmpSet.value(r + ADDRITEM).toString().split(",");

    int index = testItems.indexOf(QString::number(currItem));
    if (index < 0)
        return testItems.at(0).toInt();
    if (index + 1 == testItems.size())
        return 0;
    else
        return testItems.at(index+1).toInt();
}

int AppWindow::taskConfig()
{
    QStringList names;
    names << "setdcr" << "setmag" << "setinr" << "setacw" << "setdcw"
          << "setimp" << "setpwr" << "setind" << "setlvs" << "setlck"
          << "sethal" << "setlod" << "setnld" << "setbmf" << "setlph";
    QProgressDialog dialog(tr("配置下发进度"), tr("隐藏"), 0, names.size()*3, this);
    dialog.setWindowTitle(tr("进度对话框"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.show();

    int back = tmpSet.value(1000 + Qt::Key_0).toInt() + 0x10;  // 可用项目
    for (int i=0; i < names.size(); i++) {   // 可用项目
        int stat = tmpSet.value(back + i).toInt();
        if (stat != 0) {
            tmpMsg.insert(Qt::Key_1, names.at(i));
            tmpMsg.insert(Qt::Key_0, Qt::Key_Down);
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
            dialog.setValue((i+1)*3);
        }
    }
    tmpMsg.insert(Qt::Key_1, "config");
    tmpMsg.insert(Qt::Key_0, Qt::Key_Down);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    wait(500);
    QString name = stack->currentWidget()->objectName();
    if ((name == "author") || (name == "tester"))  // 自动跳转
        QTimer::singleShot(1000, this, SLOT(showTester()));
    return Qt::Key_Away;
}

int AppWindow::recvIoCtrl(int key, int work)
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int spec = tmpSet.value(back + backTest).toString().toInt(NULL, 16);  // 特殊配置
    int addr = tmpSet.value(2000 + Qt::Key_1).toInt();
    int have = tmpSet.value(addr + SystHave).toInt();  // 产品检测
    work = (ioHexL & X03) ? WORKL : work;  // 左启动被按下
    work = (ioHexR & X03) ? WORKR : work;  // 右启动被按下
    quint32 hex = (ioHexR != 0 && work == WORKR) ? ioHexR : ioHexL;
    QString pos = (ioHexR != 0 && work == WORKR) ? tr("右") : tr("左");
    QString str;
    if (mode == 1) {  // 真空模式 启动 && 光幕 && 非到位
        bool sl = (key == Qt::Key_Play && work == WORKL) || ((hex & X03) && ((hex & X11) == 0));
        bool sr = (key == Qt::Key_Play && work == WORKR) || ((hex & X00) && ((hex & X08) == 0));
        bool ll = (spec & 0x20) ? ((hex & X05) == 0) : (hex & X05);  // 左光幕
        bool lr = (spec & 0x20) ? ((hex & X02) == 0) : (hex & X02);  // 右光幕
        if (ioHexR != 0 && work == WORKR) {
            sl = (key == Qt::Key_Play && work == WORKR) || ((hex & X03) && ((hex & X11) == 0));
        }
        if (sl && ll && ((hex & X11) == 0)) {
            str = tr("警告:%1光幕遮挡!").arg(pos);
        }
        if (sr && lr && ((hex & X08) == 0)) {
            str = tr("警告:右光幕遮挡!");
        }
        if ((have == 1) && sl && ((hex & X14) == 0)) {
            str = tr("警告:产品未放置!");
        }
        if (!str.isEmpty()) {
            warnningString(str);
            return Qt::Key_Meta;
        }
        if (boxsave != NULL && (ioHexL & 0x10) && !(spec & 0x01)) {
            boxsave->button(QMessageBox::Yes)->click();
        }
    }
    if (mode != 1) {
        int prodaddr = back + 0x40 + ((work == WORKL) ? 0x18 : 0x19);  // 产品到位信号地址
        int prod = tmpSet.value(prodaddr).toString().toInt(NULL, 16);
        int playL = tmpSet.value(back + 0x40 + 0x14).toString().toInt(NULL, 16);  // 左启动
        int playR = tmpSet.value(back + 0x40 + 0x15).toString().toInt(NULL, 16);  // 右启动
        int stopL = tmpSet.value(back + 0x40 + 0x16).toString().toInt(NULL, 16);  // 左停止
        int stopR = tmpSet.value(back + 0x40 + 0x17).toString().toInt(NULL, 16);  // 右停止
        bool isPlay = (key == Qt::Key_Play || (playL & ioHexL) || (playR & ioHexL));
        if ((stopL & ioHexL) && (station == WORKL)) {  // 检测左停止,左右相互不影响
            taskCheckStop();
        }
        if ((stopR & ioHexL) && (station == WORKR)) {  // 检测右停止,左右相互不影响
            taskCheckStop();
        }
        if ((playL & ioHexL) && (stopL & ioHexL)) {
            str = tr("警告:光幕被遮挡!");
        }
        if ((playR & ioHexL) && (stopR & ioHexL)) {
            str = tr("警告:光幕被遮挡!");
        }
        if (isPlay && have == 1 && prod != 0 && ((ioHexL & prod) == 0))
            str = tr("警告:产品未放置!");
        if (!str.isEmpty()) {
            warnningString(str);
            return Qt::Key_Meta;
        }
        int taskplay = taskBuf.indexOf(&AppWindow::taskCheckPlay);
        if (currTask <= taskplay) {
            if ((playL & ioHexL) && !(stopL & ioHexL)) {  // 启动测试左
                taskShift = Qt::Key_Play;
                station = WORKL;
            }
            if ((playR & ioHexL) && !(stopR & ioHexL)) {  // 启动测试右
                taskShift = Qt::Key_Play;
                station = WORKR;
            }
        }
    }
    if (taskShift != Qt::Key_Play) {
        int btnL = tmpSet.value(back + 0x40 + 0x10).toString().toInt(NULL, 16);  // 左转向
        int btnR = tmpSet.value(back + 0x40 + 0x11).toString().toInt(NULL, 16);  // 右转向
        if (ioHexL & btnL) {
            station = WORKL;
            taskSendIobrd(0x01);
        }
        if (ioHexL & btnR) {
            station = WORKR;
            taskSendIobrd(0x01);
        }
    }
    return Qt::Key_Away;
}

int AppWindow::recvNoise(int mode)
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int playL = tmpSet.value(back + 0x40 + 0x14).toString().toInt(NULL, 16);  // 左启动
    int playR = tmpSet.value(back + 0x40 + 0x15).toString().toInt(NULL, 16);  // 右启动
    int stopL = tmpSet.value(back + 0x40 + 0x16).toString().toInt(NULL, 16);  // 左停止
    int stopR = tmpSet.value(back + 0x40 + 0x17).toString().toInt(NULL, 16);  // 右停止

    bool play = (ioHexL & playL) || (ioHexL & playR);  // 左噪音启动||右噪音启动
    bool stop = (ioHexL & stopL) || (ioHexL & stopR);  // 左噪音停止||右噪音停止

    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
    int nTime = tmpSet.value(conf + 0x08).toInt();  // 噪音时间
    if (nTime == 0)  // 噪音时间设置为0,不测试噪音
        return 0;
    if ((play && !stop) || mode == 1) {
        if (noiseTimer->isActive()) {
            warnningString(tr("噪音测试中\n不要重复启动"));
            return 0;  // 测试没退出,不启动
        }
        if (stack->currentWidget()->objectName() != "tester")
            return 0;  // 非测试界面,不启动
        int tmpwork = (ioHexL & playL) ? WORKL : WORKR;  // 左噪音启动||右噪音启动
        if (playL == 0)
            tmpwork = station;
        if (currTask > taskBuf.indexOf(&AppWindow::taskCheckPlay)) {
            warnningString(tr("当前测试中\n无法启动"));
            return 0;  // 当前工位正在测试,不启动
        }
        buffwork = tmpwork;
        int io = (tmpwork == WORKL) ? 0x0006 : 0x0005;
        sendUdpStr(QString("6036 %1").arg(io).toUtf8());  // 启动右工位噪音
        noiseTimer->start(100);
        noisetime = 0;
    }
    if ((stop || mode == 2) && noiseTimer->isActive()) {
        int tmpwork = (ioHexL & stopL) ? WORKL : WORKR;  // 左噪音停止||右噪音停止
        if (stopL == 0)
            tmpwork = station;
        if (tmpwork != buffwork) {  // 左启动右停止/右启动左停止
            warnningString(tr("噪音停止"));
            noiseTimer->stop();
            sendUdpStr(QString("6036 %1").arg(0).toUtf8());
            wait(500);
            warnningString(" ");
        }
    }
    return Qt::Key_Away;
}

void AppWindow::noiseThread()
{
    noisetime++;
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
    int time = tmpSet.value(conf + 0x08).toDouble()*10;
    if (noisetime == time) {
        noisetime = 0;
        noiseTimer->stop();
        warnningString(tr("噪音停止"));
        sendUdpStr(QString("6036 %1").arg(0).toUtf8());
        wait(500);
        warnningString(" ");
    }
}

void AppWindow::taskSendIobrd(int step)
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合设置地址
    int driv = tmpSet.value(conf + ADDRDRIV).toInt();  // 驱动器设置

    int downaddr = ((station == WORKL) ? 0x00 : 0x01) + back + 0x40;  // 下压动作左/右地址
    int grabaddr = ((station == WORKL) ? 0x02 : 0x03) + back + 0x40;  // 夹紧动作左/右地址
    int highaddr = ((station == WORKL) ? 0x04 : 0x05) + back + 0x40;  // 高压动作左/右地址
    int voltaddr = ((station == WORKL) ? 0x06 : 0x07) + back + 0x40;  // 低压动作左/右地址
    int beepaddr = ((station == WORKL) ? 0x08 : 0x09) + back + 0x40;  // 气动弹线左/右地址
    int saveaddr = ((driv == 0) ? 0x0A : 0x0B) + back + 0x40;  // 内驱/外驱保持
    int freeaddr = 0x0C + back + 0x40;
    int failaddr = ((isok == DATAOK) ? 0x0D : 0x0E) + back + 0x40;

    int down = tmpSet.value(downaddr).toString().toInt(NULL, 16);  // 下压动作
    int grab = tmpSet.value(grabaddr).toString().toInt(NULL, 16);  // 夹紧动作
    int high = tmpSet.value(highaddr).toString().toInt(NULL, 16);  // 高压动作
    int volt = tmpSet.value(voltaddr).toString().toInt(NULL, 16);  // 低压动作
    int beep = tmpSet.value(beepaddr).toString().toInt(NULL, 16);  // 弹线动作
    int free = tmpSet.value(freeaddr).toString().toInt(NULL, 16);  // 待机时保持
    int save = tmpSet.value(saveaddr).toString().toInt(NULL, 16);  // 内驱保持
    int fail = tmpSet.value(failaddr).toString().toInt(NULL, 16);  // 灯光动作

    int test = 0;
    switch (step) {
    case 0x00:  // 测试初始动作
        test = save + free;
        break;
    case 0x01:  // 测试常规动作
        test = save + down + grab;
        break;
    case 0x02:  // 高压动作
        test = save + down + grab + high;
        break;
    case 0x03:  // 低压动作
        test = save + down + grab + volt;
        break;
    case 0x04:  // 弹线动作
        test = save + down + beep + fail;
        break;
    case 0x05:  // 待机动作
        test = save + free + fail;
        break;
    default:
        break;
    }
    if (test != 0)
        sendUdpStr(tr("6036 %1").arg(test).toUtf8());
}

void AppWindow::showTester()
{
    recvAppShow("tester");
}

void AppWindow::showBarCode()
{
    int pAddr = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置参数地址
    int pCode = tmpSet.value(pAddr + SystCode).toInt();
    int pSize = tmpSet.value(pAddr + SystSize).toInt();
    int tAddr = tmpSet.value(3000 + Qt::Key_0).toInt();  // 临时参数地址
    int r = tmpSet.value(4000 + Qt::Key_0).toInt();
    int lenth = tmpSet.value(r + 0x07).toInt();
    pSize = (lenth != 0) ? lenth : pSize;
    tmpcode = tmpcode.mid(pCode, pSize);
    if (tmpcode.size() >= pSize) {
        barcode = tmpcode;
        tmpSet.insert(tAddr + TEMPCODE, tmpcode);
        sendSqlite();
        codeShift = Qt::Key_Away;
        tmpcode.clear();
    }
}

void AppWindow::saveConfig(QTmpMap msg)
{
    QString name = msg.value(Qt::Key_1).toString();
    if (name == "aip_tester") {
        tmpSave = msg;
        return;
    }
    btnFrame->setEnabled(false);
    QString tabName = (name == "aip_reload") ? "aip_system" : name;  // 重新加载
    QString sqlName = tabName.mid(4, 6);
    QString numb = tr("%1").arg(tmpSet.value(DataFile).toInt(), 4, 10, QChar('0'));
    QString file = tmpSet.value(DataType).toString();
    tabName = (sqlName == "config") ? tr("T%1_%2").arg(numb).arg(file) : tabName;
    QString cmd = QString("replace into %1 values(?,?)").arg(tabName);
    int max = 60000;
    int min = 40000;
    max = (sqlName == "backup") ? 20000 : max;  // 后台参数
    min = (sqlName == "backup") ? 10000 : min;
    max = (sqlName == "system") ? 30000 : max;  // 系统参数
    min = (sqlName == "system") ? 20000 : min;

    taskparm.remove("tasksave");
    boxbar->setLabelText(tr("正在保存数据"));
    boxbar->show();
    wait(10);

    QSqlQuery query(QSqlDatabase::database(sqlName));
    QSqlDatabase::database(sqlName).transaction();
    QList<int> uuids = msg.keys();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        tmpSet.insert(uuid, msg.value(uuid));
        if (uuid >= min && uuid < max) {
            query.prepare(cmd);
            query.addBindValue(uuid);
            query.addBindValue(tmpSet.value(uuid));
            if (!query.exec()) {
                qWarning() << query.lastError();
            }
        }
        boxbar->setValue(i*50/uuids.size());
    }
    QSqlDatabase::database(sqlName).commit();
    wait(500);
    boxbar->setValue(100);
    boxbar->hide();
    query.clear();

    if (name == "aip_reload") {  // 重新加载参数
        readConfig();
        sendSqlite();
        taskConfig();
    } else {
        sendSqlite();
    }
    wait(50);
    btnFrame->setEnabled(true);
    qDebug() << "app save:" << name << sqlName << tabName;
}

void AppWindow::clickButtons()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_View);
    tmpMsg.insert(Qt::Key_1, sender()->objectName());
    recvAppMsg(tmpMsg);
    tmpMsg.clear();
}

bool AppWindow::checkAction(QString msg)
{
    int temp = tmpSet.value(3000 + Qt::Key_0).toInt();  // 临时数据地址
    int sign = tmpSet.value(temp + TEMPSIGN).toInt();  // 是否已登录
    int real = tmpSet.value(DataUser).toInt();  // 当前用户地址
    int gear = (sign == 1) ? tmpSet.value(real + mRole).toInt() : 4;  // 当前权限等级
    int addr = tmpSet.value(2000 + Qt::Key_6).toInt();  // 界面权限地址
    int role = 0;
    QVariantMap win;
    for (int i=0; i < bufwin.size(); i++) {
        QVariantMap tmp = bufwin.at(i);
        if (tmp.value("name") == msg) {
            win = tmp;
            int g = tmp.value("form").toInt();
            int n = tmp.value("numb").toInt();
            role = tmpSet.value(addr + g*STEP + n).toInt();
            break;
        }
    }
    if (win.isEmpty())
        return false;

    QStringList gs;
    for (int i=0; i < bufwin.size(); i++) {
        QVariantMap tmp = bufwin.at(i);
        int p = win.value("form").toInt();
        int g = tmp.value("form").toInt();
        int n = tmp.value("numb").toInt();
        int r = tmpSet.value(addr + g*STEP + n).toInt();  // 界面权限
        int m = 1;
        if (p == 2 && n != 0) {  // 配置界面
            int conf = tmpSet.value(1000 + Qt::Key_0).toInt() + STEP;  // 后台功能选择地址
            m = tmpSet.value(conf + n - 1).toInt();
        }
        if ((p == g && r >= gear && m != 0) || g == 0)
            gs.append(tmp.value("name").toString());
    }
    for (int i=0; i < buttons.size(); i++) {
        buttons.at(i)->setVisible(gs.contains(buttons.at(i)->objectName()));
        buttons.at(i)->setChecked(buttons.at(i)->objectName() == msg);
    }
    btnFrame->setVisible(win.value("form").toInt() > 0);
    if (gear > role)
        return false;
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    return true;
}

bool AppWindow::screensSave(QString msg)
{
    if (taskparm.value("tasksave").toInt() == 1) {
        QString w = tr("数据未保存,确定退出吗?");
        int ret = QMessageBox::warning(this, tr("警告"), w, QMessageBox::Close | QMessageBox::Save);
        if (ret == QMessageBox::Save) {
            tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
            tmpMsg.insert(Qt::Key_1, msg);
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
    }
    taskparm.remove("tasksave");
    return true;
}

void AppWindow::screensShow(QString msg)
{
    qDebug() << "app show:" << msg << this->size();
    for (int i=0; i < stack->count(); i++) {
        if (stack->widget(i)->objectName() == msg) {
            if (preindex == i)
                break;
            stack->setCurrentIndex(i);
            cloudAntimation();
            preindex = i;
            break;
        }
    }
    if (msg == "tester")
        recvAppPrep();
}

void AppWindow::recvAppShow(QString msg)
{
    screensSave(msg);
    if (!checkAction(msg)) {
        QString w = tr("您无权限进行此操作,请重新登录");
        QMessageBox::warning(this, tr("警告"), w, QMessageBox::Ok);
        screensShow("signin");
    } else {
        screensShow(msg);
    }
}

void AppWindow::recvAppPrep()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int spec = tmpSet.value(back + backTest).toString().toInt(NULL, 16);  // 特殊配置
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合设置地址
    int driv = tmpSet.value(conf + ADDRDRIV).toInt();
    int syst = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置地址
    int snap = tmpSet.value(syst + SystMode).toInt();  // 弹线配置
    double tmOK = tmpSet.value(syst + SystTime).toDouble();
    double tmNG = tmpSet.value(syst + SystWarn).toDouble();
    double tmWT = tmpSet.value(syst + SystWait).toDouble();
    if (mode != 1) {
        sendUdpStr("6058 1");  // IO状态上传
        wait(100);
    }
    if (mode >= 2) {
        int addr = tmpSet.value(4000 + Qt::Key_C).toInt();  // 负载配置地址
        if (mode == 2 || mode == 4) {  // 无刷模式
            int saveaddr = ((driv == 0) ? 0x0A : 0x0B) + back + 0x40;
            int freeaddr = 0x0C + back + 0x40;
            int free = tmpSet.value(freeaddr).toString().toInt(NULL, 16);  // 待机时保持
            int save = tmpSet.value(saveaddr).toString().toInt(NULL, 16);  // 内驱保持
            sendUdpStr(tr("6036 %1").arg(save + free).toUtf8());
        }
        if (mode == 3) {
            sendUdpStr(tr("6072 %1").arg(driv).toUtf8());  // 内外置接触器切换
            wait(100);
            sendUdpStr(tr("6073 %1").arg(driv).toUtf8());  // 内外置接触器切换
            wait(100);
        }
        if (mode == 4)
            addr = tmpSet.value(4000 + Qt::Key_D).toInt();  // 空载配置地址
        int stop = (driv == 1) ? 1 : 2;
        int work = (station == WORKL) ? 1 : 2;
        int volt = (driv == 1) ? tmpSet.value(addr + 0x10).toInt()*100 : 0;
        int powr = tmpSet.value(addr + 0x24).toInt();
        // 开关 工位 Vm 电源 电压
        sendUdpStr(tr("6074 %1 %2 3 %3 %4").arg(stop).arg(work).arg(powr).arg(volt).toUtf8());
        wait(100);
        sendUdpStr("6008");  // 进入测试界面
        wait(500);
    }
    if (mode == 1) {  // 真空模式
        sendUdpStr("6008");  // 进入测试界面
        wait(200);
        QStringList testItems = tmpSet.value(conf + ADDRITEM).toString().split(",");
        int imp = tmpSet.value(4000 + Qt::Key_6).toInt();
        int vac = tmpSet.value(imp + 0x00).toInt();
        vac = (testItems.contains(QString::number(0x06))) ? vac : 2;
        sendUdpStr(tr("6052 %1").arg(vac).toUtf8());
        wait(200);
        int acw = tmpSet.value(4000 + Qt::Key_4).toInt();
        vac = tmpSet.value(acw + CACHEACW*0x0A + 5).toInt();
        vac = (testItems.contains(QString::number(0x10))) ? vac : 2;
        sendUdpStr(tr("6093 %1").arg(vac).toUtf8());

        vac = tmpSet.value(acw + CACHEACW*0x0A + 4).toInt();
        vac = (testItems.contains(QString::number(0x04))) ? vac : 2;
        sendUdpStr(tr("6068 %1").arg(vac).toUtf8());
        if (1) {
            int addr = tmpSet.value(4000 + Qt::Key_0).toInt();  // 自动启动
            QString str = QString::number(tmpSet.value(addr + ADDRAUTO).toInt());
            sendUdpStr(tr("6067 %1").arg(str).toUtf8());
            wait(200);
        }
        if (1) {  // 弹线配置
            sendUdpStr(tr("6091 %1").arg(snap).toUtf8());
        }
        if (1) {  // 光幕反转
            sendUdpStr(tr("6108 %1").arg((spec&0x20) ? 1 : 0).toUtf8());
        }
    }
    if (mode == 0) {
        sendUdpStr("6008");  // 进入测试界面
        wait(200);
    }
    sendUdpStr(tr("6071 %1 %2").arg(tmOK).arg(tmNG).toUtf8());  // 报警时间
    wait(200);
    sendUdpStr("6094 0");  // 停用防呆
    sendUdpStr(tr("6096 %1 %2").arg((mode == 0 ? 1 : 0)).arg(tmWT*1000).toUtf8());
    taskparm.insert("pumpstop", 0);
}

void AppWindow::cloudAntimation()
{
    QLabel* circle = new QLabel(stack->currentWidget());
    QLabel* line = new QLabel(this);
    line->setObjectName(QString("AntimationLine"));
    line->resize(0, 2);
    line->show();
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    circle->setPixmap(QPixmap::grabWindow(stack->widget(preindex)->winId()));
#else
    circle->setPixmap(stack->widget(preindex)->grab());
#endif
    circle->show();
    circle->resize(stack->currentWidget()->size());
    QPropertyAnimation *animation = new QPropertyAnimation(circle, "geometry");

    animation->setDuration(1000);
    animation->setStartValue(circle->geometry());

    QPropertyAnimation* animation_line = new QPropertyAnimation(line, "size");
    animation_line->setDuration(1000);
    animation_line->setEasingCurve(QEasingCurve::OutCubic);

    animation->setEndValue(QRect(-3, 0, 0, circle->height()));
    line->move(stack->x(), stack->y() - 2);
    animation_line->setStartValue(QSize(0, 2));
    animation_line->setEndValue(QSize(stack->width(), 2));

    animation->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* animation_opacity = new QPropertyAnimation(circle, "windowOpacity");
    animation_opacity->setDuration(1000);
    animation_opacity->setStartValue(1);
    animation_opacity->setEndValue(0);
    animation_opacity->setEasingCurve(QEasingCurve::OutCubic);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;

    connect(group, SIGNAL(finished()), circle, SLOT(hide()));
    connect(group, SIGNAL(finished()), circle, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), line, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), group, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation_opacity, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation_line, SLOT(deleteLater()));
    group->addAnimation(animation);
    group->addAnimation(animation_opacity);
    group->addAnimation(animation_line);
    group->start();
}

void AppWindow::closeEvent(QCloseEvent *e)
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int addr = tmpSet.value(4000 + Qt::Key_D).toInt();  // 空载配置地址

    if (mode == 1) {  // 真空模式
        sendUdpStr("6052 0");
        sendUdpStr("6068 0");
        sendUdpStr("6067 0");
    }
    if (mode == 2) {  // 无刷模式
        sendUdpStr("6036 0");
    }
    int work = (station == WORKL) ? 1 : 2;
    int powr = tmpSet.value(addr + 0x24).toInt();
    // 开关 工位 Vm 电源 电压
    sendUdpStr(tr("6074 %1 %2 3 %3 %4").arg(2).arg(work).arg(powr).arg(0).toUtf8());
    sendUdpStr(tr("6074 %1 %2 1 %3 %4").arg(2).arg(work).arg(powr).arg(0).toUtf8());
    e->accept();
}

void AppWindow::display(int item, int numb, int mode, QString str, int work)
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_News);
    tmpMsg.insert(Qt::Key_1, item);
    tmpMsg.insert(Qt::Key_2, numb);
    tmpMsg.insert((mode == 0) ? Qt::Key_3 : Qt::Key_4, str);
    tmpMsg.insert(Qt::Key_6, work);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppWindow::loopBoxbar()
{
    int t = 0;
    while (1) {
        if (boxbar->isHidden())
            break;
        boxbar->setValue(t);
        wait(100);
        t = (t + 1) % 100;
    }
}

void AppWindow::warnningString(QString dat)
{
    if (!dat.isEmpty()) {
        QLabel *warnning = new QLabel(this);
        warnning->setAttribute(Qt::WA_TranslucentBackground, true);
        warnning->setText(dat);
        warnning->setStyleSheet("qproperty-alignment:AlignCenter;color:yellow; font:108pt");
        warnning->resize(this->size());
        warnning->show();
        QTimer *timer = new QTimer(this);
        timer->singleShot(2000, warnning, SLOT(deleteLater()));
        timer->deleteLater();
        qWarning() << dat;
        tmpMsg.insert(Qt::Key_0, Qt::Key_Word);
        tmpMsg.insert(Qt::Key_1, dat);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void AppWindow::recvSqlMsg(QTmpMap msg)
{
    if (msg.value(Qt::Key_1).toString() == "copy") {  // 开始备份
        boxbar->setLabelText(tr("正在备份数据"));
        boxbar->show();
        QTimer::singleShot(10, this, SLOT(loopBoxbar()));
    }
    if (msg.value(Qt::Key_1).toString() == "over") {  // 备份完成
        boxbar->setValue(100);
        boxbar->hide();
    }
}

void AppWindow::recvAppMsg(QTmpMap msg)
{
    int tmp;
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_View:
        recvAppShow(msg.value(Qt::Key_1).toString());
        break;
    case Qt::Key_Back:
        tmpSet[tmpSet.value((3000 + Qt::Key_0)).toInt() + TEMPSIGN] = 0;
        recvAppShow("signin");
        break;
    case Qt::Key_Copy:
        recvSqlMsg(msg);
        break;
    case Qt::Key_Save:
        saveConfig(msg);
        break;
    case Qt::Key_Play:
        tmp = (msg.value(Qt::Key_1).toString() == "R") ? 0x14 : 0x13;
        if (recvIoCtrl(Qt::Key_Play, tmp) == Qt::Key_Away) {
            taskShift = Qt::Key_Play;
            station = tmp;
        }
        break;
    case Qt::Key_Stop:  // 停止测试
        if (!msg.value(Qt::Key_1).isNull()) {
            sendUdpStr("6081");
            taskCheckStop();
            break;
        }
        taskCheckStop();
        break;
    case Qt::Key_Call:  // 数据修改未保存
        taskparm.insert("tasksave", 1);
        break;
    case Qt::Key_WLAN:
        emit sendAppMsg(msg);
        break;
    case Qt::Key_Xfer:
        tmpSet = msg;
        break;
    case Qt::Key_Flip:
#ifdef __arm__
        emit sendNetMsg(msg);
#else
        qDebug() << "tcp recv:" << msg.value(Qt::Key_1).toString();
#endif
        break;
    case Qt::Key_Book:
        boxbar->setLabelText(tr("正在查询数据"));
        boxbar->setValue(1);
        boxbar->show();
        wait(1);
        emit sendAppMsg(msg);
        break;
    case Qt::Key_Word:
        if (!msg.value(Qt::Key_1).isNull())
            boxbar->setLabelText(msg.value(Qt::Key_1).toString());
        if (!msg.value(Qt::Key_2).isNull()) {
            if (msg.value(Qt::Key_2).toInt() >= 100) {
                wait(1000);
                boxbar->setValue(100);
                boxbar->hide();
            } else {
                boxbar->setValue(msg.value(Qt::Key_2).toInt());
            }
        }
        break;
    case Qt::Key_Game: {
        int back = tmpSet.value(1000 + Qt::Key_0).toInt();
        int mode = tmpSet.value(back + backMode).toInt();
        int conf = tmpSet.value(2000 + Qt::Key_7).toInt();
        int care = tmpSet.value(conf + 0x00).toInt();
        if (mode == 2 && care == 1) {  // 无刷模式,自动进入界面
            recvAppShow("repair");
        }
        break;
    }
    case Qt::Key_Down:
        tmpcode = msg.value(Qt::Key_1).toString();
        showBarCode();
        break;
    default:
        break;
    }
}

void AppWindow::recvLibMsg(QVariantMap msg)
{
    int c = msg.value("enum").toInt();
    switch (c) {
    case Qt::Key_Save: {
        QProgressDialog *tmpbox = new QProgressDialog(tr("正在保存"), tr("隐藏"), 0, 100, this);
        tmpbox->setWindowTitle(tr("保存进度框"));
        tmpbox->show();
        for (int i=0; i < 101; i++) {
            tmpbox->setValue(i);
            wait(5);
        }
        break;
    }
    default:
        break;
    }
}

void AppWindow::recvXmlMap(QVariantMap msg)
{
    QStringList keys = msg.keys();
    keys.removeOne("enum");
    for (int i=0; i < keys.size(); i++) {
        QDomDocument doc;
        QDomElement root;
        root = doc.createElement(keys.at(i));
        doc.appendChild(root);

        QVariantMap tmpMap = msg.value(keys.at(i)).toMap();
        QStringList key = tmpMap.keys();
        for (int t=0; t < key.size(); t++) {
            QDomElement element = doc.createElement(key.at(t));
            QDomText text = doc.createTextNode(tmpMap.value(key.at(t)).toString());
            if (key.at(t) == "vcc_volt") {
                double vcc = tmpMap.value(key.at(t)).toString().toDouble();
                double offset = tmpMap.value("vcc_offset").toString().toDouble();
                text = doc.createTextNode(QString::number(vcc+offset));
            }
            if (key.at(t) == "vsp_volt") {
                double vcc = tmpMap.value(key.at(t)).toString().toDouble();
                double offset = tmpMap.value("vsp_offset").toString().toDouble();
                text = doc.createTextNode(QString::number(vcc+offset));
            }
            element.appendChild(text);
            root.appendChild(element);
        }
        sendUdpStr(QString("6002 %1").arg(QString(doc.toByteArray())).toUtf8());
        tmpMap.clear();
    }
}

void AppWindow::recvAppMap(QVariantMap msg)
{
    int c = msg.value("enum").toInt();
    switch (c) {
    case Qt::Key_Save:
        wait(200);
        recvXmlMap(msg);
        break;
    case Qt::Key_View:
        sendUdpStr(msg.value("text").toByteArray());
        break;
    case Qt::Key_Shop: {  // 设备预热
        int speed = msg.value("data").toInt();
        isSampling = (speed == 0) ? false : true;
        if (speed == 0) {
            taskCheckStop();
            if (taskShift != Qt::Key_Stop)
                sendUdpStr(tr("6036 0").toUtf8());
        } else {
            recvAppPrep();
            wait(2000);
            clearBatch();
            taskShift = Qt::Key_Play;
        }
        break;
    }
    case Qt::Key_Word: {
        warnningString(msg.value("text").toString());
        break;
    }
    case Qt::Key_Send: {
        int speedmax = testparm.value("speedmax").toInt();
        int speedtst = msg.value("taskdata").toInt();
        speedmax = abs(speedmax) > abs(speedtst) ? speedmax : speedtst;
        testparm.insert("speedmax", speedmax);
        break;
    }
    case Qt::Key_Call:
        codeShift = Qt::Key_Away;
        tmpcode = msg.value("taskdata").toByteArray();
        showBarCode();
        break;
    default:
        break;
    }
}

void AppWindow::recvSocket(QByteArray msg)
{
    quint32 host;
    QByteArray tmp;
    QDataStream in(&msg, QIODevice::ReadOnly);
    in >> host >> tmp;
    if (tmp.startsWith("0x31") || tmp.startsWith("6001")) {
        taskparm.remove("tasktime");
        if (taskparm.value("taskhand").toInt() == 0) {
            QTimer::singleShot(1000, this, SLOT(taskConfig()));
        }
        taskparm.insert("taskhand", 1);
    } else if (!tmp.startsWith("6032")) {
        qDebug() << "udp recv:" << tmp;
    }
    int cmd = tmp.mid(0, 4).toInt();
    QString dat = tmp.mid(5, tmp.size());
    quint32 hex = dat.toInt();
    switch (cmd) {
    case 6001:  // 自检信息
        recvStaMsg(dat);
        break;
    case 6003:
        recvErrMsg(dat);
        break;
    case 6005:  // 上传测试界面显示信息
        recvNewMsg(dat);
        break;
    case 6007:  // 单项测试完成
        testShift = Qt::Key_Away;
        if (tempShift == Qt::Key_Away)
            taskCheckStop();
        if (currItem == nSetPWR) {
            recvPwrMsg(strPwr, 1);
            testCheckWarn();
        }
        if (currItem == nSetLOD || currItem == nSetNLD || currItem == 0x0F)
            calcLOAD(strLoad, 1);
        if (currItem == nSetEMF) {
            calcBEMF(strLoad, 1);
        }
        break;
    case 6015:  // 空载/负载启动完成
        break;
    case 6019:  // 空载/负载/霍尔结果
        if (currItem == 0x0B) //  霍尔
            calcHALL(dat);
        if (currItem == 0x0C) // 负载
            calcLOAD(dat, 0);
        if (currItem == 0x0D)
            calcLOAD(dat, 0);
        if (currItem == 0x0E)  // 反电动势
            calcBEMF(dat, 0);
        if (currItem == 0x0F)
            calcLOAD(dat, 0);
        break;
    case 6021:  // 空载/负载/反电势波形
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, cmd);
        tmpMsg.insert(Qt::Key_2, dat);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case 6032:  // 更新温度
        tmpMsg.insert(Qt::Key_0, Qt::Key_Shop);
        tmpMsg.insert(tmpSet.value(3000 + Qt::Key_0).toInt() + TEMPTEMP, dat);
        tmpSet.insert(tmpSet.value(3000 + Qt::Key_0).toInt() + TEMPTEMP, dat);
#ifdef __arm__
        emit sendNetMsg(tmpMsg);
#endif
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case 6033:  // 负载启动完成
        break;
    case 6037:  // IO板输入状态
        ioHexL = hex;
        recvIoCtrl(Qt::Key_Meta, station);
        recvNoise(0);
    case 6086:  // 上传采样转向
    case 6059:  // IO板输出状态
    case 6035:  // 反嵌采样结果
    case 6039:  // 反嵌采样波形
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, cmd);
        tmpMsg.insert(Qt::Key_5, dat);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case 6099:
        ioHexR = hex;
        recvIoCtrl(Qt::Key_Meta, station);
        break;
    case 6040:  // 反嵌测试波形
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, currItem);
        tmpMsg.insert(Qt::Key_2, timeMag);
        tmpMsg.insert(Qt::Key_5, dat);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        timeMag++;
        break;
    case 6042:  // 匝间采样波形
    case 6055:  // 匝间采样结果
    case 6085:
    case 6105:  // 匝间直线
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, cmd);
        tmpMsg.insert(Qt::Key_5, dat);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case 6043:  // 匝间测试波形
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, currItem);
        tmpMsg.insert(Qt::Key_2, timeImp);
        tmpMsg.insert(Qt::Key_5, dat);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        timeImp++;
        break;
    case 6053: {  // A8询问是否真空模式
        int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合设置地址
        QStringList testItems = tmpSet.value(conf + ADDRITEM).toString().split(",");
        int imp = tmpSet.value(4000 + Qt::Key_6).toInt();
        int vac = tmpSet.value(imp + 0x00).toInt();
        vac = (testItems.contains(QString::number(0x06))) ? vac : 2;
        sendUdpStr(tr("6052 %1").arg(vac).toUtf8());
        break;
    }
    case 6060:  // 真空上传启动信号
        if (recvIoCtrl(Qt::Key_Play, dat.toInt()) == Qt::Key_Away) {
            taskShift = Qt::Key_Play;
            prevShift = (station != dat.toInt()) ? Qt::Key_Play : Qt::Key_Meta;
            station = dat.toInt();
            tmpMsg.insert(Qt::Key_0, Qt::Key_Play);
            tmpMsg.insert(Qt::Key_1, station == WORKL ? "L" : "R");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        break;
    case 6061:  // 真空上传停止信号
        if (currTask > taskBuf.indexOf(&AppWindow::taskCheckPlay))
            taskCheckStop();
        tmpMsg.insert(Qt::Key_0, Qt::Key_Stop);
        tmpMsg.insert(Qt::Key_1, station == WORKL ? "L" : "R");
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case 6083: {
        testparm.insert("teststat", 2);
        int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
        int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
        if (mode == 1) {
            isok = (dat == "NG") ? DATANG : DATAOK;
            taskCheckStop();
        }
        break;
    }
    case 6062:  // 真空泵待机
        taskparm.insert("pumpstop", 1);
        warnningString(tr("真空泵已关闭"));
        break;
    case 6076:  // 上传RFID
        codeShift = Qt::Key_Away;
        tmpcode = dat.toUpper();
        showBarCode();
        break;
    case 6087:
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, cmd);
        tmpMsg.insert(Qt::Key_5, dat);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case 6088:
        testparm.insert("taskturn", dat);
        break;
    case 6089:
        recvPwrMsg(dat, 0);
        break;
    case 6097:
        if (boxsave != NULL) {
            if (dat.toInt() == 0)
                boxsave->button(QMessageBox::Yes)->click();
            if (dat.toInt() == 1)
                boxsave->button(QMessageBox::No)->click();
        }
        break;
    case 6102: {  // 端盖电阻
        if (currItem == nSetNLD) {
            int addr = tmpSet.value(3000 + Qt::Key_D).toInt();  // 空载结果地址
            int conf = tmpSet.value(4000 + Qt::Key_D).toInt();  // 空载配置地址
            double max = tmpSet.value(conf + 6*2 + 0).toDouble();
            double min = tmpSet.value(conf + 6*2 + 1).toDouble();
            double real = (dat.remove(">").remove("Ω").remove("k").remove("m")).toDouble();
            QString r = (real >= max || real < min) ? "NG" : "OK";
            if (max != 0) {
                isok = (real >= max || real < min) ? DATANG : isok;
                if (real > 200) {
                    dat = ">200mΩ";
                    r = "NG";
                }
                tmpMsg.insert(Qt::Key_0, Qt::Key_News);
                tmpMsg.insert(Qt::Key_1, currItem);
                tmpMsg.insert(Qt::Key_2, 6);
                tmpMsg.insert(Qt::Key_3, dat);
                tmpMsg.insert(Qt::Key_4, r);
                tmpMsg.insert(Qt::Key_6, station);
                emit sendAppMsg(tmpMsg);
                tmpMsg.clear();
                tmpSave.insert(addr + 6*0x10 + 1, real);
                tmpSave.insert(addr + 6*0x10 + 2, r);
            }
        }
        break;
    }
    case 6109:
        leek = dat.toLongLong();
        break;
    default:
        break;
    }
}

void AppWindow::sendUdpStr(QByteArray msg)
{
    quint32 host = taskparm.value("taskhost").toInt();
    QByteArray dat;
    QDataStream out(&dat, QIODevice::WriteOnly);
    out << host << msg;
    emit sendReady(dat);
    if (!msg.startsWith("6000") && !msg.startsWith("6030"))
        qDebug() << "udp send:" << msg;
}

void AppWindow::recvNewMsg(QString dat)
{
    if (currItem >= 0x0B && currItem != 0x10)  // BLDC不显示
        return;
    if (currItem == 0)
        return;
    if (currItem == nSetPWR) {
        recvPwrMsg(strPwr, 1);
        return;
    }
    if (taskShift == Qt::Key_Stop)
        return;
    int testisok = DATAOK;
    QString err;
    QString xml;
    QDomDocument docs;
    docs.setContent(dat, &err);
    if (!err.isEmpty()) {
        qDebug() << "xml read:" << err;
        return;
    }
    xml = (docs.elementsByTagName("Test_Data_Result").isEmpty()) ? xml : "Test_Data_Result";
    xml = (docs.elementsByTagName("Test_Data_Judge").isEmpty()) ? xml : "Test_Data_Judge";
    if (xml.isEmpty()) {
        qDebug() << "xml read:" << "no data";
        return;
    }
    QDomNodeList list = docs.elementsByTagName(xml).at(0).childNodes();
    for (int i=0; i < list.size(); i++) {
        QDomElement dom = list.at(i).toElement();
        QString temp = dom.text();
        if (dom.nodeName() == "Test_3") {
            if (xml == "Test_Data_Result") {
                bool isNoun = false;
                isNoun = (currItem == 0x01) && (!temp.contains("Ω"));
                timeRsl = (isNoun && (timeRsl != 0x09)) ? 0x08 : timeRsl;
            }
            bool isTurn = false;
            isTurn = ((currItem == nSetMAG) && (temp.contains(tr("正转")))) ? true : isTurn;
            isTurn = ((currItem == nSetMAG) && (temp.contains(tr("反转")))) ? true : isTurn;
            isTurn = ((currItem == nSetMAG) && (temp.contains(tr("不转")))) ? true : isTurn;
            if (isTurn) {
                timeRsl = (timeRsl == 0x09) ? 0x09 : 0x08;
            }
            timeRsl = ((currItem == nSetPWR) && (temp.contains(tr("正转")))) ? 0x03 : timeRsl;
            timeRsl = ((currItem == nSetPWR) && (temp.contains(tr("反转")))) ? 0x03 : timeRsl;
            timeRsl = ((currItem == nSetPWR) && (temp.contains(tr("不转")))) ? 0x03 : timeRsl;
            timeRsl = ((currItem == nSetIND) && (temp.contains(tr("%")))) ? 0x08 : timeRsl;
            temp = temp.contains(tr("正转")) ? "CW" : temp;
            temp = temp.contains(tr("反转")) ? "CCW" : temp;
            temp = temp.contains(tr("不转")) ? "NULL" : temp;

            if (currItem != nSetLCK && currItem != nSetLVS) {
                tmpMsg.insert(Qt::Key_0, Qt::Key_News);
                tmpMsg.insert(Qt::Key_1, currItem);
                tmpMsg.insert(Qt::Key_2, timeRsl);
                tmpMsg.insert((xml == "Test_Data_Result") ? Qt::Key_3 : Qt::Key_4, temp);
                tmpMsg.insert(Qt::Key_6, station);
                emit sendAppMsg(tmpMsg);
                tmpMsg.clear();
            }

            int addr = tmpSet.value(3000 + Qt::Key_0 + currItem).toInt() + timeRsl*0x10;
            int conf = tmpSet.value(4000 + Qt::Key_0 + currItem).toInt();
            if (currItem == 0x10) {
                addr = tmpSet.value(3000 + Qt::Key_4).toInt() + 0x10;
            }
            if (xml == "Test_Data_Judge") {
                if (currItem != 0x06)
                    tmpSave.insert(addr + 0x03, temp);
                isok = (temp == "OK") ? isok : DATANG;
                testisok = (temp == "OK") ? testisok : DATANG;
                timeRsl++;
            } else {
                if (currItem == 0x03 || currItem == 0x04 ||
                        currItem == 0x05 || currItem == 0x10) {  // 绝缘,交耐,直耐
                    QStringList reals = temp.split(",");
                    temp = reals.last();  // 结果在最后
                }
                if (currItem == 0x06) {
                    temp = temp.remove(tr("面积:"));
                    temp = temp.remove(tr("差积:"));
                    temp = temp.remove(tr("电晕:"));
                    temp = temp.remove(tr("相位:"));
                    QList<double> parm;
                    parm << tmpSet.value(conf + CACHEIMP + CACHEIMP*AREAIMP1 + timeRsl).toDouble()
                         << tmpSet.value(conf + CACHEIMP + CACHEIMP*DIFFIMP1 + timeRsl).toDouble()
                         << tmpSet.value(conf + CACHEIMP + CACHEIMP*FLUTIMP1 + timeRsl).toDouble()
                         << tmpSet.value(conf + CACHEIMP + CACHEIMP*PHSEIMP1 + timeRsl).toDouble();
                    QStringList reals = temp.split(",");
                    for (int numb=0; numb < qMin(parm.size(), reals.size()); numb++) {
                        volatile double rr = reals.at(numb).toDouble();
                        volatile double ss = parm.at(numb);
                        QString str = ((abs(rr*100) > ss*100) ? "NG" : "OK");
                        tmpSave.insert(addr + numb*3 + 0x02, rr);
                        tmpSave.insert(addr + numb*3 + 0x03, str);
                    }
                    continue;
                }
                if (currItem == nSetLCK || currItem == nSetLVS) {
                    int add = (currItem == nSetLCK) ? Qt::Key_9 : Qt::Key_A;
                    int conf = tmpSet.value(4000 + add).toInt();  // 霍尔配置地址
                    double imax = tmpSet.value(conf + CACHELVS + CACHELVS*CMAXLVS1).toDouble();
                    double imin = tmpSet.value(conf + CACHELVS + CACHELVS*CMINLVS1).toDouble();
                    double pmax = tmpSet.value(conf + CACHELVS + CACHELVS*PMAXLVS1).toDouble();
                    double pmin = tmpSet.value(conf + CACHELVS + CACHELVS*PMINLVS1).toDouble();
                    QStringList tt = temp.split(",");
                    if (imax > 0 && tt.size() >= 2) {
                        QString str = tt.at(0);
                        display(currItem, 0x00, 0, str, WORKL);
                        double ii = str.remove("A").toDouble();
                        isok = (ii >= imax || ii < imin) ? DATANG : isok;
                        display(currItem, 0x00, 1, (ii >= imax || ii < imin) ? "NG" : "OK", WORKL);
                    }
                    if (pmax > 0 && tt.size() >= 2) {
                        QString str = tt.at(1);
                        display(currItem, 0x01, 0, str, WORKL);
                        double ii = str.remove("W").toDouble();
                        isok = (ii >= pmax || ii < pmin) ? DATANG : isok;
                        display(currItem, 0x01, 1, (ii >= pmax || ii < pmin) ? "NG" : "OK", WORKL);
                    }
                }
                tmpSave.insert(addr + 0x02, temp);
            }
        }
    }
    testCheckWarn();
}

void AppWindow::recvStaMsg(QString msg)
{
    if (msg.isEmpty())
        return;
    QStringList errs;
    errs << tr("输出板异常") << tr("电阻板异常") << tr("绝缘板异常") << tr("匝间板异常")
         << tr("电参板异常") << tr("电感板异常") << tr("左输出板异常") << tr("右输出板异常")
         << tr("系统接地异常") << tr("电源切换板异常") << tr("内存空间不足") << tr("WD板异常")
         << tr("PG板异常") << tr("IO板异常") << tr("相序异常") << tr("热继电器异常")
         << tr("输出板异常复位\n请拍照") << tr("IO板异常复位\n请拍照")
         << tr("未检测到样品，\n请拿开样品重新放置");

    QStringList dat = msg.split(" ");
    QString err;
    for (int i=0; i < dat.size(); i++) {
        int cmd = dat.at(i).toInt();
        if (cmd >= errs.size() && cmd < 0xFFFF) {
            err += tr("未知异常") + QString::number(cmd) + "\n";
        }
        if (cmd <= errs.size()) {
            if ((cmd != 18) || (cmd == 18 && (ioHexL & 0x004000) != 0))
                err += errs.at(cmd) + "\n";
        }
    }
    if (!err.isEmpty()) {
        warnningString(err);
    }
}

void AppWindow::recvErrMsg(QString msg)
{
    if (msg.isEmpty())
        return;
    warnningString(msg);
    taskCheckStop();
}

void AppWindow::recvPwrMsg(QString msg, int ext)
{
    strPwr = msg;
    int taskback = tmpSet.value(1000 + Qt::Key_0).toInt();
    int taskspec = tmpSet.value(taskback + backTest).toString().toInt(NULL, 16);  // 特殊配置
    QStringList strs = msg.split(" ");
    if (strs.size() >= 9) {
        int real = tmpSet.value(3000 + Qt::Key_7).toInt();  // 电参结果地址
        int conf = tmpSet.value(Qt::Key_7 + 4000).toInt() + CACHEPWR;
        double itemproj = strs.last().toInt() % 99 - 1;  // 第几个功率
        double cmax = tmpSet.value(conf + CACHEPWR*CMAXPWR1 + itemproj).toDouble();
        double cmin = tmpSet.value(conf + CACHEPWR*CMINPWR1 + itemproj).toDouble();
        double pmax = tmpSet.value(conf + CACHEPWR*PMAXPWR1 + itemproj).toDouble();
        double pmin = tmpSet.value(conf + CACHEPWR*PMINPWR1 + itemproj).toDouble();
        double vmax = tmpSet.value(conf + CACHEPWR*VMAXPWR1 + itemproj).toDouble();
        double vmin = tmpSet.value(conf + CACHEPWR*VMINPWR1 + itemproj).toDouble();
        double turn = tmpSet.value(conf + CACHEPWR*TURNPWR1 + itemproj).toDouble();
        QStringList turns;
        turns << tr("不转") << tr("反转") << tr("正转");
        QStringList ccws;
        if (taskspec & 0x40) {  // 无锡寰宇转向反向
            ccws << "NULL" << "CW" << "CCW";
        } else {
            ccws << "NULL" << "CCW" << "CW";
        }
        double volt = strs.at(0).toDouble();
        double curr = strs.at(1).toDouble();
        double pwrs = strs.at(2).toDouble();
        double vcap = strs.at(6).toDouble();
        double itemturn = turns.indexOf(strs.at(8));

        display(currItem + 100 * itemproj, 0x00, 0, QString("%1V %2A").arg(volt).arg(curr), WORKL);
        display(currItem + 100 * itemproj, 0x01, 0, QString("%1W").arg(pwrs), WORKL);
        display(currItem + 100 * itemproj, 0x02, 0, QString("%1V").arg(vcap), WORKL);
        display(currItem + 100 * itemproj, 0x03, 0, ccws.at(itemturn), WORKL);
        if (ext != 0) {
            isok = (cmax > 0 && (curr >= cmax || curr < cmin)) ? DATANG : isok;
            display(currItem + 100 * itemproj, 0x00, 1, (curr >= cmax || curr < cmin) ? "NG" : "OK", WORKL);
            isok = (pmax > 0 && (pwrs >= pmax || pwrs < pmin)) ? DATANG : isok;
            display(currItem + 100 * itemproj, 0x01, 1, (pwrs >= pmax || pwrs < pmin) ? "NG" : "OK", WORKL);
            isok = (vmax >0 && (vcap >= vmax || vcap < vmin)) ? DATANG : isok;
            display(currItem + 100 * itemproj, 0x02, 1, (vcap >= vmax || vcap < vmin) ? "NG" : "OK", WORKL);
            isok = (turn != 0 && itemturn != turn) ? DATANG : isok;
            display(currItem + 100 * itemproj, 0x03, 1, (itemturn != turn) ? "NG" : "OK", WORKL);
            tmpSave.insert(real + 0x00*3 + 2, curr);
            tmpSave.insert(real + 0x00*3 + 3, (curr >= cmax || curr < cmin) ? "NG" : "OK");
            tmpSave.insert(real + 0x01*3 + 2, pwrs);
            tmpSave.insert(real + 0x01*3 + 3, (pwrs >= pmax || pwrs < pmin) ? "NG" : "OK");
            tmpSave.insert(real + 0x02*3 + 2, vcap);
            tmpSave.insert(real + 0x02*3 + 3, (vcap >= vmax || vcap < vmin) ? "NG" : "OK");
            tmpSave.insert(real + 0x03*3 + 2, itemturn);
            tmpSave.insert(real + 0x03*3 + 3, (itemturn != turn) ? "NG" : "OK");
        }
    }
}

void AppWindow::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void AppWindow::calcHALL(QString msg)
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int test = tmpSet.value(back + backTest).toInt();
    int hall = tmpSet.value(4000 + Qt::Key_B).toInt();  // 霍尔配置地址
    int addr = tmpSet.value(3000 + Qt::Key_B).toInt();  // 霍尔结果地址
    QStringList tmp2 = msg.split(" ");
    if (tmp2.size() >= 190) {  // 霍尔计算
        QList<double> hhh, lll, fff, ddd;
        int back = tmpSet.value(1000 + Qt::Key_0).toInt() + 0x80;  // K,b值存储位置
        double kh = tmpSet.value(back + 20).toDouble();
        double bh = tmpSet.value(back + 21).toDouble();
        double kl = tmpSet.value(back + 22).toDouble();
        double bl = tmpSet.value(back + 23).toDouble();
        double kd = tmpSet.value(back + 24).toDouble();
        double bd = tmpSet.value(back + 25).toDouble();
        double kf = tmpSet.value(back + 26).toDouble();
        double bf = tmpSet.value(back + 27).toDouble();
        kh = (kh == 0) ? 1 : kh;
        kl = (kl == 0) ? 1 : kl;
        kd = (kd == 0) ? 1 : kd;
        kf = (kf == 0) ? 1 : kf;
        for (int i=0; i < 3; i++) {
            int tst = tmpSet.value(hall + CACHEHAL + i).toInt();  // 是否显示
            if (tst != 0) {
                double s1 = ((test&0x02) ? (15.28/4095) : (1.0/1000));
                double s2 = ((test&0x02) ? 1.0/1000 : 1);
                qDebug() << s1 << s2;
                hhh << qMax(0.0, (tmp2.at(100 + i*20).toDouble() * s1) * kh - bh);
                lll << qMax(0.0, (tmp2.at(101 + i*20).toDouble()/1000) * kl - bl);
                ddd << qMax(0.0, (tmp2.at(21 + i*4).toDouble() * s2) * kd - bd);
                fff << qMax(0.0, (tmp2.at(20 + i*4).toDouble() * s2) * kf - bf);
            }
        }
        QStringList units;
        units << "V" << "V" << "%" << "Hz";
        for (int item=0; item < 4; item++) {
            double vmax = tmpSet.value(hall + item*2 + 0x00).toDouble();
            double vmin = tmpSet.value(hall + item*2 + 0x01).toDouble();
            int real = DATAOK;
            QStringList reals;
            if (vmax != 0) {
                QList<double> tmp = hhh;
                tmp = (item == 1) ? lll : tmp;
                tmp = (item == 2) ? ddd : tmp;
                tmp = (item == 3) ? fff : tmp;
                for (int numb = 0; numb < tmp.size(); numb++) {
                    reals << QString::number(tmp.at(numb), 'f', 2) + units.at(item);
                    int r = (tmp.at(numb) >= vmax || tmp.at(numb) < vmin) ? DATANG : DATAOK;
                    real = (r == DATANG) ? DATANG : real;
                    tmpSave.insert(addr + item*0x10 + numb*3 + 1, tmp.at(numb));
                    tmpSave.insert(addr + item*0x10 + numb*3 + 2, (r == DATAOK) ? "OK" : "NG");
                }
                isok = (real == DATAOK) ? isok : DATANG;
                tmpMsg.insert(Qt::Key_0, Qt::Key_News);
                tmpMsg.insert(Qt::Key_1, currItem);
                tmpMsg.insert(Qt::Key_2, item);
                tmpMsg.insert(Qt::Key_3, reals.join(","));
                tmpMsg.insert(Qt::Key_4, (real == DATAOK) ? "OK" : "NG");
                tmpMsg.insert(Qt::Key_6, station);
                emit sendAppMsg(tmpMsg);
                tmpMsg.clear();
                int itemisok = tmpItem.value(nSetHAL);
                tmpItem.insert(nSetHAL, (real == DATAOK) ? itemisok : real);
            }
        }
    }
}
// 空载/负载结果解析 前20个数据舍弃
// 20:电流;21:电压;22功率;23:转速;24:Icc电流;25:Isp;26:转向
void AppWindow::calcLOAD(QString msg, int ext)
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int test = tmpSet.value(back + backTest).toInt();
    strLoad = msg;
    int item = Qt::Key_C;
    item = (currItem == 0x0D) ? Qt::Key_D : item;
    item = (currItem == 0x0F) ? Qt::Key_F : item;
    int addr = tmpSet.value(3000 + item).toInt();  // 负载结果地址
    int hall = tmpSet.value(4000 + Qt::Key_B).toInt();  // 霍尔配置地址
    int load = tmpSet.value(4000 + item).toInt();  // 负载配置地址
    int numb = tmpSet.value(hall + 8).toInt();  // 磁极数
    int from = tmpSet.value(load + 9).toInt();  // 转速读取位置
    QStringList tmp2 = msg.split(" ");
    if (tmp2.size() >= 30) {  // 负载计算
        int back = tmpSet.value(1000 + Qt::Key_0).toInt() + 0x80;  // K,b值存储位置
        double kc = tmpSet.value(back + 0x00).toDouble();
        double bc = tmpSet.value(back + 0x01).toDouble();
        double kv = tmpSet.value(back + 0x02).toDouble();
        double bv = tmpSet.value(back + 0x03).toDouble();
        double kp = tmpSet.value(back + 0x04).toDouble();
        double bp = tmpSet.value(back + 0x05).toDouble();
        double ki = tmpSet.value(back + 0x06).toDouble();
        double bi = tmpSet.value(back + 0x07).toDouble();
        double kl = tmpSet.value(back + 0x08).toDouble();
        double bl = tmpSet.value(back + 0x09).toDouble();
        kc = (kc == 0) ? 1 : kc;
        kv = (kv == 0) ? 1 : kv;
        kp = (kp == 0) ? 1 : kp;
        ki = (ki == 0) ? 1 : ki;
        kl = (kl == 0) ? 1 : kl;
        double s1 = ((test&0x02) ? 1000 : 10);
        double s2 = ((test&0x02) ? 500 : 1);
        double s3 = ((test&0x02) ? 1000 : 1);
        double s4 = ((test&0x02) ? 1 : 100);
        double crr = (tmp2.at(20 + 0x00).toDouble() * s1) * kc + bc;  // 电流换算为mA
        double vlt = (tmp2.at(20 + 0x01).toDouble() * s2) * kv + bv;  // 电压换算为V
        double pwr = (crr*vlt/1000) * kp + bp;  // 功率
        double icc = (tmp2.at(20 + 0x04).toDouble()/s4) * ki + bi;  // icc电流
        double rpm = tmp2.at(20 + 0x03).toDouble()*60/s3/numb;  // 转速
        double ccw = tmp2.at(20 + 0x06).toDouble();  // 转向
        leek = (leek * kl) + bl;
        if (!testparm.value("taskturn").isNull() && currItem == nSetNLD) {
            QStringList tt = testparm.value("taskturn").toString().split(" ");
            if (tt.size() >= 2) {
                ccw = QString(tt.at(station == WORKL ? 0 : 1)).toInt();
                ccw = (ccw == 0) ? turnBuff : ccw;
                if (turnBuff == 0 && ccw != 0) {  // 测试到转向
                    if (ioHexS != 0) {
                        sendUdpStr(tr("6036 %1").arg(ioHexS | 0x010000).toUtf8());
                    }
                }
                turnBuff = ccw;
            }
        }
        crr = qMax(0.0, crr);
        vlt = qMax(0.0, vlt);
        pwr = qMax(0.0, pwr);
        icc = qMax(0.0, icc);
        QString turn;
        if (from == 1) {
            double tmp = testparm.value("speedmax").toInt();
            rpm = fabs(tmp);
            ccw = (fabs(tmp) < 5) ? 0: ccw;
            ccw = (tmp < -5) ? 1 : ccw;
            ccw = (tmp > 5) ? 2 : ccw;
        }
        turn = (ccw == 0) ? "NULL" : turn;
        turn = (ccw == 1) ? "CCW" : turn;
        turn = (ccw == 2) ? "CW" : turn;

        QList<double> reals;
        taskparm.insert("taskrate", rpm);
        reals << crr << pwr << icc << rpm << ccw << leek;;
        for (int i=0; i < reals.size(); i++) {
            double imax = tmpSet.value(load + i * 2 + 0x00).toDouble();
            double imin = tmpSet.value(load + i * 2 + 0x01).toDouble();
            int rrrr = DATAOK;
            if (imax != 0) {
                tmpMsg.insert(Qt::Key_0, Qt::Key_News);
                tmpMsg.insert(Qt::Key_1, currItem);
                tmpMsg.insert(Qt::Key_2, i);
                QString volt = QString::number(vlt, 'f', 2) + "V ";
                QString real;
                real = (i == 0) ? (volt + QString::number(reals.at(i), 'f', 2) + "mA") : real;
                real = (i == 1) ? (QString::number(reals.at(i), 'f', 2) + "W") : real;
                real = (i == 2) ? (QString::number(reals.at(i), 'f', 2) + "mA") : real;
                real = (i == 3) ? (QString::number(reals.at(i), 'f', 0) + "rpm") : real;
                real = (i == 4) ? turn : real;
                real = (i == 5) ? (QString::number(reals.at(i), 'f', 0) + "") : real;
                if (ext != 0) {
                    rrrr = (reals.at(i) >= imax || reals.at(i) < imin) ? DATANG : DATAOK;
                    rrrr = (i == 4) ? ((reals.at(i) != imax) ? DATANG : DATAOK) : rrrr;
                    isok = (rrrr == DATAOK) ? isok : DATANG;
                    tmpMsg.insert(Qt::Key_4, (rrrr == DATAOK) ? "OK" : "NG");
                    tmpSave.insert(addr + i*0x10 + 2, (rrrr == DATAOK) ? "OK" : "NG");
                    int itemisok = tmpItem.value(currItem);
                    tmpItem.insert(currItem, (real == DATAOK) ? itemisok : rrrr);
                    buffrate.clear();
                } else {
                    tmpSave.insert(addr + i*0x10 + 1, reals.at(i));
                    tmpMsg.insert(Qt::Key_3, real);
                }
                tmpMsg.insert(Qt::Key_6, station);
                emit sendAppMsg(tmpMsg);
                tmpMsg.clear();
            }
        }
    }
    if (ext != 0)
        testCheckWarn();
}
// 空载/负载结果解析 前20个数据舍弃
// 20:电流;21:电压;22功率;23:转速;24:Icc电流;25:Isp;26:相序;27-28-29:UVW
void AppWindow::calcBEMF(QString msg, int ext)
{
    strLoad = msg;
    int addr = tmpSet.value(3000 + Qt::Key_E).toInt();  // 反电动势结果地址
    int bemf = tmpSet.value(4000 + Qt::Key_E).toInt();  // 反电动势配置地址
    QStringList tmp2 = msg.split(" ");
    if (tmp2.size() >= 30) {  // 反电动势计算
        int back = tmpSet.value(1000 + Qt::Key_0).toInt() + 0x80;  // K,b值存储位置
        double ku = tmpSet.value(back + 14).toDouble();
        double bu = tmpSet.value(back + 15).toDouble();
        double kv = tmpSet.value(back + 16).toDouble();
        double bv = tmpSet.value(back + 17).toDouble();
        double kw = tmpSet.value(back + 18).toDouble();
        double bw = tmpSet.value(back + 19).toDouble();
        ku = (ku == 0) ? 1 : ku;
        kv = (kv == 0) ? 1 : kv;
        kw = (kw == 0) ? 1 : kw;
        double stds = tmpSet.value(bemf + 0x10 + 0x00).toDouble();
        QList<double> volts;
        QList<double> bemfs;
        QList<double> diffs;
        QList<double> vmaxs;
        for (int t=0; t < 3; t++) {
            volts << qMax(0.0, (tmp2.at(20+7+t).toDouble() * 15.28 / 4095) * ku + bu);
            bemfs << qMax(0.0, (volts.at(t) * 1000 / stds) * kv + bv);
            diffs << qMax(0.0, (tmp2.at(t).toDouble()) * kw + bw);
            vmaxs << volts.at(t)*1.414*2;
        }
        for (int i=0; i < 6; i++) {
            int rrrr = DATAOK;
            QStringList vrs;
            double vmax = tmpSet.value(bemf + i * 2 + 0x00).toDouble();
            double vmin = tmpSet.value(bemf + i * 2 + 0x01).toDouble();
            if (vmax != 0) {
                if (i < 3 || i == 5) {
                    QList<double> tmp = volts;
                    tmp = (i == 1) ? bemfs : tmp;
                    tmp = (i == 2) ? diffs : tmp;
                    tmp = (i == 5) ? vmaxs : tmp;
                    for (int t=0; t < tmp.size(); t++) {
                        QString str = QString::number(tmp.at(t), 'f', 2) + "V";
                        str = (i == 1) ? (QString::number(tmp.at(t), 'f', 2) + "V/krpm") : str;
                        str = (i == 2) ? (QString::number(tmp.at(t), 'f', 1) + "°") : str;
                        vrs << str;
                        if (ext != 0) {
                            rrrr = (tmp.at(t) >= vmax || tmp.at(t) < vmin) ? DATANG : rrrr;
                            QString s = (rrrr == DATAOK) ? "OK" : "NG";
                            isok = (rrrr == DATAOK) ? isok : DATANG;
                            tmpMsg.insert(Qt::Key_4, s);
                            tmpSave.insert(addr + i*0x10 + t*3 + 1, tmp.at(t));
                            tmpSave.insert(addr + i*0x10 + t*3 + 2, s);
                            int itemisok = tmpItem.value(nSetEMF);
                            tmpItem.insert(nSetEMF, (rrrr == DATAOK) ? itemisok : rrrr);
                        }
                    }
                }
                if (i == 3 && volts.size() >= 3) {
                    double noun = calcNoun(volts);
                    vrs << QString::number(noun, 'f', 1) + "%";
                    if (ext != 0) {
                        rrrr = (noun >= vmax || noun < vmin) ? DATANG : rrrr;
                        isok = (rrrr == DATAOK) ? isok : DATANG;
                        tmpMsg.insert(Qt::Key_4, (rrrr == DATAOK) ? "OK" : "NG");
                        tmpSave.insert(addr + i*0x10 + 1, noun);
                        tmpSave.insert(addr + i*0x10 + 2, (rrrr == DATAOK) ? "OK" : "NG");
                        int itemisok = tmpItem.value(nSetEMF);
                        tmpItem.insert(nSetEMF, (rrrr == DATAOK) ? itemisok : rrrr);
                    }
                }
                if (i == 4) {
                    int turn = tmp2.at(20 + 6).toInt();
                    QString str = "NULL";
                    str = (turn == 1) ? "ABC" : str;
                    str = (turn == 2) ? "ACB" : str;
                    if (ext != 0) {
                        rrrr = (turn != vmax) ? DATANG : rrrr;
                        isok = (rrrr == DATAOK) ? isok : DATANG;
                        tmpMsg.insert(Qt::Key_4, (rrrr == DATAOK) ? "OK" : "NG");
                        tmpSave.insert(addr + i*0x10 + 1, str);
                        tmpSave.insert(addr + i*0x10 + 2, (rrrr == DATAOK) ? "OK" : "NG");
                        int itemisok = tmpItem.value(nSetEMF);
                        tmpItem.insert(nSetEMF, (rrrr == DATAOK) ? itemisok : rrrr);
                    }
                }
            }
            tmpMsg.insert(Qt::Key_0, Qt::Key_News);
            tmpMsg.insert(Qt::Key_1, currItem);
            tmpMsg.insert(Qt::Key_6, station);
            tmpMsg.insert(Qt::Key_2, i);
            tmpMsg.insert(Qt::Key_3, vrs.join("  "));
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
    }
}

double AppWindow::calcNoun(QList<double> volts)
{
    double max = volts.at(0);
    double min = volts.at(0);
    double avr = 0;
    double tmp = 0;
    for (int t=0; t < volts.size(); t++) {
        avr += volts.at(t);
        max = qMax(max, volts.at(t));
        min = qMin(min, volts.at(t));
    }
    avr /= volts.size();
    if (avr != 0)
        tmp = (max - min) * 100 / avr;
    return tmp;
}
