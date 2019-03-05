/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#include "apptester.h"

const QString strSW = "<p style='font:11pt;color:#FFFFFF;' align='left'>%1</p>";  // 小号白,状态信息
const QString strSG = "<p style='font:11pt;color:#666666;' align='left'>%1</p>";  // 小号灰
const QString strSY = "<p style='font:11pt;color:#FFFF00;'>%1</p>";  // 小号黄,错误信息

const QString strMB = "<p style='font:18pt;color:#FFFF00;' align='center'>%1</p>";  // 中号蓝,公司
const QString strMY = "<p style='font:18pt;color:#FFFF00;' align='left'>&nbsp;%1</p>";  // 中号黄

const QString strLG = "<p style='font:64pt;color:#00FF00;'><b>%1</b></p>";  // 大号绿,OK
const QString strLR = "<p style='font:64pt;color:#FF0000;'><b>%1</b></p>";  // 大号红,NG,中断
const QString strLY = "<p style='font:64pt;color:#FFFF00;'><b>%1</b></p>";  // 大号黄,工位,测试

AppTester::AppTester(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppTester::initUI()
{
    initLayout();
    initViewBar();
    initTestBar();
    initLogoBar();
    initWorkBar();
    initRealBar();
    initWaveMAG();
    initWaveIMP();
    initWaveBMF();
    initWaveHAL();
    initTypeBar();
    initWaveAll();
    initButtonBar();
    initHistogram();
    initWorkState();
    initWireColor();
    initOtherParm();
}

void AppTester::initLayout()
{
    layout = new QHBoxLayout(this);
}

void AppTester::initViewBar()
{
    wView = new QTableWidget(this);
    wView->horizontalHeader()->setVisible(false);
    wView->verticalHeader()->setVisible(false);
    wView->setColumnCount(6);
    wView->setRowCount(21);
    wView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    wView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(wView);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(lay);
    layout->addWidget(box);
}

void AppTester::initTestBar()
{
    QStringList headers;
    headers << tr("测试项目") << tr("测试参数") << tr("测试结果") << tr("测试判定");

    mView = new QTableWidget(this);
    mView->setStyleSheet("border:none");
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);
    mView->verticalHeader()->setFixedWidth(24);
    mView->horizontalHeader()->setFixedHeight(32);
    mView->verticalHeader()->setMinimumSectionSize(30);
    mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    mView->setColumnWidth(0, 120);
    mView->setColumnWidth(1, 200);
    mView->setColumnWidth(3, 96);
    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(mView);

    QFrame *frm = new QFrame(this);
    frm->setLayout(box);

    wView->setSpan(0, 0 , 9, 4);
    wView->setCellWidget(0, 0, frm);
}

void AppTester::initLogoBar()
{
    QLabel *title = new QLabel(this);
    title->setText(strMB.arg(tr("青岛艾普智能仪器有限公司")));
    wView->setCellWidget(0, 4, title);
    wView->setSpan(0, 4, 1, 2);
    wView->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    wView->setRowHeight(0, 42);
}

void AppTester::initWorkBar()
{
    workText = new QLabel(this);
    workText->setText(strLY.arg("左"));
    wView->setCellWidget(1, 4, workText);
    wView->setSpan(1, 4, 2, 1);
}

void AppTester::initRealBar()
{
    realText = new QLabel(this);
    realText->setText(strLG.arg("OK"));
    wView->setCellWidget(1, 5, realText);
    wView->setSpan(1, 5, 2, 1);
}

void AppTester::initWaveMAG()
{
    QStringList magitem;
    magitem << tr("项目") << tr("参数") << tr("差积");

    int srow = 9;
    wView->verticalHeader()->setSectionResizeMode(srow, QHeaderView::Fixed);
    wView->setRowHeight(srow, 80);
    for (int i=0; i < 3; i++) {
        int row = i/3*1 + srow;
        int col = i%3*2;
        QGridLayout *lay = new QGridLayout;

        BoxQImage *w = new BoxQImage(this);
        connect(w, SIGNAL(clicked()), this, SLOT(clickWave()));
        lay->addWidget(w, 0, 0, magitem.size()+1, 1);

        for (int k=0; k < magitem.size(); k++) {
            QLabel *m = new QLabel(strSG.arg(magitem.at(k) + ":----"), this);
            lay->addWidget(m, k, 1);
            magText.append(m);
        }
        lay->setColumnStretch(0, 3);
        lay->setColumnStretch(1, 1);

        QFrame *frm = new QFrame(this);
        frm->setLayout(lay);
        wView->setSpan(row, col, 2, 2);
        wView->setCellWidget(row, col, frm);

        magWave.append(w);
    }
}

void AppTester::initWaveIMP()
{
    QStringList impitem;
    impitem << tr("项目") << tr("面积") << tr("差积") << tr("电晕") << tr("相位");

    int srow = 11;
    wView->verticalHeader()->setSectionResizeMode(srow + 0, QHeaderView::Fixed);
    wView->setRowHeight(srow + 0, 80);
    wView->verticalHeader()->setSectionResizeMode(srow + 2, QHeaderView::Fixed);
    wView->setRowHeight(srow + 2, 80);
    for (int i=0; i < 6; i++) {
        QFrame *frm = new QFrame(this);
        int row = i/3*2 + srow;
        int col = i%3*2;
        wView->setSpan(row, col, 2, 2);
        wView->setCellWidget(row, col, frm);
        QGridLayout *lay = new QGridLayout;
        frm->setLayout(lay);

        BoxQImage *w = new BoxQImage(this);
        impWave.append(w);
        connect(w, SIGNAL(clicked()), this, SLOT(clickWave()));

        lay->addWidget(w, 0, 0, impitem.size(), 1);

        for (int k=0; k < impitem.size(); k++) {
            QLabel *m = new QLabel(strSG.arg(impitem.at(k) + "----"), this);
            lay->addWidget(m, k, 1);
            impText.append(m);
        }
        lay->setColumnStretch(0, 3);
        lay->setColumnStretch(1, 1);
    }
}

void AppTester::initWaveBMF()
{
    bWave = new BoxQImage(this);
    wView->setCellWidget(15, 0, bWave);
    wView->setSpan(15, 0, 2, 6);
    int srow = 15;
    wView->verticalHeader()->setSectionResizeMode(srow + 0, QHeaderView::Fixed);
    wView->setRowHeight(srow + 0, 80);
}

void AppTester::initWaveHAL()
{
    hWave = new BoxQImage(this);
    wView->setCellWidget(17, 0, hWave);

    wView->setSpan(17, 0, 4, 6);
    for (int i=15; i < 21; i++) {
        wView->hideRow(i);
    }
    int srow = 17;
    wView->verticalHeader()->setSectionResizeMode(srow + 0, QHeaderView::Fixed);
    wView->setRowHeight(srow + 0, 80);
    wView->verticalHeader()->setSectionResizeMode(srow + 2, QHeaderView::Fixed);
    wView->setRowHeight(srow + 2, 80);
}

void AppTester::initTypeBar()
{
    isInit = false;
    typeText = new QComboBox(this);
    typeText->setEditable(true);
    typeText->setView(new QListView);
    typeText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(typeText, SIGNAL(currentIndexChanged(QString)), this, SLOT(clickType()));
    typeText->setStyleSheet("font:18pt;color:yellow");

    QCompleter *pCompleter = new QCompleter(typeText->model(), this);
    typeText->setCompleter(pCompleter);

    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);
    lay->addWidget(new QLabel(strMY.arg(tr("当前型号:")), this));
    lay->addWidget(typeText);

    QFrame *frm = new QFrame(this);
    frm->setLayout(lay);
    wView->setCellWidget(3, 4, frm);
    wView->setSpan(3, 4, 1, 2);

    codeText = new QLabel(this);
    codeText->setText(strMY.arg(tr("当前编码:178912013X")));
    wView->setCellWidget(4, 4, codeText);
    wView->setSpan(4, 4, 1, 2);
}

void AppTester::initWaveAll()
{
    allWave = new BoxQImage(this);
    allWave->hide();
    connect(allWave, SIGNAL(clicked()), allWave, SLOT(hide()));
}

void AppTester::initButtonBar()
{
    ms.start();
    tmpTime = 0;
    blayout = new QVBoxLayout;

    btnHome = new QPushButton("开机主页", this);
    blayout->addWidget(btnHome);
    btnHome->setObjectName("author");
    btnHome->setMinimumSize(97, 56);
    connect(btnHome, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    btnConf = new QPushButton("型号管理", this);
    blayout->addWidget(btnConf);
    btnConf->setObjectName("config");
    btnConf->setMinimumSize(97, 56);
    connect(btnConf, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    btnZero = new QPushButton("状态复位", this);
    blayout->addWidget(btnZero);
    btnZero->setMinimumSize(97, 56);
    connect(btnZero, SIGNAL(clicked(bool)), this, SLOT(clickReset()));

    QLabel *btnLogo = new QLabel(this);
    blayout->addWidget(btnLogo);
    btnLogo->setPixmap(QPixmap("./qrc/icon_aip.png"));
    btnLogo->setScaledContents(true);
    btnLogo->setFixedHeight(66);

    btnL = new QRadioButton(this);
    btnL->setText(tr("左工位"));
    btnL->setChecked(true);
    btnR = new QRadioButton(this);
    btnR->setText(tr("右工位"));

    QHBoxLayout *rlay = new QHBoxLayout;
    rlay->setMargin(0);
    rlay->addWidget(btnL);
    rlay->addWidget(btnR);
    wFrame = new QFrame(this);
    wFrame->setLayout(rlay);
    blayout->addWidget(wFrame);

    btnTest = new QPushButton("开始测试", this);
    blayout->addWidget(btnTest);
    btnTest->setMinimumSize(97, 56);
    connect(btnTest, SIGNAL(clicked(bool)), this, SLOT(clickTest()));

    btnStop = new QPushButton("停止测试", this);
    blayout->addWidget(btnStop);
    btnStop->setMinimumSize(97, 56);
    connect(btnStop, SIGNAL(clicked(bool)), this, SLOT(clickStop()));

    blayout->addStretch();

    warnText = new QLabel(this);
    warnText->setFixedHeight(120);
    blayout->addWidget(warnText);

    blayout->addStretch();

    QGroupBox *box = new QGroupBox(this);
    box->setFixedWidth(180);
    box->setLayout(blayout);
    layout->addWidget(box);
}

void AppTester::initHistogram()
{
    bodys = new BoxQImage(this);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(bodys);

    QFrame *frm = new QFrame(this);
    frm->setLayout(lay);

    wView->setCellWidget(5, 4, frm);
    wView->setSpan(5, 4, 4, 2);

    testQu = 0;
    testOK = 0;

    QVariantMap tmp;
    tmp.insert("index", 0);
    tmp.insert("names", tr("总数"));
    tmp.insert("point", testQu);
    bodys->setBodys(tmp);
    tmpQua.append(tmp);

    tmp.insert("index", 0xFF);
    tmp.insert("names", tr("合格"));
    tmp.insert("point", testOK);
    bodys->setBodys(tmp);
    tmpQua.append(tmp);
}

void AppTester::initWorkState()
{
    QGridLayout *wlayout = new QGridLayout;
    blayout->addLayout(wlayout);

    QStringList names, texts;
    names << tr("光幕") << tr("产品");
    texts << "stop" << "prod";
    for (int i=0; i < names.size(); i++) {
        QLabel *text = new QLabel(names.at(i), this);
        text->setFixedHeight(25);
        text->setStyleSheet("background:#121919");
        wlayout->addWidget(text, i/2, i%2);
        labels.insert(texts.at(i), text);
    }
}

void AppTester::initWireColor()
{
    QGridLayout *colorLayout = new QGridLayout;
    for (int i=0; i < 8; i++) {
        QLabel *color = new QLabel(QString::number(i+1));
        color->setFixedHeight(25);
        colorLayout->addWidget(color, i/2, i%2);
        color->setAlignment(Qt::AlignCenter);
        colors.append(color);
    }
    blayout->addLayout(colorLayout);
}

void AppTester::initOtherParm()
{
    QGridLayout *lay = new QGridLayout;
    blayout->addLayout(lay);

    signText = new QLabel(this);
    signText->setText(strSW.arg(tr("网络状态:____")));
    lay->addWidget(signText, 0, 1);

    userText = new QLabel(this);
    userText->setText(strSW.arg(tr("当前用户:____")));
    lay->addWidget(userText, 1, 1);

    tempText = new QLabel(strSW.arg(tr("当前温度:____")), this);
    lay->addWidget(tempText, 2, 1);

    testText = new QLabel(strSW.arg(tr("测试时间:____")), this);
    lay->addWidget(testText, 3, 1);

    dateText = new QLabel(this);
    lay->addWidget(dateText, 4, 1);
    QString strA = tr("当前日期:") + QDate::currentDate().toString("yy-MM-dd");
    dateText->setText(strSW.arg(strA));

    timeText = new QLabel(this);
    lay->addWidget(timeText, 5, 1);
    QString strB = tr("当前时间:") + QTime::currentTime().toString("hh:mm:ss");
    timeText->setText(strSW.arg(strB));

    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(1, 3);
    lay->setColumnStretch(2, 1);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(100);
}

void AppTester::initSettings()
{
    isInit = false;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int mode = tmpSet.value(back + backMode).toInt();
    int test = tmpSet.value(back + backTest).toInt();
    int work = tmpSet.value(back + backWork).toInt();
    wFrame->setVisible((work == 2) ? true : false);
    workText->setText(strLY.arg((work == 0) ? "右" : "左"));
    labels.value("stop")->setVisible((mode == 1 && (test & 0x01)) ? true : false);
    labels.value("prod")->setVisible((mode == 1 && (test & 0x01)) ? true : false);

    tmpNG.clear();
    initQuality();
    if (Qt::Key_0) {  // 初始化状态栏显示
        int temp = tmpSet.value((3000 + Qt::Key_0)).toInt();  // 时临数据地址
        int sign = tmpSet.value(temp + TEMPSIGN).toInt();
        int setuser = tmpSet.value(DataUser).toInt();
        QString strSign = (sign == 0) ? tr("未登录") : tmpSet.value(setuser).toString();
        userText->setText(strSW.arg(tr("当前用户:") + strSign));
    }
    if (Qt::Key_0) {
        tmpRow = 0;
        int r = tmpSet.value(4000 + Qt::Key_0).toInt();
        tmpSave.clear();
        tmpView.clear();
        tmpItem.clear();
        tmpParm.clear();
        QStringList testItems = tmpSet.value(r + ADDRITEM).toString().split(",");
        for (int i=0; i < testItems.size(); i++) {
            QString str = testItems.at(i);
            if (str.toInt() == 0x01) {  // 电阻测试
                initSetDCR();
            }
            if (str.toInt() == 0x02) {
                initSetMAG();
                initSetCCW();
            }
            if (str.toInt() == 0x03) {
                initSetINR();
            }
            if (str.toInt() == 0x04) {
                initSetACW();
            }
            if (str.toInt() == 0x05) {
                initSetDCW();
            }
            if (str.toInt() == 0x06) {
                initSetIMP();
            }
            if (str.toInt() == 0x07) {
                initSetPWR();
            }
            if (str.toInt() == 0x08) {
                initSetIND();
            }
            if (str.toInt() == 0x0B) {
                initSetHAL();
            }
            if (str.toInt() == 0x0C) {  // 负载
                initSetLOD();
            }
            if (str.toInt() == 0x0D) {
                initSetNLD();
            }
            if (str.toInt() == 0x0E) {
                initSetBMF();
            }
            if (str.toInt() == 0x0F) {
                initSetLVS();
            }
            if (str.toInt() == 0x10) {
                initSetACW2();
            }
        }
        mView->setRowCount(qMax(tmpItem.keys().size(), 11));
        for (int t=0; t < mView->rowCount(); t++) {
            for (int c=0; c < 4; c++) {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignCenter);
                if (c == 0)
                    item->setText(tmpItem.value(t).toString());
                if (c == 1)
                    item->setText(tmpParm.value(t).toString());
                mView->setItem(t, c, item);
            }
        }
        mView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        QStringList wireColor = tmpSet.value(r + ADDRWIRE).toString().split(",");
        for (int t=0; t < wireColor.size(); t++)
            colors.at(t)->setStyleSheet(QString("background-color:%1").arg(wireColor.at(t)));
    }

    if (Qt::Key_1) {
        btnTest->setVisible((mode == 4) ? false : true);
        QTmpMap tmpShow;
        tmpShow.insert(0x09, (mode <= 1) ? 1 : 0);  // 反嵌
        tmpShow.insert(0x0A, (mode <= 1) ? 1 : 0);  // 反嵌
        tmpShow.insert(0x0B, (mode <= 1 || mode == 4) ? 1 : 0);  // 匝间
        tmpShow.insert(0x0C, (mode <= 1 || mode == 4) ? 1 : 0);  // 匝间
        tmpShow.insert(0x0D, (mode <= 1 || mode == 4) ? 1 : 0);  // 匝间
        tmpShow.insert(0x0E, (mode <= 1 || mode == 4) ? 1 : 0);  // 匝间
        tmpShow.insert(0x0F, (mode >= 2 && mode != 4) ? 1 : 0);
        tmpShow.insert(0x10, (mode >= 2 && mode != 4) ? 1 : 0);
        tmpShow.insert(0x11, (mode >= 2) ? 1 : 0);
        tmpShow.insert(0x12, (mode >= 2) ? 1 : 0);
        tmpShow.insert(0x13, (mode >= 2) ? 1 : 0);
        tmpShow.insert(0x14, (mode >= 2) ? 1 : 0);
        QList<int> keys = tmpShow.keys();
        for (int i=0; i < keys.size(); i++) {
            int t = keys.at(i);
            int k = tmpShow.value(t).toInt();
            if (k == 1) {
                wView->showRow(t);
            } else {
                wView->hideRow(t);
            }
        }
    }
    tmpSave.insert(Qt::Key_0, Qt::Key_Save);
    tmpSave.insert(Qt::Key_1, "aip_tester");
    emit sendAppMsg(tmpSave);
    tmpSave.clear();
    isInit = true;
}

void AppTester:: initSetDCR()
{
    int row = 0;
    int save = tmpSet.value(3000 + Qt::Key_1).toInt();  // 电阻结果地址
    int addr = tmpSet.value(4000 + Qt::Key_1).toInt();  // 电阻配置地址
    double noun = tmpSet.value(addr + 2).toDouble();  // 不平衡度
    int temp = tmpSet.value(addr + 3).toInt();  // 温度折算
    for (int numb=0; numb < 8; numb++) {
        double test = tmpSet.value(addr + CACHEDCR + CACHEDCR*CHECKDCR + numb).toDouble();
        double from = tmpSet.value(addr + CACHEDCR + CACHEDCR*PORTDCR1 + numb).toDouble();
        double stop = tmpSet.value(addr + CACHEDCR + CACHEDCR*PORTDCR2 + numb).toDouble();
        double wire = tmpSet.value(addr + CACHEDCR + CACHEDCR*WIREDCR1 + numb).toDouble();
        double unit = tmpSet.value(addr + CACHEDCR + CACHEDCR*UNITDCR1 + numb).toDouble();
        double rmax = tmpSet.value(addr + CACHEDCR + CACHEDCR*UPPERDCR + numb).toDouble();
        double rmin = tmpSet.value(addr + CACHEDCR + CACHEDCR*LOWERDCR + numb).toDouble();
        if (test != 0) {
            QString ustr = (unit > 0) ? "Ω" : "mΩ";
            ustr = (unit > 1) ? "kΩ" : ustr;
            ustr.append(tr(" %1折算").arg((temp == 0 && wire != 3) ? tr("已") : tr("未")));
            QString item = tr("电阻%1-%2").arg(from).arg(stop);
            QString parm = tr("%1-%2%3").arg(rmin).arg(rmax).arg(ustr);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetDCR, row);
            row++;
            tmpSave.insert(save + row*0x10 + 0x00, item);  // 项目
            tmpSave.insert(save + row*0x10 + 0x01, parm);  // 参数
        }
    }
    if (noun != 0) {  // 不平衡度
        tmpItem.insert(tmpRow, tr("电阻平衡"));
        tmpParm.insert(tmpRow, tr("<%1%").arg(noun));
        insertItem(nSetDCR, 0x08);
    }
}

void AppTester::initSetMAG()
{
    int save = tmpSet.value(3000 + Qt::Key_2).toInt();  // 反嵌结果地址
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int nmag = tmpSet.value(back + backNMag).toInt();
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    QStringList testItems = tmpSet.value(conf + ADDRITEM).toString().split(",");
    bool isTest = testItems.contains(QString::number(nSetMAG));
    int addr = tmpSet.value(4000 + Qt::Key_2).toInt();  // 反嵌配置地址
    for (int numb=0; numb < 3; numb++) {
        int check = tmpSet.value(addr + CACHEMAG + CACHEMAG*CHECKMAG + numb).toInt();
        int portf = tmpSet.value(addr + CACHEMAG + CACHEMAG*PORTMAG1 + numb).toInt();
        int portt = tmpSet.value(addr + CACHEMAG + CACHEMAG*PORTMAG2 + numb).toInt();
        int upper = tmpSet.value(addr + CACHEMAG + CACHEMAG*UPPERMAG + numb).toInt();
        check = (isTest) ? check : 0;
        check = (nmag == 0) ? check : 0;
        QString str = (check != 0) ? strSW : strSG;
        QString item = tr("反嵌%1-%2").arg(portf).arg(portt);
        QString parm = tr("%1%").arg(upper);
        magText.at(numb*3 + 0)->setText(str.arg(tr("项目: ") + item));
        magText.at(numb*3 + 1)->setText(str.arg(tr("上限: ") + parm));
        magText.at(numb*3 + 2)->setText(str.arg(tr("差积: ----")));
        tmpSave.insert(save + numb*0x10 + 0x00, item);  // 项目
        tmpSave.insert(save + numb*0x10 + 0x01, parm);  // 参数
        magWave.at(numb)->setEnabled((check != 0) ? true : false);
        QVariantMap tmp;
        tmp.insert("index", 0);
        tmp.insert("color", int(Qt::white));
        magWave.at(numb)->setTexts(tmp);
        if (check != 0) {
            bool isShow = true;
            for (int i=0; i < tmpView.size(); i++) {
                QTmpMap tmp = tmpView.at(i);
                if (tmp.value(Qt::Key_1) == nSetMAG) {
                    isShow = false;
                }
            }
            if (isShow) {
                tmpItem.insert(tmpRow, tr("反嵌"));
                tmpParm.insert(tmpRow, parm);
                insertItem(nSetMAG, 0x00);
            }
        }
    }
}

void AppTester::initSetCCW()
{
    int addr = tmpSet.value(4000 + Qt::Key_2).toInt();  // 反嵌配置地址
    int turn = tmpSet.value(addr + 0).toInt();
    if (turn != 2) {
        tmpItem.insert(tmpRow, tr("转向"));
        tmpParm.insert(tmpRow, tr("%1").arg(turn == 0 ? tr("CW") : tr("CCW")));
        insertItem(nSetMAG, 0x08);
    }
}

void AppTester::initSetINR()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int vacu = tmpSet.value(back + backVacu).toInt();
    int save = tmpSet.value(3000 + Qt::Key_3).toInt();  // 绝缘结果地址
    int addr = tmpSet.value(4000 + Qt::Key_3).toInt();  // 绝缘配置地址

    if (vacu != 2) {  // 非真空/真空
        int numb = 4;
        double volt = tmpSet.value(addr + CACHEINR*VOLTINR1 + numb).toDouble();
        double smin = tmpSet.value(addr + CACHEINR*LOWERINR + numb).toDouble();
        double time = tmpSet.value(addr + CACHEINR*TIMEINR1 + numb).toDouble();
        volt = (volt == 0) ? 500 : 1000;
        QString item = tr("绝缘");
        QString parm = tr("%1V %2M %3s").arg(volt).arg(smin).arg(time);
        tmpItem.insert(tmpRow, item);
        tmpParm.insert(tmpRow, parm);
        insertItem(nSetINR, 0x00);
        tmpSave.insert(save + 0x00, item);  // 项目
        tmpSave.insert(save + 0x01, parm);  // 参数
    }
    if (vacu == 2) {  // 相间
        int row = 0;
        for (int i=0; i < 4; i++) {
            double test = tmpSet.value(addr + CACHEINR*CHECKINR + i).toDouble();
            double volt = tmpSet.value(addr + CACHEINR*VOLTINR1 + i).toDouble();
            double smin = tmpSet.value(addr + CACHEINR*LOWERINR + i).toDouble();
            double time = tmpSet.value(addr + CACHEINR*TIMEINR1 + i).toDouble();
            volt = (volt == 0) ? 500 : 1000;
            if (test == 1) {
                QString item = tr("绝缘%1").arg(row+1);
                QString parm = tr("%1V %2M %3s").arg(volt).arg(smin).arg(time);
                tmpItem.insert(tmpRow, item);
                tmpParm.insert(tmpRow, parm);
                insertItem(nSetINR, row);
                row++;
                tmpSave.insert(save + row*0x10 + 0x00, item);  // 项目
                tmpSave.insert(save + row*0x10 + 0x01, parm);  // 参数
            }
        }
    }
}

void AppTester::initSetACW()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int vacu = tmpSet.value(back + backVacu).toInt();
    int save = tmpSet.value(3000 + Qt::Key_4).toInt();  // 交耐结果地址
    int addr = tmpSet.value(4000 + Qt::Key_4).toInt();  // 交耐配置地址
    if (vacu != 2) {  // 非真空/真空
        int numb = 4;
        double volt = tmpSet.value(addr + CACHEACW + CACHEACW*VOLTACW1 + numb).toDouble();
        double smax = tmpSet.value(addr + CACHEACW + CACHEACW*UPPERACW + numb).toDouble();
        double smin = tmpSet.value(addr + CACHEACW + CACHEACW*LOWERACW + numb).toDouble();
        double time = tmpSet.value(addr + CACHEACW + CACHEACW*TIMEACW1 + numb).toDouble();
        double last = tmpSet.value(addr + CACHEACW + CACHEACW*ADDRACWA + numb).toDouble();
        QString item = tr("交耐");
        QString parm = tr("%1V %2-%3mA %4s").arg(volt).arg(smin).arg(smax).arg(time);
        parm.append(last == 0 ? "" : tr(" ARC:%1").arg(last));
        tmpItem.insert(tmpRow, item);
        tmpParm.insert(tmpRow, parm);
        insertItem(nSetACW, 0x00);
        tmpSave.insert(save + 0x00, item);  // 项目
        tmpSave.insert(save + 0x01, parm);  // 参数
    }
    if (vacu == 2) {  // 相间
        int row = 0;
        for (int i=0; i < 4; i++) {
            double test = tmpSet.value(addr + CACHEACW + CACHEACW*CHECKACW + i).toDouble();
            double volt = tmpSet.value(addr + CACHEACW + CACHEACW*VOLTACW1 + i).toDouble();
            double smax = tmpSet.value(addr + CACHEACW + CACHEACW*UPPERACW + i).toDouble();
            double smin = tmpSet.value(addr + CACHEACW + CACHEACW*LOWERACW + i).toDouble();
            double time = tmpSet.value(addr + CACHEACW + CACHEACW*TIMEACW1 + i).toDouble();
            double last = tmpSet.value(addr + CACHEACW + CACHEACW*ADDRACWA + i).toDouble();
            if (test == 1) {
                QString item = tr("交耐%1").arg(row + 1);
                QString parm = tr("%1V %2-%3mA %4s").arg(volt).arg(smin).arg(smax).arg(time);
                parm.append(last == 0 ? "" : tr(" ARC:%1").arg(last));
                tmpItem.insert(tmpRow, item);
                tmpParm.insert(tmpRow, parm);
                insertItem(nSetACW, row);
                row++;
                tmpSave.insert(save + row*0x10 + 0x00, item);  // 项目
                tmpSave.insert(save + row*0x10 + 0x01, parm);  // 参数
            }
        }
    }
}

void AppTester::initSetACW2()
{
    int save = tmpSet.value(3000 + Qt::Key_4).toInt() + 0x10;  // 交耐结果地址
    int addr = tmpSet.value(4000 + Qt::Key_4).toInt();  // 交耐配置地址
    int numb = 5;
    double volt = tmpSet.value(addr + CACHEACW + CACHEACW*VOLTACW1 + numb).toDouble();
    double smax = tmpSet.value(addr + CACHEACW + CACHEACW*UPPERACW + numb).toDouble();
    double smin = tmpSet.value(addr + CACHEACW + CACHEACW*LOWERACW + numb).toDouble();
    double time = tmpSet.value(addr + CACHEACW + CACHEACW*TIMEACW1 + numb).toDouble();
    double last = tmpSet.value(addr + CACHEACW + CACHEACW*ADDRACWA + numb).toDouble();
    if (1) {
        QString item = tr("交耐2");
        QString parm = tr("%1V %2-%3mA %4s").arg(volt).arg(smin).arg(smax).arg(time);
        parm.append(last == 0 ? "" : tr(" ARC:%1").arg(last));
        tmpItem.insert(tmpRow, item);
        tmpParm.insert(tmpRow, parm);
        insertItem(0x10, 0x00);
        tmpSave.insert(save + 0x00, item);  // 项目
        tmpSave.insert(save + 0x01, parm);  // 参数
    }
}

void AppTester::initSetDCW()
{
    int save = tmpSet.value(3000 + Qt::Key_5).toInt();  // 直耐结果地址
    int addr = tmpSet.value(4000 + Qt::Key_5).toInt();  // 直耐配置地址
    int numb = 4;
    double volt = tmpSet.value(addr + CACHEACW + CACHEACW*VOLTACW1 + numb).toDouble();
    double smax = tmpSet.value(addr + CACHEACW + CACHEACW*UPPERACW + numb).toDouble();
    double smin = tmpSet.value(addr + CACHEACW + CACHEACW*LOWERACW + numb).toDouble();
    double time = tmpSet.value(addr + CACHEACW + CACHEACW*TIMEACW1 + numb).toDouble();
    if (1) {
        QString item = tr("直耐");
        QString parm = tr("%1V %2-%3mA %4s").arg(volt).arg(smin).arg(smax).arg(time);
        tmpItem.insert(tmpRow, item);
        tmpParm.insert(tmpRow, parm);
        insertItem(nSetDCW, 0x00);
        tmpSave.insert(save + 0x00, item);  // 项目
        tmpSave.insert(save + 0x01, parm);  // 参数
    }
}

void AppTester::initSetIMP()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int grnd = tmpSet.value(back + backGrnd).toInt();
    int save = tmpSet.value(3000 + Qt::Key_6).toInt();  // 匝间结果地址
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    QStringList testItems = tmpSet.value(conf + ADDRITEM).toString().split(",");
    bool isTest = testItems.contains(QString::number(nSetIMP));
    int addr = tmpSet.value(4000 + Qt::Key_6).toInt();  // 匝间配置地址
    for (int i=0; i < 6; i++) {
        int check = tmpSet.value(addr + CACHEIMP + CACHEIMP*CHECKIMP + i).toInt();
        double portf = tmpSet.value(addr + CACHEIMP + CACHEIMP*PORTIMP1 + i).toDouble();
        double portt = tmpSet.value(addr + CACHEIMP + CACHEIMP*PORTIMP2 + i).toDouble();
        double volts = tmpSet.value(addr + CACHEIMP + CACHEIMP*VOLTIMP1 + i).toDouble();
        double area1 = tmpSet.value(addr + CACHEIMP + CACHEIMP*AREAIMP1 + i).toDouble();
        double diff1 = tmpSet.value(addr + CACHEIMP + CACHEIMP*DIFFIMP1 + i).toDouble();
        double sflut = tmpSet.value(addr + CACHEIMP + CACHEIMP*FLUTIMP1 + i).toDouble();
        double phase = tmpSet.value(addr + CACHEIMP + CACHEIMP*PHSEIMP1 + i).toDouble();
        check = (isTest && (grnd != 1)) ? check : 0;
        QString str = (check != 0) ? strSW : strSG;
        QString item = tr("匝间%1-%2").arg(portf).arg(portt);
        impText.at(i*5 + 0)->setText(str.arg(tr("项目: 匝间%1-%2").arg(portf).arg(portt)));
        impText.at(i*5 + 1)->setText(str.arg(tr("面积: ----")));
        str = (diff1 != 0) ? str : strSG;
        impText.at(i*5 + 2)->setText(str.arg(tr("差积: ----")));
        str = (sflut != 0) ? str : strSG;
        impText.at(i*5 + 3)->setText(str.arg(tr("电晕: ----")));
        str = (phase != 0) ? str : strSG;
        impText.at(i*5 + 4)->setText(str.arg(tr("相位: ----")));
        impWave.at(i)->setEnabled((check != 0) ? true : false);
        QVariantMap tmp;
        tmp.insert("index", 0);
        tmp.insert("color", int(Qt::white));
        tmp.insert("width", 80);
        tmp.insert("lenth", 85);
        tmp.insert("title", tr("%1V").arg(volts));
        impWave.at(i)->setTexts(tmp);
        impWave.at(i)->update();
        allWave->setTexts(tmp);
        allWave->update();
        if (check != 0) {
            tmpSave.insert(save + i*0x10 + 0x00, item);  // 项目
            if (area1 > 0)
                tmpSave.insert(save + i*0x10 + 0x01, tr("%1").arg(area1));  // 项目
            if (diff1 > 0)
                tmpSave.insert(save + i*0x10 + 0x04, tr("%1").arg(diff1));  // 项目
            if (sflut > 0)
                tmpSave.insert(save + i*0x10 + 0x07, tr("%1").arg(sflut));  // 项目
            if (phase > 0)
                tmpSave.insert(save + i*0x10 + 0x0A, tr("%1").arg(phase));  // 项目
        }
        if (check != 0 || grnd == 1) {
            bool isShow = true;
            for (int i=0; i < tmpView.size(); i++) {
                QTmpMap tmp = tmpView.at(i);
                if (tmp.value(Qt::Key_1) == nSetIMP) {
                    isShow = false;
                }
            }
            if (isShow) {
                if (grnd == 1) {
                    tmpItem.insert(tmpRow, tr("单点破损"));
                    tmpParm.insert(tmpRow, tr("0~5mA"));
                } else {
                    tmpItem.insert(tmpRow, tr("匝间"));
                    tmpParm.insert(tmpRow, tr("%1% %2%").arg(area1).arg(diff1));
                }
                insertItem(nSetIMP, 0x00);
            }
        }
    }
}

void AppTester::initSetPWR()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int nmag = tmpSet.value(back + backNMag).toInt();

    int real = tmpSet.value(3000 + Qt::Key_7).toInt();  // 霍尔结果地址
    int conf = tmpSet.value(4000 + Qt::Key_7).toInt();  // 霍尔配置地址
    QStringList items;
    items << tr("电流") << tr("功率") << tr("转速") << tr("转向");
    QStringList units;
    units << "A" << "W" << "rpm" << "";
    for (int i=0; i < PWR_SIZE; i++) {
        double test = tmpSet.value(conf + CACHEPWR + CACHEPWR*CHECKPWR + i).toDouble();
        double imax = tmpSet.value(conf + CACHEPWR + CACHEPWR*CMAXPWR1 + i).toDouble();
        double imin = tmpSet.value(conf + CACHEPWR + CACHEPWR*CMINPWR1 + i).toDouble();
        double pmax = tmpSet.value(conf + CACHEPWR + CACHEPWR*PMAXPWR1 + i).toDouble();
        double pmin = tmpSet.value(conf + CACHEPWR + CACHEPWR*PMINPWR1 + i).toDouble();
        double rmax = tmpSet.value(conf + CACHEPWR + CACHEPWR*VMAXPWR1 + i).toDouble();
        double rmin = tmpSet.value(conf + CACHEPWR + CACHEPWR*VMINPWR1 + i).toDouble();
        double tmax = tmpSet.value(conf + CACHEPWR + CACHEPWR*TURNPWR1 + i).toDouble();

        if (test != 0 && imax != 0 && nmag == 0) {
            QString item = tr("电参%1").arg(i+1) + tr("电流");
            QString parm = tr("%1-%2A").arg(imin).arg(imax);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetPWR + 100 * i, 0x00);
            tmpSave.insert(real+i*0x10+0x00*3+0, ((i == 0) ? tr("低速") : tr("高速")));
            tmpSave.insert(real+i*0x10+0x00*3+1, parm);
        }
        if (test != 0 && pmax != 0 && nmag == 0) {
            QString item = tr("电参%1").arg(i+1) +  tr("功率");
            QString parm = tr("%1-%2W").arg(pmin).arg(pmax);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetPWR + 100 * i, 0x01);
            tmpSave.insert(real + i*0x10 + 0x01*3 + 1, parm);
        }
        if (test != 0 && rmax != 0 && nmag == 0) {
            QString item = tr("电参%1").arg(i+1) +  tr("容压");
            QString parm = tr("%1-%2rpm").arg(rmin).arg(rmax);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetPWR + 100 * i, 0x02);
            tmpSave.insert(real + i*0x10 + 0x02*3 + 1, parm);
        }
        if (test != 0 && tmax != 0) {
            QString item = tr("电参%1").arg(i+1) +  tr("转向");
            QString parm = (tmax == 1) ? tr("CCW") : tr("CW");
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetPWR + 100 * i, 0x03);
            tmpSave.insert(real + i*0x10 + 0x03*3 + 1, parm);
        }
    }
}

void AppTester::initSetIND()
{
    int row = 0;
    int save = tmpSet.value(3000 + Qt::Key_8).toInt();  // 电感结果地址
    int addr = tmpSet.value(4000 + Qt::Key_8).toInt();  // 电感配置地址
    double noun = tmpSet.value(addr + 0).toDouble();  // 不平衡度
    for (int numb=0; numb < 8; numb++) {
        double test = tmpSet.value(addr + CACHEIND + CACHEIND*CHECKIND + numb).toDouble();
        double from = tmpSet.value(addr + CACHEIND + CACHEIND*PORTIND1 + numb).toDouble();
        double stop = tmpSet.value(addr + CACHEIND + CACHEIND*PORTIND2 + numb).toDouble();
        double unit = tmpSet.value(addr + CACHEIND + CACHEIND*UNITIND1 + numb).toDouble();
        double rmax = tmpSet.value(addr + CACHEIND + CACHEIND*UPPERIND + numb).toDouble();
        double rmin = tmpSet.value(addr + CACHEIND + CACHEIND*LOWERIND + numb).toDouble();
        if (test != 0) {
            QString ustr = (unit > 0) ? "mH" : "uH";
            QString item = tr("电感%1-%2").arg(from).arg(stop);
            QString parm = tr("%1-%2%3").arg(rmin).arg(rmax).arg(ustr);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetIND, row);
            row++;
            tmpSave.insert(save + row*0x10 + 0x00, item);  // 项目
            tmpSave.insert(save + row*0x10 + 0x01, parm);  // 参数
        }
    }
    if (noun != 0) {  // 不平衡度
        tmpItem.insert(tmpRow, tr("电感平衡"));
        tmpParm.insert(tmpRow, tr("<%1%").arg(noun));
        insertItem(nSetIND, 0x08);
    }
}

void AppTester::initSetHAL()
{
    int real = tmpSet.value(3000 + Qt::Key_B).toInt();  // 霍尔结果地址
    int addr = tmpSet.value(4000 + Qt::Key_B).toInt();  // 霍尔配置地址
    QStringList items;
    items << tr("霍尔高电平") << tr("霍尔低电平") << tr("霍尔占空比") << tr("霍尔频率");
    QStringList units;
    units << "V" << "V" << "%" << "Hz";
    for (int numb=0; numb < items.size(); numb++) {
        double max = tmpSet.value(addr + numb*2 + 0).toDouble();
        double min = tmpSet.value(addr + numb*2 + 1).toDouble();
        if (max != 0) {
            QString item = items.at(numb);
            QString parm = tr("%1-%2").arg(min).arg(max) + units.at(numb);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetHAL, numb);
            tmpSave.insert(real + numb*0x10, parm);
        }
    }
}

void AppTester::initSetLOD()
{
    int real = tmpSet.value(3000 + Qt::Key_C).toInt();  // 负载结果地址
    int addr = tmpSet.value(4000 + Qt::Key_C).toInt();  // 负载配置地址
    QStringList items;
    items << tr("负载电流") << tr("负载功率") << tr("Icc电流") << tr("负载转速") << tr("负载转向");
    QStringList units;
    units << "mA" << "W" << "mA" << "rpm";

    for (int numb=0; numb < 5; numb++) {
        double max = tmpSet.value(addr + numb*2 + 0).toDouble();
        double min = tmpSet.value(addr + numb*2 + 1).toDouble();
        if (max != 0) {
            QString item = items.at(numb);
            QString parm = tr("%1").arg(max == 1 ? "CCW" : "CW");
            parm = (numb < units.size() ? tr("%1-%2").arg(min).arg(max) + units.at(numb) : parm);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetLOD, numb);
            tmpSave.insert(real + numb*0x10, parm);
        }
    }
}

void AppTester::initSetNLD()
{
    int real = tmpSet.value(3000 + Qt::Key_D).toInt();  // 空载结果地址
    int addr = tmpSet.value(4000 + Qt::Key_D).toInt();  // 空载配置地址
    QStringList items;
    items << tr("空载电流") << tr("空载功率") << tr("Icc电流") << tr("空载转速") << tr("空载转向");
    QStringList units;
    units << "mA" << "W" << "mA" << "rpm";

    for (int numb=0; numb < 5; numb++) {
        double max = tmpSet.value(addr + numb*2 + 0).toDouble();
        double min = tmpSet.value(addr + numb*2 + 1).toDouble();
        if (max != 0) {
            QString item = items.at(numb);
            QString parm = tr("%1").arg(max == 1 ? "CCW" : "CW");
            parm = (numb < units.size() ? tr("%1-%2").arg(min).arg(max) + units.at(numb) : parm);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetNLD, numb);
            tmpSave.insert(real + numb*0x10, parm);
        }
    }
}

void AppTester::initSetBMF()
{
    int real = tmpSet.value(3000 + Qt::Key_E).toInt();  // 反电动势结果地址
    int addr = tmpSet.value(4000 + Qt::Key_E).toInt();  // 反电动势配置地址
    QStringList items;
    items << tr("反电动势电压") << tr("反电动势常数") << tr("与霍尔相位差")
          << tr("三相不平衡度") << tr("三相相序");
    QStringList units;
    units << "V" << "V/krmp" << "°" << "%";

    for (int numb=0; numb < 5; numb++) {
        double max = tmpSet.value(addr + numb*2 + 0).toDouble();
        double min = tmpSet.value(addr + numb*2 + 1).toDouble();
        if (max != 0) {
            QString item = items.at(numb);
            QString parm = tr("%1").arg((max == 1) ? "ABC" : "ACB");
            parm = (numb < units.size() ? tr("%1-%2").arg(min).arg(max) + units.at(numb) : parm);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetEMF, numb);
            tmpSave.insert(real + numb*0x10, parm);
        }
    }
}

void AppTester::initSetLVS()
{
    int real = tmpSet.value(3000 + Qt::Key_F).toInt();  // 低启结果地址
    int addr = tmpSet.value(4000 + Qt::Key_F).toInt();  // 低启配置地址
    QStringList items;
    items << tr("低启电流") << tr("低启功率") << tr("Icc电流") << tr("低启转速") << tr("低启转向");
    QStringList units;
    units << "mA" << "W" << "mA" << "rpm";

    for (int numb=0; numb < 5; numb++) {
        double max = tmpSet.value(addr + numb*2 + 0).toDouble();
        double min = tmpSet.value(addr + numb*2 + 1).toDouble();
        if (max != 0) {
            QString item = items.at(numb);
            QString parm = tr("%1").arg(max == 1 ? "CCW" : "CW");
            parm = (numb < units.size() ? tr("%1-%2").arg(min).arg(max) + units.at(numb) : parm);
            tmpItem.insert(tmpRow, item);
            tmpParm.insert(tmpRow, parm);
            insertItem(nSetLPH, numb);
            tmpSave.insert(real + numb*0x10, parm);
        }
    }
}

void AppTester::insertItem(int item, int numb)
{
    QTmpMap tmp;
    tmp.insert(Qt::Key_0, tmpRow);
    tmp.insert(Qt::Key_1, item);
    tmp.insert(Qt::Key_2, numb);
    tmpView.append(tmp);
    tmp.clear();
    tmpRow++;
}

void AppTester::initQuality()
{
    codeText->setText(strMY.arg(tr("当前编码:")));
    QStringList itemNams;
    itemNams << "电阻" << "反嵌" << "绝缘" << "交耐" << "直耐"
             << "匝间" << "电参" << "电感" << "堵转" << "低启"
             << "霍尔" << "负载" << "空载" << "BEMF"  << "缺相";

    int r = tmpSet.value(1000 + Qt::Key_0).toInt() + 0x10;  // 可用项目地址
    for (int i=0; i < itemNams.size(); i++) {   // 可用项目
        int stat = tmpSet.value(r + i).toInt();
        if (stat != 0) {
            bool n = true;
            for (int t=0; t < tmpQua.size(); t++) {
                QVariantMap tmp = tmpQua.at(t);
                if (tmp.value("index").toInt() == i + 1) {
                    if (tmpNG.value(i + 1).toInt() != 0) {
                        tmp.insert("point", tmp.value("point").toInt() + 1);
                        tmpQua[t] = tmp;
                    }
                    n = false;
                    break;
                }
            }
            if (n) {
                QVariantMap tmp;
                tmp.insert("index", i+1);
                tmp.insert("names", itemNams.at(i));
                tmp.insert("point", 0);
                tmpQua.append(tmp);
            }
        }
    }
    for (int i=0; i < tmpQua.size(); i++) {
        QVariantMap tmp = tmpQua.at(i);
        if (tmp.value("index").toInt() == 0) {
            tmp.insert("point", testQu);
            tmpQua[i] = tmp;
        }
        if (tmp.value("index").toInt() == 0xFF) {
            tmp.insert("point", testOK);
            tmpQua[i] = tmp;
        }
    }

    for (int i=0; i < tmpQua.size(); i++) {
        bodys->setBodys(tmpQua.at(i));
    }
    bodys->update();
}

void AppTester::clickType()
{
    if (isInit) {
        this->setFocus();
        btnTest->setEnabled(false);
        int row = typeText->currentIndex();
        QString name = typeText->currentText();
        QString numb = tmpTyp.value(name).toString();
        if (tmpSet.value(DataType).toString() != name) {
            if (row < tmpBuf.value("type").toInt()) {
                QString str = tr("型号更改为%1?").arg(name);
                int ret = QMessageBox::warning(this, "", str, QMessageBox::Ok | QMessageBox::No);
                if (ret == QMessageBox::Ok) {
                    tmpMsg.insert(DataFile, numb);
                    tmpMsg.insert(DataType, name);
                    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
                    tmpMsg.insert(Qt::Key_1, "aip_reload");
                    emit sendAppMsg(tmpMsg);
                    tmpMsg.clear();
                    QTimer::singleShot(2000, this, SLOT(updateTest()));
                    initSettings();
                } else {
                    updateType();
                }
            } else {
                QMessageBox::warning(this, "", tr("型号不存在"), QMessageBox::Ok);
                updateType();
            }
        }
    }
}

void AppTester::clickTest()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Play);
    tmpMsg.insert(Qt::Key_1, btnL->isChecked() ? "L" : "R");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppTester::clickStop()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Stop);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppTester::clickWave()
{
    BoxQImage *ww = qobject_cast<BoxQImage*>(sender());
    QList<QVariantMap> lines = ww->getLines();
    for (int i=0; i < lines.size(); i++) {
        allWave->setLines(lines.at(i));
    }
    QList<QVariantMap> texts = ww->getTexts();
    for (int i=0; i < texts.size(); i++) {
        allWave->setTexts(texts.at(i));
    }
    updateWave();
}

void AppTester::clickReset()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Stop);
    tmpMsg.insert(Qt::Key_1, "reset");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppTester::clickButton()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_View);
    tmpMsg.insert(Qt::Key_1, sender()->objectName());
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppTester::updateType()
{
    isInit = false;
    QSqlQuery query(QSqlDatabase::database("config"));
    query.exec("select name from sqlite_master where type='table' order by name");
    typeText->clear();
    tmpTyp.clear();
    while (query.next()) {
        QString t = query.value(0).toString();
        QString numb = t.mid(1, 4);
        QString name = t.mid(6, 50);
        tmpTyp.insert(name, numb);
        typeText->addItem(name);
    }
    tmpBuf.insert("type", typeText->count());
    typeText->setCurrentText(tmpSet.value(DataType).toString());
    isInit = true;
}

void AppTester::updateWave()
{
    int w = 0;
    for (int i=0; i < 4; i++) {
        w += wView->columnWidth(i);
    }
    int h = 0;
    for (int i=0; i < 6; i++) {
        h += wView->rowHeight(i + 9);
    }
    int x = wView->x() + wView->parentWidget()->x() + 1;
    int y = wView->y() + wView->parentWidget()->y() + wView->height() - h;
    allWave->resize(w-18, h-17);
    allWave->move(x+9, y+7);
    allWave->update();
    allWave->show();
}

void AppTester::updateShow()
{
    initSetMAG();
    initSetIMP();
    tmpMap.insert("width", 2);
    tmpMap.insert("frame", 1);
    tmpMap.insert("shade", 1);
    for (int i=0; i < magWave.size(); i++) {
        for (int index=0; index < 2; index++) {
            tmpMap.insert("index", index);
            magWave.at(i)->setLines(tmpMap);
            allWave->setLines(tmpMap);
        }
        magWave.at(i)->update();
    }
    allWave->update();
    for (int i=0; i < impWave.size(); i++) {
        for (int index=0; index < 2; index++) {
            tmpMap.insert("index", index);
            impWave.at(i)->setLines(tmpMap);
        }
        impWave.at(i)->update();
    }
    for (int i=0; i < 3; i++) {
        tmpMap.insert("index", i);
        tmpMap.insert("frame", 0);
        bWave->setLines(tmpMap);
        bWave->update();
    }
    for (int i=3; i < 8; i++) {
        tmpMap.insert("index", i);
        tmpMap.insert("shade", 0);
        hWave->setLines(tmpMap);
        hWave->update();
    }
    for (int i=0; i < mView->rowCount(); i++) {
        mView->item(i, 2)->setText("");
        mView->item(i, 3)->setText("");
    }
    testText->setText(strSW.arg(tr("测试时间:____")));
    tmpMap.clear();
}

void AppTester::updateTime()
{
    if (tmpTime != 0) {
        QString tt = QString::number((ms.elapsed() - tmpTime)/1000.0, 'f', 1);
        testText->setText(strSW.arg(tr("测试时间:%1s").arg(tt)));
    }
    QString strA = tr("当前日期:") + QDate::currentDate().toString("yy-MM-dd");
    dateText->setText(strSW.arg(strA));
    QString strB = tr("当前时间:") + QTime::currentTime().toString("hh:mm:ss");
    timeText->setText(strSW.arg(strB));
}

void AppTester::updateTest()
{
    btnTest->setEnabled(true);
    typeText->setEnabled(true);

    //    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    //    int grnd = tmpSet.value(back + backGrnd).toInt();
    //    if (grnd == 1)
    //        impWave.at(0)->clicked();
}

void AppTester::recvErrMsg(QTmpMap msg)
{
    QString err = msg.value(Qt::Key_1).toString();
    QStringList pp = err.split("\n");
    QString str;
    for (int i=0; i < pp.size(); i++) {
        str.append(strSY.arg(pp.at(i)));
    }
    str.append("<br><\br>");
    warnText->setText(str);
}

void AppTester::recvTmpMsg(QTmpMap msg)
{
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    if (!msg.value(addr + TEMPTEMP).isNull()) {
        QString temp = tr("当前") + msg.value(addr + TEMPTEMP).toString();
        tempText->setText(strSW.arg(temp));
    }
}

void AppTester::recvLedMsg(QTmpMap msg)
{
    if (msg.value(Qt::Key_2).isNull())
        return;
    int c = msg.value(Qt::Key_2).toInt();
    if (c == DATAON) {
        updateShow();
        tmpNG.clear();
        tmpTime = ms.elapsed();
        realText->setText(strLY.arg(tr("测试")));
        int work = msg.value(Qt::Key_4).toInt();
        btnL->setChecked((work == WORKL) ? true : false);
        btnR->setChecked((work == WORKR) ? true : false);
        int back = tmpSet.value(1000 + Qt::Key_0).toInt();
        if (tmpSet.value(back + backWork).toInt() != 0)
            workText->setText(strLY.arg((work == WORKR) ? tr("右") : tr("左")));
        btnHome->setEnabled(false);
        btnConf->setEnabled(false);
        btnTest->setEnabled(false);
        warnText->clear();
    }
    if (c == DATAOK || c == DATANG || c == DATADC) {
        QString str = strLG.arg("OK");
        testQu = (c == DATADC) ? testQu : testQu + 1;
        testOK = (c != DATAOK) ? testOK : testOK + 1;
        str = (c == DATADC) ? strLR.arg(tr("中断")) : str;
        str = (c == DATANG) ? strLR.arg(tr("NG")) : str;
        realText->setText(str);
        int t = ms.elapsed() - tmpTime;
        QString tt = QString::number(t/1000.0, 'f', 1);
        testText->setText(strSW.arg(tr("测试时间:%1s").arg(tt)));
        if (c == DATADC)
            tmpNG.clear();
        else
            initQuality();
        btnHome->setEnabled(true);
        btnConf->setEnabled(true);
        btnTest->setEnabled(true);
        tmpTime = 0;
    }
}

void AppTester::recvFGWave(QTmpMap msg)
{
    QString w = msg.value(Qt::Key_2).toString();
    QStringList ws = w.split(" ");
    if (ws.size() < 200)
        return;
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int wbmf = tmpSet.value(back + backWave).toInt();
    wbmf = (wbmf == 0) ? 1 : wbmf;
    int index = ws.at(0).toInt();
    ws.removeFirst();
    QList<int> cc;
    cc << int(Qt::cyan) << int(Qt::yellow) << int(Qt::green);
    QStringList mPoint;
    for (int t=0; t < ws.size(); t++) {
        double p1 = ((ws.at(t).toInt() - 128) * wbmf + 128) / 2.56;  // 反电动势波形
        double p2 = ws.at(t).toInt() * 30 / 256 + 16 * ((5 - index) * 2 + 1) - 10;  // 空载/负载波形
        mPoint.append(QString::number((index < 3) ? p1 : p2));
    }
    tmpMap.insert("index", index);
    tmpMap.insert("frame", 0);
    tmpMap.insert("color", cc.at(index%cc.size()));
    tmpMap.insert("point", mPoint);
    BoxQImage *ww = (index < 3) ? bWave : hWave;
    ww->setLines(tmpMap);
    ww->update();
    tmpMap.clear();
}

void AppTester::recvManual(QTmpMap msg)
{  // 手动状态下匝间波形
    QString str = msg.value(Qt::Key_5).toString();
    QStringList ws = str.split(" ");
    if (ws.size() > 200) {
        int numb = ws.at(0).toInt();
        ws.removeFirst();
        int addr = tmpSet.value(4000 + Qt::Key_6).toInt() + CACHEIMP;  // 匝间配置地址
        int from = tmpSet.value(addr + CACHEIMP*FROMIMP1 + numb).toInt();
        int stop = tmpSet.value(addr + CACHEIMP*STOPIMP1 + numb).toInt();
        QStringList mPoint;
        for (int i=from; i < qMin(stop, ws.size()); i++) {
            double p = ws.at(i).toInt() * 100 / 1024;
            mPoint.append(QString::number(p));
        }
        tmpMap.insert("index", 1);  // 清测试波形
        tmpMap.insert("color", int(Qt::green));
        impWave.at(numb)->setLines(tmpMap);
        allWave->setLines(tmpMap);

        tmpMap.insert("index", 0);  // 显示波形
        tmpMap.insert("point", mPoint);
        impWave.at(numb)->setLines(tmpMap);
        allWave->setLines(tmpMap);

        tmpMap.insert("index", 0);  // 显示电压
        tmpMap.insert("color", int(Qt::white));
        tmpMap.insert("width", 85);
        tmpMap.insert("lenth", 85);
        tmpMap.insert("title", tr("%1V").arg(strv));
        impWave.at(0)->setTexts(tmpMap);
        allWave->setTexts(tmpMap);

        impWave.at(0)->update();
        allWave->update();
    }
}

void AppTester::recvNewMsg(QTmpMap msg)
{
    tmpMap.insert("width", 2);
    tmpMap.insert("frame", 1);
    tmpMap.insert("shade", 1);
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int grnd = tmpSet.value(back + backGrnd).toInt();
    int nmag = tmpSet.value(back + backNMag).toInt();
    int item = msg.value(Qt::Key_1).toInt();  // 测试项目
    int numb = msg.value(Qt::Key_2).toInt();  // 单项数量
    int work = msg.value(Qt::Key_6).toInt();  // 工位
    if (item == nSetMAG && numb < 3 && nmag == 0) {  // 更新反嵌结果
        if (!msg.value(Qt::Key_3).isNull()) {
            QString str = msg.value(Qt::Key_3).toString();
            magText.at(numb*3 + 2)->setText(strSW.arg(tr("差积: ") + str));
        }
        if (!msg.value(Qt::Key_4).isNull()) {
            QString str = msg.value(Qt::Key_4).toString();
            QStringList ws = tmpStr.split(" ");
            if (ws.size() > 200) {
                ws.removeFirst();
                QStringList mPoint;
                for (int i=0; i < qMin(MAG_SIZE, ws.size()); i++) {
                    double p  = ws.at(i).toInt() * 100 / 256;
                    mPoint.append(QString::number(p));
                }
                tmpMap.insert("index", 0);
                tmpMap.insert("color", str == "OK" ? int(Qt::green) : int(Qt::red));
                tmpMap.insert("point", mPoint);
                magWave.at(numb)->setLines(tmpMap);
                magWave.at(numb)->update();
            }
            if (str != "OK")
                tmpNG.insert(item, "1");
        }
        if (!msg.value(Qt::Key_5).isNull()) {
            tmpStr = msg.value(Qt::Key_5).toString();
        }
    }
    if ((item == nSetIMP || item == 6105) && numb < 6 && grnd != 1) {  // 更新匝间结果
        if (!msg.value(Qt::Key_3).isNull()) {
            QString str = msg.value(Qt::Key_3).toString();
            QStringList imp = str.split(",");
            for (int i=0; i < imp.size(); i++) {
                QStringList pp = QString(imp.at(i)).split(":");
                if (i == 0)
                    impText.at(numb*5 + 1 + i)->setText(strSW.arg(tr("面积: ") + pp.last()));
                if (i == 1)
                    impText.at(numb*5 + 1 + i)->setText(strSW.arg(tr("差积: ") + pp.last()));
                if (i == 2)
                    impText.at(numb*5 + 1 + i)->setText(strSW.arg(tr("电晕: ") + pp.last()));
                if (i == 3)
                    impText.at(numb*5 + 1 + i)->setText(strSW.arg(tr("相位: ") + pp.last()));
            }
        }
        if (item == 6105) {
            numb = msg.value(Qt::Key_5).toInt() % 6;
            QStringList tmp;
            tmp.append(QString::number(numb));
            for (int i=0; i < 400; i++) {
                tmp.append(QString::number(512));
            }
            tmpStr = tmp.join(" ");

            msg.insert(Qt::Key_4, "NG");
        }
        if (!msg.value(Qt::Key_4).isNull()) {
            QString str = msg.value(Qt::Key_4).toString();
            QStringList ws = tmpStr.split(" ");
            if (ws.size() > 200) {
                ws.removeFirst();
                int addr = tmpSet.value(4000 + Qt::Key_6).toInt() + CACHEIMP;  // 匝间配置地址
                int from = tmpSet.value(addr + CACHEIMP*FROMIMP1 + numb).toInt();
                int stop = tmpSet.value(addr + CACHEIMP*STOPIMP1 + numb).toInt();
                int wimp = tmpSet.value(4000 + Qt::Key_H).toInt();  // 匝间标准波形地址
                QStringList mp1, mp0;
                int ss = (work == 0x13) ? 0 : 1;
                for (int i=from; i < qMin(stop, ws.size()); i++) {
                    double p1 = ws.at(i).toInt() * 100.0 / 1024;
                    mp1.append(QString::number(p1));
                    double p2 = tmpSet.value(wimp + IMP_SIZE*(numb*2 + ss) + i).toInt()*100.0/1024;
                    mp0.append(QString::number(p2));
                }
                tmpMap.insert("index", 0);
                tmpMap.insert("color", int(Qt::white));
                tmpMap.insert("point", mp0);
                impWave.at(numb)->setLines(tmpMap);
                allWave->setLines(tmpMap);
                tmpMap.insert("index", 1);
                tmpMap.insert("point", mp1);
                tmpMap.insert("color", str == "OK" ? int(Qt::green) : int(Qt::red));
                impWave.at(numb)->setLines(tmpMap);
                allWave->setLines(tmpMap);
                impWave.at(numb)->update();
                allWave->update();
            }
            if (str != "OK")
                tmpNG.insert(item, "1");
        }
        if (!msg.value(Qt::Key_5).isNull()) {
            tmpStr = msg.value(Qt::Key_5).toString();
        }
    }
    if (item <= 0x10) {
        for (int i=0; i < tmpView.size(); i++) {
            QTmpMap tmp = tmpView.at(i);
            int pn = tmp.value(Qt::Key_1).toInt();
            int pc = tmp.value(Qt::Key_2).toInt();
            int pt = (((item == nSetMAG) || (item == nSetIMP)) && (numb < 0x08)) ? 1 : 0;
            numb = (item == nSetMAG && numb < 0x03) ? 0 : numb;
            numb = (item == nSetIMP && numb < 0x06) ? 0 : numb;
            if (pn == item && pc == numb) {
                if (!msg.value(Qt::Key_3).isNull() && pt == 0) {
                    int row = tmp.value(Qt::Key_0).toInt();
                    QString str = msg.value(Qt::Key_3).toString();
                    mView->item(row, 2)->setText(str);
                    if (row > 13) {
                        mView->verticalScrollBar()->setValue(row - 13);
                    }
                }
                if (!msg.value(Qt::Key_4).isNull()) {  // 测试判定
                    int row = tmp.value(Qt::Key_0).toInt();
                    QString str = msg.value(Qt::Key_4).toString();
                    QString txt = mView->item(row, 2)->text();
                    QString pre = mView->item(row, 3)->text();
                    if (pt == 1) {
                        int pk = txt.count(":");
                        mView->item(row, 2)->setText(txt + tr(" %1:").arg(pk+1) + str);
                    }
                    str = (pre == "NG") ? pre : str;
                    QColor brush = QColor((str == "OK") ? Qt::green : Qt::red);
                    mView->item(row, 3)->setText(str);
                    mView->item(row, 3)->setForeground(QBrush(brush));
                    if (str != "OK")
                        tmpNG.insert(item, "1");
                }
            }
        }
    }
    if (item == 6021) {  // 空载/负载/反电势波形
        recvFGWave(msg);
    }
    if (item == 6037) {
        quint32 hex = msg.value(Qt::Key_5).toString().toInt();
        if (hex & X05) {  // 左光幕
            labels.value("stop")->setStyleSheet("color:black;background:#00FF00");
        } else {
            labels.value("stop")->setStyleSheet("color:white;background:#121919");
        }
        if (hex & X14) {  // 左产品
            labels.value("prod")->setStyleSheet("color:black;background:#00FF00");
        } else {
            labels.value("prod")->setStyleSheet("color:white;background:#121919");
        }
    }
    //    if (item == 6042) {  // 手动波形
    //        recvManual(msg);
    //    }
    //    if (item == 6087) {  // 匝间电压
    //        strv = msg.value(Qt::Key_5).toString();
    //    }
    tmpMap.clear();
}

void AppTester::recvAppMsg(QTmpMap msg)
{
    int addr = 0;
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        addr = tmpSet.value(3000 + Qt::Key_0).toInt();  // 临时参数地址
        if (!msg.value(addr + TEMPCODE).isNull()) {
            QString str = tr("当前编码:%1").arg(msg.value(addr + TEMPCODE).toString());
            codeText->setText(strMY.arg(str));
        }
        break;
    case Qt::Key_Shop:
        recvTmpMsg(msg);
        break;
    case Qt::Key_News:
        if (this->isHidden())
            break;
        recvNewMsg(msg);
        break;
    case Qt::Key_Call:
        recvLedMsg(msg);
        break;
    case Qt::Key_WLAN:
        if (msg.value(Qt::Key_1).toInt() == 0) {
            signText->setText(strSG.arg(tr("网络状态:未连接")));
        } else {
            signText->setText(strSW.arg(tr("网络状态:%1%").arg(msg.value(Qt::Key_2).toInt())));
        }
        break;
    case Qt::Key_Word:
        recvErrMsg(msg);
        break;
    default:
        break;
    }
}

void AppTester::showEvent(QShowEvent *e)
{
    QTimer::singleShot(2000, this, SLOT(updateTest()));
    btnTest->setEnabled(false);
    this->setFocus();
    initSettings();
    updateType();
    updateShow();
    e->accept();
}

