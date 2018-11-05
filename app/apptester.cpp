/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#include "apptester.h"

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
    initButtonBar();
    initHistogram();
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
    mView->setEnabled(false);
    mView->setStyleSheet("border:none");
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);
    mView->verticalHeader()->setFixedWidth(24);
    mView->horizontalHeader()->setFixedHeight(32);
    mView->verticalHeader()->setMinimumSectionSize(30);
    mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    mView->setColumnWidth(0, 120);
    mView->setColumnWidth(1, 194);
    mView->setColumnWidth(3, 108);
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
    title->setText(titleAP.arg(tr("青岛艾普智能仪器有限公司")));
    wView->setCellWidget(0, 4, title);
    wView->setSpan(0, 4, 1, 2);
    wView->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    wView->setRowHeight(0, 42);
    wView->verticalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    wView->setRowHeight(1, 60);
}

void AppTester::initWorkBar()
{
    workText = new QLabel(this);
    workText->setText(judgeON.arg("左"));
    wView->setCellWidget(1, 4, workText);
    wView->setSpan(1, 4, 2, 1);
}

void AppTester::initRealBar()
{
    realText = new QLabel(this);
    realText->setText(judgeOK);
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

        BoxQLabel *w = new BoxQLabel(this);
        lay->addWidget(w, 0, 0, magitem.size()+1, 1);

        for (int k=0; k < magitem.size(); k++) {
            QLabel *m = new QLabel(largeEN.arg(magitem.at(k) + ":----"), this);
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

        BoxQLabel *w = new BoxQLabel(this);
        impWave.append(w);

        lay->addWidget(w, 0, 0, impitem.size(), 1);

        for (int k=0; k < impitem.size(); k++) {
            QLabel *m = new QLabel(largeEN.arg(impitem.at(k) + "----"), this);
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
    typeText = new QLabel(largeHD.arg(tr("当前型号:X123456789ABCDEF")), this);
    wView->setCellWidget(3, 4, typeText);
    wView->setSpan(3, 4, 1, 2);

    codeText = new QLabel(this);
    codeText->setText(largeHD.arg(tr("当前编码:178912013X")));
    wView->setCellWidget(4, 4, codeText);
    wView->setSpan(4, 4, 1, 2);
}

void AppTester::initButtonBar()
{
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
    btnLogo->setPixmap(QPixmap(":/icon_aip.png"));
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
    btnTest->setMinimumSize(97, 81);
    connect(btnTest, SIGNAL(clicked(bool)), this, SLOT(clickTest()));

    QPushButton *btnStop = new QPushButton("停止测试", this);
    blayout->addWidget(btnStop);
    btnStop->setMinimumSize(97, 81);
    connect(btnStop, SIGNAL(clicked(bool)), this, SLOT(clickStop()));

    blayout->addStretch();

    warnText = new QLabel(this);
    warnText->setMinimumHeight(100);
    blayout->addWidget(warnText);

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
    signText->setText(largeTM.arg(tr("网络状态:____")));
    lay->addWidget(signText, 0, 1);

    userText = new QLabel(this);
    userText->setText(largeTM.arg(tr("当前用户:____")));
    lay->addWidget(userText, 1, 1);

    tempText = new QLabel(largeTM.arg(tr("当前温度:____")), this);
    lay->addWidget(tempText, 2, 1);

    testText = new QLabel(largeTM.arg(tr("测试时间:____")), this);
    lay->addWidget(testText, 3, 1);

    dateText = new QLabel(this);
    lay->addWidget(dateText, 4, 1);
    QString strA = tr("当前日期:") + QDate::currentDate().toString("yy-MM-dd");
    dateText->setText(largeTM.arg(strA));

    timeText = new QLabel(this);
    lay->addWidget(timeText, 5, 1);
    QString strB = tr("当前时间:") + QTime::currentTime().toString("hh:mm:ss");
    timeText->setText(largeTM.arg(strB));

    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(1, 3);
    lay->setColumnStretch(2, 1);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);
}

void AppTester::initSettings()
{
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();
    int work = tmpSet.value(back + backWork).toInt();
    if (work != 2)
        wFrame->hide();
    else
        wFrame->show();

    tmpNG.clear();
    initQuality();
    if (Qt::Key_0) {  // 初始化状态栏显示
        int temp = tmpSet.value((3000 + Qt::Key_0)).toInt();  // 时临数据地址
        int sign = tmpSet.value(temp + TEMPSIGN).toInt();
        int setuser = tmpSet.value(DataUser).toInt();
        QString strSign = (sign == 0) ? tr("未登录") : tmpSet.value(setuser).toString();
        userText->setText(largeTM.arg(tr("当前用户:") + strSign));
        typeText->setText(largeHD.arg(tr("当前型号:") + tmpSet.value(DataType).toString()));
    }
    if (Qt::Key_0) {
        tmpRow = 0;
        int r = tmpSet.value(4000 + Qt::Key_0).toInt();
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
            if (str.toInt() == 0x06) {
                initSetIMP();
            }
            if (str.toInt() == 0x0B) {
                initSetHAL();
            }
            if (str.toInt() == 0x0C) {  // 负载
                initSetLOD();
            }
            if (str.toInt() == 0x0E) {
                initSetBMF();
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
        int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
        if (mode == 2) {
            for (int i=9; i < 15; i++)
                wView->hideRow(i);
            for (int i=15; i < 21; i++)
                wView->showRow(i);
        } else {
            for (int i=9; i < 15; i++)
                wView->showRow(i);
            for (int i=15; i < 21; i++)
                wView->hideRow(i);
        }
    }
}

void AppTester::initSetDCR()
{
    int row = 0;
    int addr = tmpSet.value(4000 + Qt::Key_1).toInt();  // 电阻配置地址
    int noun = tmpSet.value(addr + 2).toInt();  // 不平衡度
    for (int t=0; t < 8; t++) {
        double test = tmpSet.value(addr + CACHEDCR + CACHEDCR*CHECKDCR + t).toDouble();
        double portf = tmpSet.value(addr + CACHEDCR + CACHEDCR*PORTDCR1 + t).toDouble();
        double portt = tmpSet.value(addr + CACHEDCR + CACHEDCR*PORTDCR2 + t).toDouble();
        double unit = tmpSet.value(addr + CACHEDCR + CACHEDCR*UNITDCR1 + t).toDouble();
        double rmax = tmpSet.value(addr + CACHEDCR + CACHEDCR*UPPERDCR + t).toDouble();
        double rmin = tmpSet.value(addr + CACHEDCR + CACHEDCR*LOWERDCR + t).toDouble();
        if (test != 0) {
            QString str = (unit > 0) ? "Ω" : "mΩ";
            str = (unit > 1) ? "kΩ" : str;
            tmpItem.insert(tmpRow, tr("电阻%1-%2").arg(portf).arg(portt));
            tmpParm.insert(tmpRow, tr("%1-%2%3").arg(rmin).arg(rmax).arg(str));
            QTmpMap tmp;
            tmp.insert(Qt::Key_0, tmpRow);
            tmp.insert(Qt::Key_1, 1);
            tmp.insert(Qt::Key_2, t);
            tmpView.append(tmp);
            tmp.clear();
            tmpRow++;
            row++;
        }
    }
    if (noun != 0) {  // 不平衡度
        tmpItem.insert(tmpRow, tr("电阻平衡"));
        tmpParm.insert(tmpRow, tr("<%1%").arg(noun));
        QTmpMap tmp;
        tmp.insert(Qt::Key_0, tmpRow);
        tmp.insert(Qt::Key_1, 1);
        tmp.insert(Qt::Key_2, row);
        tmpView.append(tmp);
        tmp.clear();
        tmpRow++;
    }
}

void AppTester::initSetMAG()
{
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    QStringList testItems = tmpSet.value(conf + ADDRITEM).toString().split(",");
    bool isTest = testItems.contains(QString::number(nSetMAG));
    int addr = tmpSet.value(4000 + Qt::Key_2).toInt();  // 反嵌配置地址
    for (int i=0; i < 3; i++) {
        int check = tmpSet.value(addr + CACHEMAG + CACHEMAG*CHECKMAG + i).toInt();
        int portf = tmpSet.value(addr + CACHEMAG + CACHEMAG*PORTMAG1 + i).toInt();
        int portt = tmpSet.value(addr + CACHEMAG + CACHEMAG*PORTMAG2 + i).toInt();
        int upper = tmpSet.value(addr + CACHEMAG + CACHEMAG*UPPERMAG + i).toInt();
        check = (isTest) ? check : 0;
        QString str = (check != 0) ? largeTM : largeEN;
        magText.at(i*3 + 0)->setText(str.arg(tr("项目: 反嵌%1-%2").arg(portf).arg(portt)));
        magText.at(i*3 + 1)->setText(str.arg(tr("上限: %1%").arg(upper)));
        magText.at(i*3 + 2)->setText(str.arg(tr("差积: ----")));
    }
}

void AppTester::initSetCCW()
{
    int addr = tmpSet.value(4000 + Qt::Key_2).toInt();  // 反嵌配置地址
    int turn = tmpSet.value(addr + 0).toInt();
    if (turn != 2) {
        tmpItem.insert(tmpRow, tr("转向"));
        tmpParm.insert(tmpRow, tr("%1").arg(turn == 0 ? tr("正转") : tr("反转")));
        QTmpMap tmp;
        tmp.insert(Qt::Key_0, tmpRow);
        tmp.insert(Qt::Key_1, 2);
        tmp.insert(Qt::Key_2, 0x10);
        tmpView.append(tmp);
        tmp.clear();
        tmpRow++;
    }
}

void AppTester::initSetINR()
{
    int addr = tmpSet.value(4000 + Qt::Key_3).toInt();  // 交耐配置地址
    int numb = 4;
    double volt = tmpSet.value(addr + CACHEINR*VOLTINR1 + numb).toDouble();
    double smin = tmpSet.value(addr + CACHEINR*LOWERINR + numb).toDouble();
    double time = tmpSet.value(addr + CACHEINR*TIMEINR1 + numb).toDouble();
    volt = (volt == 0) ? 500 : 1000;
    if (1) {
        tmpItem.insert(tmpRow, tr("绝缘"));
        tmpParm.insert(tmpRow, tr("%1V %2M %3s").arg(volt).arg(smin).arg(time));
        QTmpMap tmp;
        tmp.insert(Qt::Key_0, tmpRow);
        tmp.insert(Qt::Key_1, 3);
        tmp.insert(Qt::Key_2, 0);
        tmpView.append(tmp);
        tmp.clear();
        tmpRow++;
    }
}

void AppTester::initSetACW()
{
    int addr = tmpSet.value(4000 + Qt::Key_4).toInt();  // 交耐配置地址
    int numb = 4;
    double volt = tmpSet.value(addr + CACHEACW + CACHEACW*VOLTACW1 + numb).toDouble();
    double smax = tmpSet.value(addr + CACHEACW + CACHEACW*UPPERACW + numb).toDouble();
    double smin = tmpSet.value(addr + CACHEACW + CACHEACW*LOWERACW + numb).toDouble();
    double time = tmpSet.value(addr + CACHEACW + CACHEACW*TIMEACW1 + numb).toDouble();
    if (1) {
        tmpItem.insert(tmpRow, tr("交耐"));
        tmpParm.insert(tmpRow, tr("%1V %2-%3mA %4s").arg(volt).arg(smin).arg(smax).arg(time));
        QTmpMap tmp;
        tmp.insert(Qt::Key_0, tmpRow);
        tmp.insert(Qt::Key_1, 4);
        tmp.insert(Qt::Key_2, 0);
        tmpView.append(tmp);
        tmp.clear();
        tmpRow++;
    }
}

void AppTester::initSetIMP()
{
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    QStringList testItems = tmpSet.value(conf + ADDRITEM).toString().split(",");
    bool isTest = testItems.contains(QString::number(nSetIMP));
    int addr = tmpSet.value(4000 + Qt::Key_6).toInt();  // 反嵌配置地址
    for (int i=0; i < 6; i++) {
        int check = tmpSet.value(addr + CACHEIMP + CACHEIMP*CHECKIMP + i).toInt();
        double portf = tmpSet.value(addr + CACHEIMP + CACHEIMP*PORTIMP1 + i).toDouble();
        double portt = tmpSet.value(addr + CACHEIMP + CACHEIMP*PORTIMP2 + i).toDouble();
        double volts = tmpSet.value(addr + CACHEIMP + CACHEIMP*VOLTIMP1 + i).toDouble();
        double diff1 = tmpSet.value(addr + CACHEIMP + CACHEIMP*DIFFIMP1 + i).toDouble();
        double sflut = tmpSet.value(addr + CACHEIMP + CACHEIMP*FLUTIMP1 + i).toDouble();
        double phase = tmpSet.value(addr + CACHEIMP + CACHEIMP*PHSEIMP1 + i).toDouble();
        check = (isTest) ? check : 0;
        QString str = (check != 0) ? largeTM : largeEN;
        impText.at(i*5 + 0)->setText(str.arg(tr("项目: 匝间%1-%2").arg(portf).arg(portt)));
        impText.at(i*5 + 1)->setText(str.arg(tr("面积: ----")));
        str = (diff1 != 0) ? str : largeEN;
        impText.at(i*5 + 2)->setText(str.arg(tr("差积: ----")));
        str = (sflut != 0) ? str : largeEN;
        impText.at(i*5 + 3)->setText(str.arg(tr("电晕: ----")));
        str = (phase != 0) ? str : largeEN;
        impText.at(i*5 + 4)->setText(str.arg(tr("相位: ----")));
        impWave.at(i)->setEnabled((check != 0) ? true : false);
        impWave.at(i)->setText(tr("%1V").arg(volts), 1);
    }
}

void AppTester::initSetHAL()
{
    int addr = tmpSet.value(4000 + Qt::Key_B).toInt();  // 负载配置地址
    QStringList items;
    items << tr("霍尔高电平") << tr("霍尔低电平") << tr("霍尔占空比") << tr("霍尔频率");
    QStringList units;
    units << "V" << "V" << "%" << "Hz";

    for (int i=0; i < items.size(); i++) {
        int max = tmpSet.value(addr + i*2 + 0).toInt();
        int min = tmpSet.value(addr + i*2 + 1).toInt();
        if (max != 0) {
            tmpItem.insert(tmpRow, items.at(i));
            tmpParm.insert(tmpRow, tr("%1-%2").arg(min).arg(max) + units.at(i));
            QTmpMap tmp;
            tmp.insert(Qt::Key_0, tmpRow);
            tmp.insert(Qt::Key_1, 0x0B);
            tmp.insert(Qt::Key_2, i);
            tmpView.append(tmp);
            tmp.clear();
            tmpRow++;
        }
    }
}

void AppTester::initSetLOD()
{
    int addr = tmpSet.value(4000 + Qt::Key_C).toInt();  // 负载配置地址
    QStringList items;
    items << tr("负载电流") << tr("负载功率") << tr("Icc电流") << tr("负载转速") << tr("负载转向");
    QStringList units;
    units << "mA" << "W" << "mA" << "rpm";

    for (int i=0; i < 5; i++) {
        int max = tmpSet.value(addr + i*2 + 0).toInt();
        int min = tmpSet.value(addr + i*2 + 1).toInt();
        if (max != 0) {
            tmpItem.insert(tmpRow, items.at(i));
            if (i < units.size())
                tmpParm.insert(tmpRow, tr("%1-%2").arg(min).arg(max) + units.at(i));
            else
                tmpParm.insert(tmpRow, tr("%1").arg(max == 1 ? "CCW" : "CW"));
            QTmpMap tmp;
            tmp.insert(Qt::Key_0, tmpRow);
            tmp.insert(Qt::Key_1, 0x0C);
            tmp.insert(Qt::Key_2, i);
            tmpView.append(tmp);
            tmp.clear();
            tmpRow++;
        }
    }
}

void AppTester::initSetBMF()
{
    int addr = tmpSet.value(4000 + Qt::Key_E).toInt();  // 负载配置地址
    QStringList items;
    items << tr("反电动势电压") << tr("反电动势常数") << tr("与霍尔相位差")
          << tr("三相不平衡度") << tr("三相相序");
    QStringList units;
    units << "V" << "V/krmp" << "°" << "%";

    for (int i=0; i < 5; i++) {
        int max = tmpSet.value(addr + i*2 + 0).toInt();
        int min = tmpSet.value(addr + i*2 + 1).toInt();
        if (max != 0) {
            tmpItem.insert(tmpRow, items.at(i));
            if (i < units.size())
                tmpParm.insert(tmpRow, tr("%1-%2").arg(min).arg(max) + units.at(i));
            else
                tmpParm.insert(tmpRow, tr("%1").arg((max == 1) ? "ABC" : "ACB"));
            QTmpMap tmp;
            tmp.insert(Qt::Key_0, tmpRow);
            tmp.insert(Qt::Key_1, 0x0E);
            tmp.insert(Qt::Key_2, i);
            tmpView.append(tmp);
            tmp.clear();
            tmpRow++;
        }
    }
}

void AppTester::initQuality()
{
    codeText->setText(largeHD.arg(tr("当前编码:")));
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

void AppTester::updateShow()
{
    initSetMAG();
    initSetIMP();
    for (int i=0; i < magWave.size(); i++) {
        magWave.at(i)->setZero();
        magWave.at(i)->update();
    }
    for (int i=0; i < impWave.size(); i++) {
        impWave.at(i)->setZero();
        impWave.at(i)->update();
    }
    for (int i=0; i < 3; i++) {
        QVariantMap tmp;
        tmp.insert("width", 2);
        tmp.insert("index", i);
        tmp.insert("frame", 0);
        tmp.insert("shade", 1);
        tmp.insert("point", 0);
        bWave->setLines(tmp);
        bWave->update();
    }
    for (int i=3; i < 8; i++) {
        QVariantMap tmp;
        tmp.insert("width", 2);
        tmp.insert("index", i);
        tmp.insert("frame", 0);
        tmp.insert("shade", 1);
        tmp.insert("point", 0);
        hWave->setLines(tmp);
        hWave->update();
    }

    for (int i=0; i < mView->rowCount(); i++) {
        mView->item(i, 2)->setText("");
        mView->item(i, 3)->setText("");
    }
    testText->setText(largeTM.arg(tr("测试时间:____")));
}

void AppTester::updateTime()
{
    QString strA = tr("当前日期:") + QDate::currentDate().toString("yy-MM-dd");
    dateText->setText(largeTM.arg(strA));
    QString strB = tr("当前时间:") + QTime::currentTime().toString("hh:mm:ss");
    timeText->setText(largeTM.arg(strB));
}

void AppTester::updateTest()
{
    btnTest->setEnabled(true);
}

void AppTester::recvErrMsg(QTmpMap msg)
{
    QString err = msg.value(Qt::Key_1).toString();
    QStringList pp = err.split("\n");
    QString str;
    for (int i=0; i < pp.size(); i++) {
        str.append(largeER.arg(pp.at(i)));
    }
    str.append("<br><\br>");
    warnText->setText(str);
}

void AppTester::recvTmpMsg(QTmpMap msg)
{
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    if (!msg.value(addr + TEMPTEMP).isNull()) {
        QString temp = tr("当前") + msg.value(addr + TEMPTEMP).toString();

        tempText->setText(largeTM.arg(temp));
    }
}

void AppTester::recvLedMsg(QTmpMap msg)
{
    if (msg.value(Qt::Key_2).isNull())
        return;
    int c = msg.value(Qt::Key_2).toInt();
    if (c == DATAON) {
        updateShow();
        ms.restart();
        tmpNG.clear();
        realText->setText(judgeON.arg(tr("测试")));
        workText->setText(judgeON.arg(msg.value(Qt::Key_4).toInt() == 0x14 ? tr("右") : tr("左")));
        btnHome->setEnabled(false);
        btnConf->setEnabled(false);
        btnTest->setEnabled(false);
    }
    if (c == DATAOK || c == DATANG || c == DATADC) {
        QString str = judgeOK;
        testQu = (c == DATADC) ? testQu : testQu + 1;
        testOK = (c != DATAOK) ? testOK : testOK + 1;
        str = (c == DATADC) ? judgeDC.arg(tr("中断")) : str;
        str = (c == DATANG) ? judgeNG : str;
        realText->setText(str);
        testText->setText(largeTM.arg(tr("测试时间:%1s").arg(ms.elapsed()/1000.0)));
        initQuality();
        btnHome->setEnabled(true);
        btnConf->setEnabled(true);
        btnTest->setEnabled(true);
    }
}

void AppTester::recvNewMsg(QTmpMap msg)
{
    int item = msg.value(Qt::Key_1).toInt();  // 测试项目
    int numb = msg.value(Qt::Key_2).toInt();  // 单项数量
    int work = msg.value(Qt::Key_6).toInt();  // 工位

    if (((item != 6 && (item != 2)) || (item == 2 && numb == 0x10)) && item <= 0x0F) {
        for (int i=0; i < tmpView.size(); i++) {
            QTmpMap tmp = tmpView.at(i);
            int pn = tmp.value(Qt::Key_1).toInt();
            int pc = tmp.value(Qt::Key_2).toInt();
            if (pn == item && pc == numb) {
                if (!msg.value(Qt::Key_3).isNull()) {  // 测试结果
                    int row = tmp.value(Qt::Key_0).toInt();
                    QString str = msg.value(Qt::Key_3).toString();
                    mView->item(row, 2)->setText(str);
                }
                if (!msg.value(Qt::Key_4).isNull()) {  // 测试判定
                    int row = tmp.value(Qt::Key_0).toInt();
                    QString str = msg.value(Qt::Key_4).toString();
                    QColor brush = QColor((str == "OK") ? Qt::green : Qt::red);
                    mView->item(row, 3)->setText(str);
                    mView->item(row, 3)->setForeground(QBrush(brush));
                    if (str != "OK")
                        tmpNG.insert(item, "1");
                }
            }
        }
    }
    if (item == 2 && numb != 0x10) {  // 更新反嵌结果
        if (!msg.value(Qt::Key_3).isNull()) {
            QString str = msg.value(Qt::Key_3).toString();
            magText.at(numb*3 + 2)->setText(largeTM.arg(tr("差积: ") + str));
        }
        if (!msg.value(Qt::Key_4).isNull()) {
            QString str = msg.value(Qt::Key_4).toString();
            QStringList ws = tmpStr.split(" ");
            if (ws.size() > 200) {
                ws.removeFirst();
                QVector<double> y(MAG_SIZE);
                for (int i=0; i < qMin(MAG_SIZE, ws.size()); i++) {
                    y[i] = ws.at(i).toInt() * 100 / 256;
                }
                magWave.at(numb)->setWave(y, str == "OK" ? 0x00 : 0x10);
                magWave.at(numb)->update();
            }
            if (str != "OK")
                tmpNG.insert(item, "1");
        }
        if (!msg.value(Qt::Key_5).isNull()) {
            tmpStr = msg.value(Qt::Key_5).toString();
        }
    }
    if (item == 6) {  // 更新匝间结果
        if (!msg.value(Qt::Key_3).isNull()) {
            QString str = msg.value(Qt::Key_3).toString();
            QStringList imp = str.split(",");

            for (int i=0; i < imp.size(); i++) {
                QStringList pp = QString(imp.at(i)).split(":");
                if (i == 0)
                    impText.at(numb*5 + 1 + i)->setText(largeTM.arg(tr("面积: ") + pp.last()));
                if (i == 1)
                    impText.at(numb*5 + 1 + i)->setText(largeTM.arg(tr("差积: ") + pp.last()));
                if (i == 2)
                    impText.at(numb*5 + 1 + i)->setText(largeTM.arg(tr("电晕: ") + pp.last()));
                if (i == 3)
                    impText.at(numb*5 + 1 + i)->setText(largeTM.arg(tr("相位: ") + pp.last()));
            }
        }
        if (!msg.value(Qt::Key_4).isNull()) {
            QString str = msg.value(Qt::Key_4).toString();
            QStringList ws = tmpStr.split(" ");
            if (ws.size() > 200) {
                ws.removeFirst();
                int addr = tmpSet.value(4000 + Qt::Key_H).toInt();  // 匝间标准波形地址
                QVector<double> y(IMP_SIZE), sy(IMP_SIZE);
                int ss = (work == 0x13) ? 0 : 1;
                for (int i=0; i < qMin(IMP_SIZE, ws.size()); i++) {
                    y[i] = ws.at(i).toInt() * 100 / 1024;
                    sy[i] = tmpSet.value(addr + IMP_SIZE*(numb*2 + ss) + i).toInt() * 100 / 1024;
                }
                impWave.at(numb)->setWave(sy, 1);
                impWave.at(numb)->setWave(y, str == "OK" ? 0x00 : 0x10);
                impWave.at(numb)->update();
            }
            if (str != "OK")
                tmpNG.insert(item, "1");
        }
        if (!msg.value(Qt::Key_5).isNull()) {
            tmpStr = msg.value(Qt::Key_5).toString();
        }
    }
    if (item == 6021) {  // 测试波形
        QString w = msg.value(Qt::Key_2).toString();
        QStringList ws = w.split(" ");
        if (ws.size() < 200)
            return;
        int index = ws.at(0).toInt();
        ws.removeFirst();
        QVariantMap tmp;
        tmp.insert("width", 2);
        tmp.insert("index", index);
        tmp.insert("frame", 0);
        tmp.insert("shade", 1);
        QList<int> cc;
        cc << int(Qt::cyan) << int(Qt::yellow) << int(Qt::green);

        if (index < 3) {
            QStringList mPoint;
            for (int t=0; t < ws.size(); t++) {
                mPoint.append(QString::number(ws.at(t).toInt() * 100 / 256));
            }
            tmp.insert("color", cc.at(index));
            tmp.insert("point", mPoint);
            bWave->setLines(tmp);
            bWave->update();
        } else {
            QStringList mPoint;
            for (int t=0; t < ws.size(); t++) {
                int p = ws.at(t).toInt() * 30 / 256 + 16 * ((5 - index) * 2 + 1) - 10;
                mPoint.append(QString::number(p));
            }
            tmp.insert("color", cc.at(index%3));
            tmp.insert("point", mPoint);
            hWave->setLines(tmp);
            hWave->update();
        }
    }
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
            codeText->setText(largeHD.arg(str));
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
            signText->setText(largeEN.arg(tr("网络状态:未连接")));
        } else {
            signText->setText(largeTM.arg(tr("网络状态:%1%").arg(msg.value(Qt::Key_2).toInt())));
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
    updateShow();
    e->accept();
}

