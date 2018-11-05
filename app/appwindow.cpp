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
QMap<int, pClass> initMap;
QMap<int, pClass> taskMap;
QMap<int, pClass> testMap;

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    t.start();
    initUI();
    initDevice();
}

AppWindow::~AppWindow()
{
    sql->quit();
    sql->wait();
}

int AppWindow::initUI()
{
    initTitle();
    initLayout();
    initSqlDir();
    initAuthor();
    QTimer::singleShot(100, this, SLOT(initScreen()));
    return Qt::Key_Away;
}

int AppWindow::initTitle()
{
#ifndef __linux__
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif

    char s_month[5];
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    int month, day, year;

    sscanf((__DATE__), "%s %d %d", s_month, &day, &year);
    month = (strstr(month_names, s_month)-month_names)/3+1;

    QDate dt;
    dt.setDate(year, month, day);
    static const QTime tt = QTime::fromString(__TIME__, "hh:mm:ss");

    QDateTime t(dt, tt);
    verNumb = QString("V-2.3.%1").arg(t.toString("yyMMdd-hhmm"));
    QString ttt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qWarning() << "app vern:" << verNumb << ttt;
    this->setWindowTitle(tr("电机综合测试仪%1").arg(verNumb));
    return Qt::Key_Away;
}

int AppWindow::initLayout()
{
    isudp = 0;
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
    this->resize(1440, 900);
    return Qt::Key_Away;
}

int AppWindow::initAuthor()
{
    AppAuthor *app = new AppAuthor(this);
    initWidget(nAuthor, 0, "author", tr("返回主页"), app);
    return Qt::Key_Away;
}

int AppWindow::initDevice()
{
    scanner = new QTimer(this);
    connect(scanner, SIGNAL(timeout()), this, SLOT(showBarCode()));
    return Qt::Key_Away;
}

int AppWindow::initScreen()
{
    isChange = false;
    boxbar = new BoxDialog(this);
    boxbar->setValue(0);
    boxbar->show();

    QStringList names;
    initMap[names.size()] = &AppWindow::initTester;
    names << tr("正在初始化测试界面");
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
    initMap[names.size()] = &AppWindow::initLogger;
    names << tr("正在初始化调试信息");
    initMap[names.size()] = &AppWindow::initConfig;
    names << tr("正在初始化型号管理");
    initMap[names.size()] = &AppWindow::initSetDcr;
    names << tr("正在初始化电阻配置");
    initMap[names.size()] = &AppWindow::initSetMag;
    names << tr("正在初始化反嵌配置");
    initMap[names.size()] = &AppWindow::initSetInr;
    names << tr("正在初始化绝缘配置");
    initMap[names.size()] = &AppWindow::initSetAcw;
    names << tr("正在初始化高压配置");
    initMap[names.size()] = &AppWindow::initSetImp;
    names << tr("正在初始化匝间配置");
    initMap[names.size()] = &AppWindow::initSetHal;
    names << tr("正在初始化霍尔配置");
    initMap[names.size()] = &AppWindow::initSetLod;
    names << tr("正在初始化负载配置");
    initMap[names.size()] = &AppWindow::initSetBmf;
    names << tr("正在初始化BEMF配置");
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

    for (int i=0; i < names.size(); i++)
        showBoxPop(names.at(i), i);

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
    initWidget(nTester, 0, "tester", tr("测试界面"), app);
    return Qt::Key_Away;
}

int AppWindow::initSignin()
{
    AppSignin *app = new AppSignin(this);
    initWidget(nSignin, 1, "signin", tr("用户登录"), app);
    return Qt::Key_Away;
}

int AppWindow::initSystem()
{
    AppSystem *app = new AppSystem(this);
    initWidget(nSystem, 1, "system", tr("系统设置"), app);
    return Qt::Key_Away;
}

int AppWindow::initIoCtrl()
{
    AppIoCtrl *app = new AppIoCtrl(this);
    initWidget(nIoctrl, 1, "ioctrl", tr("动作设置"), app);
    return Qt::Key_Away;
}

int AppWindow::initMaster()
{
    AppMaster *app = new AppMaster(this);
    initWidget(nMaster, 1, "master", tr("用户管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initPermit()
{
    AppPermit *app = new AppPermit(this);
    initWidget(nPermit, 1, "permit", tr("权限管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initBackup()
{
    AppBackup *app = new AppBackup(this);
    initWidget(nBackup, 1, "backup", tr("后台管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initLogger()
{
    AppLogger *app = AppLogger::instance();
    initWidget(nLogger, 1, "logger", tr("调试信息"), app);
    return Qt::Key_Away;
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
    initWidget(nSetDCR, 2, "setdcr", tr("电阻配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetMag()
{
    TypSetMag *app = new TypSetMag(this);
    initWidget(nSetMAG, 2, "setmag", tr("反嵌配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetInr()
{
    TypSetInr *app = new TypSetInr(this);
    initWidget(nSetINR, 2, "setinr", tr("绝缘配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetAcw()
{
    TypSetAcw *app = new TypSetAcw(this);
    initWidget(nSetACW, 2, "setacw", tr("耐压配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetImp()
{
    TypSetImp *app = new TypSetImp(this);
    initWidget(nSetIMP, 2, "setimp", tr("匝间配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetHal()
{
    TypSetHal *app = new TypSetHal(this);
    initWidget(nSetHAL, 2, "sethal", tr("霍尔配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetLod()
{
    TypSetLod *app = new TypSetLod(this);
    initWidget(nSetLOD, 2, "setlod", tr("负载配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetBmf()
{
    TypSetBmf *app = new TypSetBmf(this);
    initWidget(nSetEMF, 2, "setbmf", tr("BEMF配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initImport()
{
    sql = new QThread(this);
    sql->start();
    SqlImport *app = new SqlImport;
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
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
    initWidget(nRecord, 3, "record", tr("数据管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initUpload()
{
    SqlUpload *app = new SqlUpload(this);
    initWidget(nUpload, 3, "upload", tr("数据上传"), app);
    return Qt::Key_Away;
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
    tmpSet.insert(DataSoft, verNumb);
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
    timeUdp = 0;
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
    UdpSocket *udp = new UdpSocket;
    connect(udp, SIGNAL(sendUdpMsg(QByteArray)), this, SLOT(recvUdpMsg(QByteArray)));
    connect(this, SIGNAL(sendUdpMsg(QTmpMap)), udp, SLOT(recvAppMsg(QTmpMap)));
    udp->initSocket();

    return Qt::Key_Away;
}

int AppWindow::initSerial()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    if (!tmpSet.value(back + 0x20 + 0x04).toString().isEmpty()) {  // 伺服左
        QString com = tmpSet.value(back + 0x20 + 0x04).toString();
        if (servoL.open(com) == Qt::Key_Meta)
            warnningString(tr("串口%1打开失败").arg(com));
        connect(&servoL, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    }
    if (!tmpSet.value(back + 0x20 + 0x05).toString().isEmpty()) {  // 伺服右
        QString com = tmpSet.value(back + 0x20 + 0x05).toString();
        if (servoR.open(com) == Qt::Key_Meta)
            warnningString(tr("串口%1打开失败").arg(com));
        connect(&servoR, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    }
    if (!tmpSet.value(back + 0x20 + 0x06).toString().isEmpty()) {  // PLC左
        QString com = tmpSet.value(back + 0x20 + 0x06).toString();
        if (mbdktL.open(com) == Qt::Key_Meta)
            warnningString(tr("串口%1打开失败").arg(com));
        connect(this, SIGNAL(sendAppMsg(QTmpMap)), &mbdktL, SLOT(recvAppMsg(QTmpMap)));
    }
    if (!tmpSet.value(back + 0x20 + 0x07).toString().isEmpty()) {  // PLC右
        QString com = tmpSet.value(back + 0x20 + 0x07).toString();
        if (mbdktR.open(com) == Qt::Key_Meta)
            warnningString(tr("串口%1打开失败").arg(com));
        connect(this, SIGNAL(sendAppMsg(QTmpMap)), &mbdktR, SLOT(recvAppMsg(QTmpMap)));
    }
    if (!tmpSet.value(back + 0x20 + 0x09).toString().isEmpty()) {  // PLC右
        QString com = tmpSet.value(back + 0x20 + 0x09).toString();
        if (invert.open(com) == Qt::Key_Meta)
            warnningString(tr("串口%1打开失败").arg(com));
    }
    return Qt::Key_Away;
}

int AppWindow::initThread()
{
    QList<pClass> taskBuf;
    taskBuf << &AppWindow::taskClearData << &AppWindow::taskCheckPlay
            << &AppWindow::taskCheckCode << &AppWindow::taskStartView
            << &AppWindow::taskToolIobrd << &AppWindow::taskStartWait
            << &AppWindow::taskStartTest << &AppWindow::taskClearCtrl
            << &AppWindow::taskStartSave << &AppWindow::taskStartBeep
            << &AppWindow::taskClearBeep << &AppWindow::taskResetTest;
    taskMap.clear();
    for (int i=0; i < taskBuf.size(); i++) {
        taskMap.insert(i, taskBuf.at(i));
    }

    QList<pClass> testBuf;
    testBuf << &AppWindow::testClearData << &AppWindow::testToolServo
            << &AppWindow::testStartSend << &AppWindow::testStartTest
            << &AppWindow::testStopServo;

    testMap.clear();
    for (int i=0; i < testBuf.size(); i++) {
        testMap.insert(i, testBuf.at(i));
    }

    QStringList items;
    items << "Conf" << "DCR" << "MAG" << "IR" << "ACW" << "DCW" << "IMP"
          << "PWR" << "IND" << "LCK" << "LVS" << "HALL" << "LOAD"
          << "NOLOAD" << "BEMF" << "LPH";
    for (int i=0; i < items.size(); i++) {
        tstMap.insert(i, items.at(i));
    }

    QTimer *tasks = new QTimer(this);
    connect(tasks, SIGNAL(timeout()), this, SLOT(taskThread()));
    tasks->start(10);

    ioHex = 0;
    currTask = 0;
    currTest = 0;
    station = 0x13;

    loading = new QTimer(this);
    connect(loading, SIGNAL(timeout()), this, SLOT(testWaitServo()));

    return Qt::Key_Away;
}

int AppWindow::initButton(QString title, QString name)
{
    QPushButton *btnTester = new QPushButton(title, this);
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

int AppWindow::initWidget(int numb, int form, QString name, QString mark, QWidget *app)
{
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

int AppWindow::taskThread()
{
    taskLoopSignin();

    tmpMsg.insert(Qt::Key_0, Qt::Key_Time);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    int ret = Qt::Key_Meta;
    if (taskMap.keys().contains(currTask))
        ret = (this->*taskMap[currTask])();
    if (ret == Qt::Key_Away) {
        currTask++;
        currTask = (currTask >= taskMap.size()) ? 0 : currTask;
    }
    return ret;
}

int AppWindow::taskLoopSignin()
{  // 每2.5秒发一次登录或心跳，无回应报警
    timeUdp++;
    if (timeUdp%(TIME_OUT/4) == 0) {
        if (isudp == 0)
            sendUdpStr("6000");
        else
            sendUdpStr("6030");
        tmpMsg.insert(Qt::Key_0, Qt::Key_WLAN);
        tmpMsg.insert(Qt::Key_1, isudp);
        tmpMsg.insert(Qt::Key_2, timeUdp * 100 / TIME_OUT);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
    if (timeUdp == TIME_OUT) {
        timeUdp = 0;
        isudp = 0;
    }

    return Qt::Key_Away;
}

int AppWindow::taskClearData()
{
    isok = DATAOK;
    timeOut = 0;
    timeTst = 0;
    currTask = 0;
    currTest = 0;
    currItem = 0;
    speed = 0;
    speeds.clear();
    barcode.clear();
    codeShift = Qt::Key_Meta;
    taskShift = Qt::Key_Meta;
    testShift = Qt::Key_Meta;
    tempShift = Qt::Key_Meta;
    return Qt::Key_Away;
}

int AppWindow::taskCheckPlay()
{
    int ret = Qt::Key_Meta;
    if (taskShift == Qt::Key_Play) {
        if (stack->currentWidget()->objectName() != "tester") {
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
        }
    }
    return ret;
}

int AppWindow::taskCheckCode()
{
    int ret = Qt::Key_Meta;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    if (mode == 3) {  // 产线模式,控石产线专用
        if (timeOut%100 == 0) {
            sendUdpStr("6075");  // 读取RFID
        }
        if (codeShift == Qt::Key_Away) {
            timeOut = 0;
            ret = Qt::Key_Away;
        } else {
            timeOut++;
            if (timeOut > 300) { // 重复3次后未检到RFID,显示警告,下发停止
                timeOut = 0;
                warnningString(tr("警告:RFID未检到"));
                currTask = taskMap.values().indexOf(&AppWindow::taskStartTest) + 1;
                sendUdpStr(tr("6022 %1").arg(station).toUtf8());
                taskShift = Qt::Key_Stop;
            }
        }
    } else {
        ret = Qt::Key_Away;
    }
    return ret;
}

int AppWindow::taskStartView()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    if (mode == 1) {  // 真空模式
        sendUdpStr(QString("6020 %1").arg(station).toUtf8());
    } else {
        sendUdpStr(QString("6066 %1").arg(station).toUtf8());
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
    tmpMsg.insert(Qt::Key_2, DATAON);
    tmpMsg.insert(Qt::Key_1, "LEDY");
    tmpMsg.insert(Qt::Key_4, station);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    currItem = getNextItem();
    timeOut = t.elapsed();
    tmpSet[tmpSet.value((3000 + Qt::Key_0)).toInt() + TEMPISOK] = DATAOK;
    qDebug() << "app view:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskToolIobrd()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
    QStringList testItems = tmpSet.value(conf + ADDRITEM).toString().split(",");
    if (mode == 2 && !testItems.contains(QString::number(0x0D))) { // 无刷模式
        int back = tmpSet.value(1000 + Qt::Key_0).toInt() + 0x40;
        int down = back + ((station == 0x13) ? 0x00 : 0x03);
        int move = back + ((station == 0x13) ? 0x0C : 0x0F);
        down = tmpSet.value(down).toString().toInt(NULL, 16);
        move = tmpSet.value(move).toString().toInt(NULL, 16);
        sendUdpStr(tr("6036 %1").arg(down + move).toUtf8());
    }
    return Qt::Key_Away;
}

int AppWindow::taskStartWait()
{
    int ret = Qt::Key_Meta;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int syst = tmpSet.value((2000 + Qt::Key_1)).toInt();  // 系统设置地址
    int wait = tmpSet.value(syst + SystWait).toDouble() * 1000;
    if (t.elapsed() - timeOut > wait) {
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
{
    int ret = Qt::Key_Meta;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    if (mode == 3) {
        if (timeTst % 100 == 0) {
            sendUdpStr(tr("6077 %1").arg(YY14 | YY08).toUtf8());
        }
        timeTst++;
        if (timeTst > 500) {
            timeTst = 0;
            warnningString(tr("警告:轴移除超时!"));
        }
        if ((ioHex & XX23))
            ret = Qt::Key_Away;
    } else {
        ret = Qt::Key_Away;
    }
    return ret;
}

int AppWindow::taskStartSave()
{
    if (1) {
        int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
        int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
        if (mode == 3) {  // 产线模式,控石产线专用
            quint32 cmd = 0;
            cmd |= (isok) ? YY0E : YY10;  // 总合格/总不合格
            cmd |= barcode.isEmpty() ? YY09 : 0x00;  // RFID未读到
            sendUdpStr(tr("6077 %1").arg(cmd).toUtf8());
        }
        if (mode >= 2)  // 无刷模式
            sendUdpStr(tr("6080 %1").arg((isok == DATAOK) ? "1" : "0").toUtf8());  // 下发总测试结果
    }
    if (taskShift == Qt::Key_Stop)
        return Qt::Key_Away;
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    int user = tmpSet.value(DataUser).toInt();
    QString temp = tmpSet.value(addr + TEMPTEMP).toString();
    temp = temp.remove(tr("温度:"));
    temp = temp.remove("°C");
    tmpMsg = tmpSave;
    qDebug() << tmpSave;
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);  // 存储数据
    tmpMsg.insert(Qt::Key_1, "aip_record");
    tmpMsg.insert(addr + TEMPDATE, QDate::currentDate().toString("yyyy-MM-dd"));  // 日期
    tmpMsg.insert(addr + TEMPPLAY, tmpSet.value(addr + TEMPPLAY));  // 测试开始时间
    tmpMsg.insert(addr + TEMPSTOP, QTime::currentTime());  // 测试结束时间
    tmpMsg.insert(addr + TEMPTYPE, tmpSet.value(DataType).toString());  // 型号
    tmpMsg.insert(addr + TEMPCODE, barcode);  // 条码
    tmpMsg.insert(addr + TEMPUSER, tmpSet.value(user).toString());  // 用户
    tmpMsg.insert(addr + TEMPWORK, (station == 0x13) ? "L" : "R");  // 工位
    tmpMsg.insert(addr + TEMPTEMP, temp);  // 温度
    tmpMsg.insert(addr + TEMPISOK, (isok == DATAOK) ? "OK" : "NG");  // 结果
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    qDebug() <<"app save:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskStartBeep()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int syst = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置地址
    int beep = tmpSet.value(syst + SystBeep).toInt() * 10 + 9;  // 报警时间
    if (mode == 2) { // 无刷模式
        int back = tmpSet.value(1000 + Qt::Key_0).toInt() + 0x40;
        int down = back + ((station == 0x13) ? 0x12 : 0x13);
        down = tmpSet.value(down).toString().toInt(NULL, 16);
        sendUdpStr(tr("6036 %1").arg(down).toUtf8());
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
    tmpMsg.insert(Qt::Key_1, isok == DATAOK ? "LEDG" : "LEDR");
    tmpMsg.insert(Qt::Key_2, (taskShift == Qt::Key_Stop) ? DATADC : isok);
    tmpMsg.insert(Qt::Key_3, beep);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    timeOut = t.elapsed();
    qDebug() <<"app beep:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskClearBeep()
{
    int ret = Qt::Key_Meta;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int syst = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置地址
    int real = tmpSet.value(3000 + Qt::Key_0).toInt();  // 零散参数地址
    int isok = tmpSet.value(real + TEMPISOK).toInt();
    int tt = tmpSet.value(syst + (isok == DATAOK ? SystTime : SystWarn)).toDouble()*1000;
    if (t.elapsed() - timeOut >= tt) {
        if (mode == 2) { // 无刷模式
            sendUdpStr("6036 0");
        }
        timeOut = t.elapsed();
        ret = Qt::Key_Away;
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_3, 0);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        qDebug() <<"app wait:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0')) << tt;
    }
    return ret;
}

int AppWindow::taskResetTest()
{
    if (taskShift == Qt::Key_Stop)
        return Qt::Key_Away;
    int addr = tmpSet.value(1000 + Qt::Key_0).toInt();
    int test = tmpSet.value(addr + backAuto).toInt();
    int time = tmpSet.value(addr + backWait).toInt();
    if (test >= 1) {
        if (t.elapsed() - timeOut >= time*1000) {
            for (int i=0; i < taskMap.size(); i++) {
                if (taskMap.value(i) == &AppWindow::taskCheckPlay) {
                    taskClearData();
                    currTask = i;
                    taskShift = Qt::Key_Play;
                    station = (test == 1) ? WORKL : station;
                    station = (test == 2) ? WORKR : station;
                    station = (test == 3) ? ((station == WORKL) ? WORKR : WORKL) : station;
                    t.restart();
                    break;
                }
            }
        }
    }
    return (test >= 1) ? Qt::Key_Meta : Qt::Key_Away;
}

int AppWindow::taskCheckStop()
{
    int addr = tmpSet.value(3000 + Qt::Key_0).toInt();  // 综合测试结果
    int play = taskMap.values().indexOf(&AppWindow::taskCheckPlay);
    int save = taskMap.values().indexOf(&AppWindow::taskStartSave);
    if (currTask < save)
        sendUdpStr(tr("6022 %1").arg(station).toUtf8());
    if (currTask < save && currTask > play) {
        taskShift = Qt::Key_Stop;
        isok = DATANG;
        currTask = taskMap.values().indexOf(&AppWindow::taskStartSave);
        tmpSet.insert(addr + TEMPISOK, DATANG);
    }
    return Qt::Key_Away;
}

int AppWindow::testThread()
{
    int ret = Qt::Key_Meta;
    if (testMap.keys().contains(currTest)) {
        if ((this->*testMap[currTest])() == Qt::Key_Away) {
            currTest++;
            currTest = (currTest >= testMap.size()) ? 0 : currTest;  // 测试流程
            if (currTest == 0) {
                currItem = getNextItem();
                if (currItem == 0)
                    ret = Qt::Key_Away;
            }
        }
    }
    return ret;
}

int AppWindow::testClearData()
{
    testShift = Qt::Key_Meta;
    timeTst = 0;
    return Qt::Key_Away;
}

int AppWindow::testToolServo()
{
    int ret = Qt::Key_Away;
    if (currItem == 0x0C) { // 负载
        int addr = tmpSet.value(4000 + Qt::Key_C).toInt();  // 负载配置地址
        double from = tmpSet.value(addr + 0x09).toDouble();
        tmpMap.insert("mode", 1);  // 转矩模式
        tmpMap.insert("turn", 0);
        tmpMap.insert("data", 0);
        tmpMap.insert("from", from);
        if (station == WORKL) {
            if (from != 0)
                servoL.test();
            ret = mbdktL.test(tmpMap);
        } else {
            if (from != 0)
                servoR.test();
            ret = mbdktR.test(tmpMap);
        }
        tmpMap.clear();
        if (ret == Qt::Key_Away) {
            int time = tmpSet.value(addr + CACHELOD*0x02).toDouble() * 1000;
            loading->start(time);
        }
    }
    if (currItem == 0x0E) {  // 反电动势
        int addr = tmpSet.value(4000 + Qt::Key_E).toInt();  // 反电动势配置地址
        int temp = tmpSet.value(addr + 0x10).toDouble();
        int turn = tmpSet.value(addr + 0x11).toDouble();
        int time = tmpSet.value(addr + 0x13).toDouble()*100;
        if (timeTst == 0) {
            tmpMap.insert("mode", 0);  // 转速模式
            tmpMap.insert("turn", turn);
            tmpMap.insert("data", temp);
            if (station == 0x13)
                ret = mbdktL.test(tmpMap);
            else
                ret = mbdktR.test(tmpMap);
            tmpMap.clear();
            if (ret == Qt::Key_Away)
                timeTst++;
        }
        if (timeTst != 0) {
            timeTst++;
            ret = (timeTst >= time/2) ? Qt::Key_Away : Qt::Key_Meta;
        }
    }
    return ret;
}

int AppWindow::testWaitServo()
{
    int ret = Qt::Key_Away;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    int addr = tmpSet.value(4000 + Qt::Key_C).toInt();  // 负载配置地址
    double from = tmpSet.value(addr + 0x09).toDouble();
    double temp = tmpSet.value(addr + 0x13).toDouble();
    double comp = tmpSet.value(addr + 0x23).toDouble();
    temp = qMax(temp-comp, 0.0);
    tmpMap.insert("mode", 1);  // 转矩模式
    tmpMap.insert("turn", 0);
    tmpMap.insert("data", (mode == 3) ? temp*1368 : temp*2500);
    tmpMap.insert("from", from);
    if (station == WORKL) {
        if (from != 0)
            servoL.test();
        ret = mbdktL.test(tmpMap);
    } else {
        if (from != 0)
            servoR.test();
        ret = mbdktR.test(tmpMap);
    }
    tmpMap.clear();
    if (ret == Qt::Key_Away)
        loading->stop();
    else
        loading->start(10);
    return ret;
}

int AppWindow::testStopServo()
{
    int ret = Qt::Key_Away;
    if (currItem == 0x0C) { // 负载
        if (station == 0x13) {
            ret = mbdktL.test(tmpMap);
        } else {
            ret = mbdktR.test(tmpMap);
        }
        tmpMap.clear();
    }
    if (currItem == 0x0E) {  // 反电动势
        if (station == 0x13)
            ret = mbdktL.test(tmpMap);
        else
            ret = mbdktR.test(tmpMap);
        tmpMap.clear();
    }
    return ret;
}

int AppWindow::testToolInvrt()
{
    int ret = Qt::Key_Meta;
    if (currItem == 0x0C) { // 负载
        int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 型号设置地址
        int mode = tmpSet.value(conf + ADDRAUTO).toInt();  // 内外置驱动设置
        if (mode == 1) {  // 外驱
            int addr = tmpSet.value(4000 + Qt::Key_C).toInt();  // 负载配置地址
            addr += 0x10;
            double tt = tmpSet.value(addr + 0x10 + 0x04).toDouble() * 100; // 测试时间
            int freq = tmpSet.value(addr + 0x10 + 0x05).toInt();  // 外驱频率
            int turn = tmpSet.value(addr + 0x20 + 0x05).toInt() - 1; // 外驱转向
            tmpMap.insert("freq", freq);
            tmpMap.insert("turn", turn);
            if (timeTst % 300 == 0) {
                sendUdpStr(QString("6083 %1 %2").arg(station).arg(1).toUtf8());
                invert.test(tmpMap);
                tmpMap.clear();
            }
            timeTst++;
            if (timeTst > tt / 2) {
                timeTst = 0;
                ret = Qt::Key_Away;
            }
        }
    } else {
        ret = Qt::Key_Away;
    }
    return ret;
}

int AppWindow::testWaitInvrt()
{
    int ret = Qt::Key_Away;
    if (currItem == 0x0C) { // 负载
        int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 型号设置地址
        int mode = tmpSet.value(conf + ADDRAUTO).toInt();  // 内外置驱动设置
        if (mode == 1) {  // 外驱
            tmpMap.insert("freq", 0);
            tmpMap.insert("turn", 0);
            invert.test(tmpMap);
            tmpMap.clear();
        }
    }
    return ret;
}

int AppWindow::testStartSend()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Play);
    tmpMsg.insert(Qt::Key_1, currItem);
    tmpMsg.insert(Qt::Key_4, station);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    sendUdpStr(QString("6006 %1").arg(tstMap.value(currItem).toString()).toUtf8());

    return Qt::Key_Away;
}

int AppWindow::testStartTest()
{
    int ret = Qt::Key_Meta;
    if (testShift == Qt::Key_Away) {
        ret = Qt::Key_Away;
    }
    return ret;
}

int AppWindow::getNextItem()
{
    timeDet = 0;
    timeRsl = 0;
    timeMag = 0;
    timeImp = 0;
    int r = tmpSet.value(4000 + Qt::Key_0).toInt();
    QStringList testItems = tmpSet.value(r + ADDRITEM).toString().split(",");
    qDebug() << "app next:" << testItems;

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
          << "setimp" << "setpwr" << "setind" << "setlvs" << "setlck" << "sethal"
          << "setlod" << "setnld" << "setbmf" << "setlvs";

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
    if (stack->currentWidget()->objectName() == "author")
        QTimer::singleShot(1000, this, SLOT(showTester()));
    if (stack->currentWidget()->objectName() == "tester")
        QTimer::singleShot(1000, this, SLOT(showTester()));
    return Qt::Key_Away;
}

int AppWindow::recvIoCtrl(int key, int work)
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    if (mode == 1) {  // 真空模式
        QString str;
        bool sl = (key == Qt::Key_Play && work == 0x13) || ((ioHex & X03) && ((ioHex & X11) == 0));
        bool sr = (key == Qt::Key_Play && work == 0x14) || ((ioHex & X00) && ((ioHex & X08) == 0));
        if (sl && (ioHex & X05)) {
            str = tr("警告:左光幕遮挡!");
        }
        if (sr && (ioHex & X02)) {
            str = tr("警告:右光幕遮挡!");
        }
        if (!str.isEmpty()) {
            warnningString(str);
            return Qt::Key_Meta;
        }
    }
    return Qt::Key_Away;
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
    tmpcode = tmpcode.mid(pCode, pSize);
    barcode = tmpcode;
    tmpSet.insert(tAddr + TEMPCODE, tmpcode);
    sendSqlite();
    scanner->stop();
    tmpcode.clear();
}

void AppWindow::showBoxPop(QString text, int t)
{
    boxbar->setLabelText(text);
    wait(10);
    (this->*initMap[t])();
    boxbar->setValue((t+1)*100/initMap.size());
    //    qDebug() << "box show:" << text;
}

void AppWindow::saveBackup(QTmpMap msg)
{
    boxbar->setLabelText(tr("正在保存后台数据"));
    boxbar->show();
    wait(10);
    QString name = "backup";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = msg.keys();
    QSqlDatabase::database(name).transaction();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        tmpSet.insert(uuid, msg.value(uuid));
        if (uuid >= 10000 && uuid < 20000) {
            query.prepare("replace into aip_backup values(?,?)");
            query.addBindValue(uuid);
            query.addBindValue(tmpSet[uuid]);
            query.exec();
        }
        boxbar->setValue(i*50/uuids.size());
    }
    QSqlDatabase::database(name).commit();
    boxbar->setValue(99);
    wait(500);
    query.clear();
    boxbar->setValue(100);
}

void AppWindow::saveSqlite(QTmpMap msg)
{
    boxbar->setLabelText(tr("正在保存系统数据"));
    boxbar->show();
    wait(10);
    QString name = "system";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = msg.keys();
    QSqlDatabase::database(name).transaction();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        tmpSet.insert(uuid, msg.value(uuid));
        if (uuid >= 20000 && uuid < 30000) {
            query.prepare("replace into aip_system values(?,?)");
            query.addBindValue(uuid);
            query.addBindValue(msg[uuid]);
            query.exec();
        }
        boxbar->setValue(i*50/uuids.size());
    }
    QSqlDatabase::database(name).commit();
    boxbar->setValue(99);
    wait(500);
    query.clear();
    boxbar->setValue(100);
}

void AppWindow::saveConfig(QTmpMap msg)
{
    QElapsedTimer tt;
    tt.start();
    boxbar->setLabelText(tr("正在保存配置数据"));
    boxbar->show();
    wait(10);
    QString name = "config";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = msg.keys();
    QSqlDatabase::database(name).transaction();
    QString c_numb = tr("%1").arg(tmpSet.value(DataFile).toInt(), 4, 10, QChar('0'));
    QString c_name = tmpSet.value(DataType).toString();
    c_name = tr("T%1_%2").arg(c_numb).arg(c_name);
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        if (uuid >= 40000) {
            tmpSet.insert(uuid, msg.value(uuid));
            query.prepare(QString("replace into '%1' values(?,?)").arg(c_name));
            query.addBindValue(uuid);
            query.addBindValue(msg.value(uuid));
            query.exec();
        }
        boxbar->setValue(i*99/uuids.size());
    }
    QSqlDatabase::database(name).commit();
    boxbar->setValue(99);
    wait(500);
    query.clear();
    boxbar->setValue(100);
    testShift = Qt::Key_Away;
    qDebug() << "app save:" << tr("%1ms").arg(tt.elapsed(), 4, 10, QChar('0')) << c_name;
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
        if (gs.contains(buttons.at(i)->objectName()))
            buttons.at(i)->show();
        else
            buttons.at(i)->hide();
        if (buttons.at(i)->objectName() == msg)
            buttons.at(i)->setChecked(true);
        else
            buttons.at(i)->setChecked(false);
    }
    if (win.value("form").toInt() > 0)
        btnFrame->show();
    else
        btnFrame->hide();
    if (gear > role)
        return false;
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    return true;
}

bool AppWindow::screensSave(QString msg)
{
    if (isChange) {
        QString w = tr("数据未保存,确定退出吗?");
        int ret = QMessageBox::warning(this, tr("警告"), w, QMessageBox::Close | QMessageBox::Save);
        if (ret == QMessageBox::Save) {
            tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
            tmpMsg.insert(Qt::Key_1, msg);
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
    }
    isChange = false;
    return true;
}

void AppWindow::screensShow(QString msg)
{
    qDebug() << "app show:" << msg;
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
    sendUdpStr("6008");  // 进入测试界面
    wait(200);
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    double syst = tmpSet.value(2000 + Qt::Key_1).toInt();  // 系统设置地址
    double tmOK = tmpSet.value(syst + SystTime).toDouble();
    double tmNG = tmpSet.value(syst + SystWarn).toDouble();
    double conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合设置地址

    if (mode >= 2) {
        int driv = tmpSet.value(conf + ADDRDRIV).toInt();
        sendUdpStr("6058 1");  // IO状态上传
        wait(100);
        sendUdpStr(tr("6072 %1").arg(driv).toUtf8());  // 内外置接触器切换
        wait(100);
        sendUdpStr(tr("6073 %1").arg(driv).toUtf8());  // 内外置接触器切换
        wait(100);
        int stop = (driv == 1) ? 1 : 2;
        int work = (station == WORKL) ? 1 : 2;
        sendUdpStr(QString("6074 %1 %2 3 %3 %4").arg(stop).arg(work).arg(3).arg(0).toUtf8());
        wait(100);
    }
    if (mode == 1) {  // 真空模式
        QStringList testItems = tmpSet.value(conf + ADDRITEM).toString().split(",");
        if (testItems.contains(QString::number(0x06))) {  // 测试匝间
            int addr = tmpSet.value(4000 + Qt::Key_6).toInt();
            QString str = QString::number(tmpSet.value(addr + 0x00).toInt());
            sendUdpStr(tr("6052 %1").arg(str).toUtf8());
            wait(200);
        }
        if (testItems.contains(QString::number(0x04))) {  // 测试交耐
            int addr = tmpSet.value(4000 + Qt::Key_4).toInt();
            QString str = QString::number(tmpSet.value(addr + 0x00).toInt());
            sendUdpStr(tr("6068 %1").arg(str).toUtf8());
            wait(200);
        }
        if (1) {
            int addr = tmpSet.value(4000 + Qt::Key_0).toInt();  // 自动启动
            QString str = QString::number(tmpSet.value(addr + ADDRAUTO).toInt());
            sendUdpStr(tr("6067 %1").arg(str).toUtf8());
            wait(200);
        }
    }
    sendUdpStr(tr("6071 %1 %2").arg(tmOK).arg(tmNG).toUtf8());  // 报警时间
    wait(200);
    sendUdpStr("6008");  // 进入测试界面
    wait(500);
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
    if (mode == 1) {  // 真空模式
        sendUdpStr("6052 0");
        sendUdpStr("6068 0");
        sendUdpStr("6067 0");
    }
    e->accept();
}

void AppWindow::keyReleaseEvent(QKeyEvent *e)
{
    tmpcode.append(e->text());
    scanner->start(200);
    e->accept();
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
        if (msg.value(Qt::Key_1).toString() == "aip_backup") {  // 后台参数保存
            saveBackup(msg);
            sendSqlite();
        }
        if (msg.value(Qt::Key_1).toString() == "aip_system") {  // 系统参数保存
            saveSqlite(msg);
            sendSqlite();
        }
        if (msg.value(Qt::Key_1).toString() == "aip_config") {  // 配置参数保存
            saveConfig(msg);
            sendSqlite();
        }
        if (msg.value(Qt::Key_1).toString() == "aip_reload") {  // 重新加载参数
            saveSqlite(msg);
            readConfig();
            sendSqlite();
            taskConfig();
        }
        if (msg.value(Qt::Key_1).toString() == "aip_tester") {  // 配置保存数据
            tmpSave = msg;
        }
        isChange = false;
        break;
    case Qt::Key_Play:
        tmp = (msg.value(Qt::Key_1).toString() == "R") ? 0x14 : 0x13;
        if (recvIoCtrl(Qt::Key_Play, tmp) == Qt::Key_Away) {
            taskShift = Qt::Key_Play;
            station = tmp;
        }
        break;
    case Qt::Key_Stop:  // 停止测试
        if (currItem == 0x0C || currItem == 0x0D) {
            tempShift = Qt::Key_Away;
            break;
        }
        taskCheckStop();
        if (!msg.value(Qt::Key_1).isNull()) {
            sendUdpStr("6081");
        }
        break;
    case Qt::Key_Call:  // 数据修改未保存
        qDebug() << "change" << qobject_cast<QWidget*>(sender())->objectName();
        isChange = true;
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
        boxbar->show();
        boxbar->setLabelText(tr("正在查询数据"));
        boxbar->setValue(1);
        wait(1);
        emit sendAppMsg(msg);
        break;
    case Qt::Key_Word:
        if (!msg.value(Qt::Key_1).isNull())
            boxbar->setLabelText(msg.value(Qt::Key_1).toString());
        if (!msg.value(Qt::Key_2).isNull()) {
            if (msg.value(Qt::Key_2).toInt() >= 100)
                wait(1000);
            boxbar->setValue(msg.value(Qt::Key_2).toInt());
        }
        break;
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

        tmpMap = msg.value(keys.at(i)).toMap();
        QStringList key = tmpMap.keys();
        for (int t=0; t < key.size(); t++) {
            QDomElement element = doc.createElement(key.at(t));
            QDomText text = doc.createTextNode(tmpMap.value(key.at(t)).toString());
            if ((keys.at(i) == "LOAD" || keys.at(i) == "NOLOAD") && key.at(t) == "time") {
                int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 型号设置地址
                int mode = tmpSet.value(conf + ADDRDRIV).toInt();  // 内外置驱动设置
                if (mode == 1) {
                    double tt = tmpMap.value(key.at(t)).toDouble();
                    text = doc.createTextNode(QString::number(tt/2)); // 下发负载测试时间的50%
                }
            }
            if ((keys.at(i) == "BEMF") && key.at(t) == "time") {
                double tt = tmpMap.value(key.at(t)).toDouble();
                text = doc.createTextNode(QString::number(tt/2)); // 下发负载测试时间的50%
            }
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
    case Qt::Key_Stop:
        warnningString(msg.value("text").toString());
        break;
    case Qt::Key_Send:
        speed = msg.value("text").toInt();
        break;
    default:
        break;
    }
}

void AppWindow::recvUdpMsg(QByteArray msg)
{
    if (isudp == 0)
        QTimer::singleShot(1000, this, SLOT(taskConfig()));
    isudp = 1;
    timeUdp = 0;
    int cmd = msg.mid(0, 4).toInt();
    QString dat = msg.mid(5, msg.size());
    quint32 hex = dat.toInt();
    switch (cmd) {
    case 6001:  // 自检信息
        recvStaMsg(dat);
        break;
    case 6005:  // 上传测试界面显示信息
        recvNewMsg(dat);
        break;
    case 6007:  // 单项测试完成
        testShift = Qt::Key_Away;
        if (tempShift == Qt::Key_Away)
            taskCheckStop();
        break;
    case 6015:  // 空载/负载启动完成
        break;
    case 6019:  // 空载/负载/霍尔结果
        if (currItem == 0x0B) //  霍尔
            calcHALL(dat);
        if (currItem == 0x0C) // 负载
            calcLOAD(dat);
        if (currItem == 0x0E)  // 反电动势
            calcBEMF(dat);
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
        ioHex = hex;
        recvIoCtrl(Qt::Key_Meta, station);
    case 6059:  // IO板输出状态
    case 6035:  // 反嵌采样结果
    case 6039:  // 反嵌采样波形
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, dat);
        tmpMsg.insert(Qt::Key_2, cmd);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
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
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, dat);
        tmpMsg.insert(Qt::Key_2, cmd);
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
    case 6053:  // A8询问是否真空模式
        sendUdpStr("6052 1");
        break;
    case 6060:  // 真空上传启动信号
        station = dat.toInt();
        taskShift = Qt::Key_Play;
        break;
    case 6061:  // 真空上传停止信号
        if (currItem == 0x0C || currItem == 0x0D) {
            tempShift = Qt::Key_Away;
            break;
        }
        taskCheckStop();
        break;
    case 6083:
        isok = (dat == "NG") ? DATANG : DATAOK;
        break;
    case 6062:  // 真空泵待机
        recvAppShow("author");
        warnningString(tr("真空泵已关闭"));
        break;
    case 6076:  // 上传RFID
        codeShift = Qt::Key_Away;
        barcode = dat.toUpper();
        showBarCode();
        break;
    default:
        break;
    }
}

void AppWindow::sendUdpStr(QByteArray msg)
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, msg);
    emit sendUdpMsg(tmpMsg);
    tmpMsg.clear();
}

void AppWindow::recvNewMsg(QString dat)
{
    if (currItem >= 0x0B)  // BLDC不显示
        return;
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
                timeRsl = ((currItem == 0x01) && (!temp.contains("Ω")) ? 0x08 : timeRsl);
            }
            timeRsl = ((currItem == 0x02) && (temp.contains(tr("正转")))) ? 0x08 : timeRsl;
            timeRsl = ((currItem == 0x02) && (temp.contains(tr("反转")))) ? 0x08 : timeRsl;
            timeRsl = ((currItem == 0x02) && (temp.contains(tr("不转")))) ? 0x08 : timeRsl;

            tmpMsg.insert(Qt::Key_0, Qt::Key_News);
            tmpMsg.insert(Qt::Key_1, currItem);
            tmpMsg.insert(Qt::Key_2, timeRsl);
            tmpMsg.insert((xml == "Test_Data_Result") ? Qt::Key_3 : Qt::Key_4, temp);
            tmpMsg.insert(Qt::Key_6, station);
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();

            int addr = tmpSet.value(3000 + Qt::Key_0 + currItem).toInt() + timeRsl*0x10;
            int conf = tmpSet.value(4000 + Qt::Key_0 + currItem).toInt();
            if (xml == "Test_Data_Judge") {
                if (currItem != 0x06)
                    tmpSave.insert(addr + 0x03, temp);
                isok = (temp == "OK") ? isok : DATANG;
                timeRsl++;
            } else {
                if (currItem == 0x03 || currItem == 0x04) {  // 绝缘,交耐
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
                tmpSave.insert(addr + 0x02, temp);
            }
        }
    }
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
        if (cmd >= errs.size() && cmd < 0xFF) {
            err += tr("未知异常") + QString::number(cmd) + "\n";
        }
        if (cmd <= errs.size()) {
            if ((cmd != 18) || (cmd == 18 && (ioHex & 0x004000) != 0))
                err += errs.at(cmd) + "\n";
        }
    }
    if (!err.isEmpty()) {
        tmpMsg.insert(Qt::Key_0, Qt::Key_Word);
        tmpMsg.insert(Qt::Key_1, err);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        warnningString("\n" + err + "\n");
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
                hhh << qMax(0.0, (tmp2.at(100 + i*20).toDouble()*15.28/4095) * kh - bh);
                lll << qMax(0.0, (tmp2.at(101 + i*20).toDouble()*15.28/4095) * kl - bl);
                ddd << qMax(0.0, (tmp2.at(21 + i*4).toDouble()/1000) * kd - bd);
                fff << qMax(0.0, (tmp2.at(20 + i*4).toDouble()/1000) * kf - bf);
            }
        }
        for (int numb=0; numb < 4; numb++) {
            double vmax = tmpSet.value(hall + numb*2 + 0x00).toDouble();
            double vmin = tmpSet.value(hall + numb*2 + 0x01).toDouble();
            int r = DATAOK;
            QStringList reals;
            if (vmax != 0) {
                if (numb == 0) {
                    for (int t=0; t < hhh.size(); t++) {
                        reals << QString::number(hhh.at(t), 'f', 2) +"V";
                        if (hhh.at(t) >= vmax || hhh.at(t) <= vmin) {
                            r = DATANG;
                        }
                        tmpSave.insert(addr + t*0x10 + numb, hhh.at(t));
                    }
                }
                if (numb == 1) {
                    for (int t=0; t < lll.size(); t++) {
                        reals << QString::number(lll.at(t), 'f', 2) +"V";
                        if (lll.at(t) >= vmax || lll.at(t) <= vmin) {
                            r = DATANG;
                        }
                        tmpSave.insert(addr + t*0x10 + numb, lll.at(t));
                    }
                }
                if (numb == 2) {
                    for (int t=0; t < ddd.size(); t++) {
                        reals << QString::number(ddd.at(t), 'f', 2) +"%";
                        if (ddd.at(t) >= vmax || ddd.at(t) <= vmin) {
                            r = DATANG;
                        }
                        tmpSave.insert(addr + t*0x10 + numb, ddd.at(t));
                    }
                }
                if (numb == 3) {
                    for (int t=0; t < fff.size(); t++) {
                        reals << QString::number(fff.at(t), 'f', 2) +"Hz";
                        if (fff.at(t) >= vmax || fff.at(t) <= vmin) {
                            r = DATANG;
                        }
                        tmpSave.insert(addr + t*0x10 + numb, fff.at(t));
                    }
                }
            }
            isok = (r == DATAOK) ? isok : DATANG;
            tmpMsg.insert(Qt::Key_0, Qt::Key_News);
            tmpMsg.insert(Qt::Key_1, currItem);
            tmpMsg.insert(Qt::Key_2, numb);
            tmpMsg.insert(Qt::Key_3, reals.join(","));
            tmpMsg.insert(Qt::Key_4, (r == DATAOK) ? "OK" : "NG");
            tmpMsg.insert(Qt::Key_6, station);
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
    }
}

void AppWindow::calcLOAD(QString msg)
{
    int addr = tmpSet.value(3000 + Qt::Key_C).toInt();  // 负载结果地址
    int hall = tmpSet.value(4000 + Qt::Key_B).toInt();  // 霍尔配置地址
    int load = tmpSet.value(4000 + Qt::Key_C).toInt();  // 负载配置地址
    int numb = tmpSet.value(hall + 8).toInt();  // 磁极数
    int from = tmpSet.value(load + 9).toInt();  // 转速读取位置
    if (from != 0) {
        if (station == 0x13)
            servoL.test();
        else
            servoR.test();
    }
    QStringList tmp2 = msg.split(" ");
    if (tmp2.size() >= 30 && currItem == 0x0C) {  // 负载计算
        int back = tmpSet.value(1000 + Qt::Key_0).toInt() + 0x80;  // K,b值存储位置
        double kc = tmpSet.value(back + 0x00).toDouble();
        double bc = tmpSet.value(back + 0x01).toDouble();
        double kv = tmpSet.value(back + 0x02).toDouble();
        double bv = tmpSet.value(back + 0x03).toDouble();
        double kp = tmpSet.value(back + 0x04).toDouble();
        double bp = tmpSet.value(back + 0x05).toDouble();
        double ki = tmpSet.value(back + 0x06).toDouble();
        double bi = tmpSet.value(back + 0x07).toDouble();
        kc = (kc == 0) ? 1 : kc;
        kv = (kv == 0) ? 1 : kv;
        kp = (kp == 0) ? 1 : kp;
        ki = (ki == 0) ? 1 : ki;
        double crr = (tmp2.at(20 + 0x00).toDouble()*1000) * kc + bc;  // 电流换算为mA
        double vlt = (tmp2.at(20 + 0x01).toDouble()*500) * kv + bv;  // 电压换算为V
        double pwr = (crr*vlt/1000) * kp + bp;  // 功率
        double icc = (tmp2.at(20 + 0x04).toDouble()) * ki + bi;  // icc电流
        double rpm = tmp2.at(20 + 0x03).toDouble()*60/1000/numb;  // 转速
        double ccw = tmp2.at(20 + 0x06).toDouble();  // 转向
        double ext = tmp2.at(20 + 0x09).toDouble();
        crr = qMax(0.0, crr);
        vlt = qMax(0.0, vlt);
        pwr = qMax(0.0, pwr);
        icc = qMax(0.0, icc);
        if (tempShift == Qt::Key_Away && crr < 0.1) {
            int work = (station == WORKL) ? 1 : 2;
            sendUdpStr(QString("6074 %1 %2 3 %3 %4").arg(5).arg(work).arg(3).arg(0).toUtf8());
        }
        QString turn;
        if (from == 1) {
            double tmp = 0;
            speeds.append(speed);
            if (speeds.size() > 5) {
                speeds.removeFirst();
                for (int i=0; i < speeds.size()-2; i++) {
                    tmp += speeds.at(i);
                }
                tmp = tmp / (speeds.size()-2);
            } else {
                tmp = speed;
            }
            rpm = abs(tmp);
            ccw = (abs(tmp) < 5) ? 0: ccw;
            ccw = (tmp < -5) ? 1 : ccw;
            ccw = (tmp > 5) ? 2 : ccw;
            qDebug() << "app load:" << speed;
        }
        turn = (ccw == 0) ? "NULL" : turn;
        turn = (ccw == 1) ? "CCW" : turn;
        turn = (ccw == 2) ? "CW" : turn;

        QList<double> reals;
        reals << crr << pwr << icc << rpm << ccw;
        for (int i=0; i < reals.size(); i++) {
            double imax = tmpSet.value(load + i * 2 + 0x00).toDouble();
            double imin = tmpSet.value(load + i * 2 + 0x01).toDouble();
            int r = DATAOK;
            tmpMsg.insert(Qt::Key_0, Qt::Key_News);
            tmpMsg.insert(Qt::Key_1, currItem);
            tmpMsg.insert(Qt::Key_2, i);
            if (i == 0) {
                QString str = QString::number(vlt, 'f', 2) + "V,";
                str += QString::number(reals.at(i), 'f', 2) + "mA";
                tmpMsg.insert(Qt::Key_3, str);
            }
            if (i == 1)
                tmpMsg.insert(Qt::Key_3, QString::number(vlt, 'f', 2) + "V");
            if (i == 2)
                tmpMsg.insert(Qt::Key_3, QString::number(reals.at(i), 'f', 0) + "mA");
            if (i == 3)
                tmpMsg.insert(Qt::Key_3, QString::number(reals.at(i), 'f', 0) + "rpm");
            if (i == 4)
                tmpMsg.insert(Qt::Key_3, turn);
            if (imax != 0) {
                if (i == 1)
                    tmpMsg.insert(Qt::Key_3, QString::number(reals.at(i), 'f', 2) + "W");
                if (i == 4 && reals.at(i) != imax)
                    r = DATANG;
                if (i != 4 &&  (reals.at(i) >= imax || reals.at(i) <= imin))
                    r = DATANG;
            }
            if (ext != 0) {
                if (imax != 0)
                    tmpSave.insert(addr + i, reals.at(i));
                isok = (r == DATAOK) ? isok : DATANG;
                tmpMsg.insert(Qt::Key_4, (r == DATAOK) ? "OK" : "NG");
            }
            tmpMsg.insert(Qt::Key_6, station);
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
    }
}

void AppWindow::calcBEMF(QString msg)
{
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
        double ext = tmp2.at(20 + 0x09).toDouble();
        QList<double> volts;
        QList<double> bemfs;
        QList<double> diffs;
        for (int numb=0; numb < 5; numb++) {
            int vr = DATAOK;
            QStringList vrs;
            double vmax = tmpSet.value(bemf + numb * 2 + 0x00).toDouble();
            double vmin = tmpSet.value(bemf + numb * 2 + 0x01).toDouble();
            if (vmax != 0) {
                if (numb == 0) {
                    for (int t=0; t < 3; t++) {
                        volts << qMax(0.0, (tmp2.at(20+7+t).toDouble() * 15.28 / 4095) * ku + bu);
                        vrs << QString::number(volts.at(t), 'f', 2) + "V";
                        if ((vmax != 0) && (volts.at(t) >= vmax || volts.at(t) < vmin))
                            vr = DATANG;
                        tmpSave.insert(addr + t * 0x10 + numb, volts.at(t));
                    }
                }
                if (numb == 1) {
                    for (int t=0; t < 3; t++) {
                        bemfs << qMax(0.0, (volts.at(t) * 1000 / stds) * kv + bv);
                        vrs << QString::number(bemfs.at(t), 'f', 2) + "V/krpm";
                        if ((vmax != 0) && (bemfs.at(t) >= vmax || bemfs.at(t) < vmin))
                            vr = DATANG;
                        tmpSave.insert(addr + t * 0x10 + numb, bemfs.at(t));
                    }
                }
                if (numb == 2) {
                    for (int t=0; t < 3; t++) {
                        diffs << qMax(0.0, (tmp2.at(12 + t).toDouble() / 100) * kw + bw);
                        vrs << QString::number(diffs.at(t), 'f', 1) + "°";
                        if ((vmax != 0) && (diffs.at(t) >= vmax || diffs.at(t) < vmin))
                            vr = DATANG;
                        tmpSave.insert(addr + t * 0x10 + numb, diffs.at(t));
                    }
                }
                if (numb == 3 && volts.size() >= 3) {
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
                    vrs << tr("%1%").arg(tmp, 0, 'g', 3);
                    if ((vmax != 0) && (tmp >= vmax || tmp < vmin))
                        vr = DATANG;
                    tmpSave.insert(addr + numb * 0x10, tmp);
                }
                if (numb == 4) {
                    int turn = tmp2.at(20 + 6).toInt();
                    if (turn == 0)
                        vrs << "NULL";
                    if (turn == 1)
                        vrs << "ABC";
                    if (turn == 2)
                        vrs << "ACB";
                    if (turn != vmax)
                        vr = DATANG;
                    tmpSave.insert(addr + numb * 0x10, vrs);
                }
            }
            tmpMsg.insert(Qt::Key_0, Qt::Key_News);
            tmpMsg.insert(Qt::Key_1, currItem);
            tmpMsg.insert(Qt::Key_6, station);
            tmpMsg.insert(Qt::Key_2, numb);
            tmpMsg.insert(Qt::Key_3, vrs.join("  "));
            if (ext != 0) {
                isok = (vr == DATAOK) ? isok : DATANG;
                tmpMsg.insert(Qt::Key_4, (vr == DATAOK) ? "OK" : "NG");
            }
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
    }
}
