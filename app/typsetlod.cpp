/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       负载配置界面
*******************************************************************************/
#include "typsetlod.h"

TypSetLod::TypSetLod(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetLod::initUI()
{
    initLayout();
    initViewBar();
    initTimeBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetLod::initLayout()
{
    layout = new QVBoxLayout(this);
}

void TypSetLod::initViewBar()
{
    QStringList names;
    names << tr("上限") << tr("下限");
    QStringList items;
    items << tr("电流(mA)") << tr("功率(W)") << tr("Icc(mA)") << tr("转速(rpm)") << tr("转向");
    iMode = new BoxQModel(this);
    iMode->setRowCount(items.size());
    iMode->setColumnCount(names.size());
    iMode->setHorizontalHeaderLabels(names);
    iMode->setVerticalHeaderLabels(items);
    connect(iMode, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    iView = new QTableView(this);
    iView->setModel(iMode);
    iView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    iView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    iView->horizontalHeader()->setFixedHeight(30);
    iView->verticalHeader()->setFixedWidth(90);
    connect(iView, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));

    QVBoxLayout *play = new QVBoxLayout;
    play->addWidget(iView);
    QGroupBox *boxTest = new QGroupBox(this);
    boxTest->setTitle(tr("测试项目"));
    boxTest->setFixedHeight(240);
    boxTest->setLayout(play);

    QStringList vname;
    vname << tr("参数");
    QStringList volts;
    volts << tr("Vm 电压(V)") << tr("Vcc电压(V)") << tr("Vsp电压(V)")
          << tr("扭矩设置(N·m)") << tr("测试时间(s)") << tr("外驱频率(Hz)");
    vMode = new BoxQModel(this);
    vMode->setRowCount(volts.size());
    vMode->setColumnCount(vname.size());
    vMode->setVerticalHeaderLabels(volts);
    vMode->setHorizontalHeaderLabels(vname);
    connect(vMode, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(change()));
    vView = new QTableView(this);
    vView->setModel(vMode);
    vView->verticalHeader()->setFixedWidth(128);
    vView->horizontalHeader()->setFixedHeight(30);
    vView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList pname;
    pname << tr("参数");
    QStringList parms;
    parms << tr("启动延时(s)")<< tr("Vcc补偿(V)") << tr("Vsp补偿(V)")
          << tr("扭矩补偿(N·m)") << tr("电源选择") << tr("Vcc内外置");
    pMode = new BoxQModel(this);
    pMode->setRowCount(parms.size());
    pMode->setColumnCount(pname.size());
    pMode->setVerticalHeaderLabels(parms);
    pMode->setHorizontalHeaderLabels(vname);
    connect(pMode, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(change()));

    pView = new QTableView(this);
    pView->setModel(pMode);
    pView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pView->horizontalHeader()->setFixedHeight(30);
    pView->verticalHeader()->setFixedWidth(128);
    connect(pView, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));

    QStringList mname;
    mname << tr("参数");
    QStringList pwms;
    pwms << tr("驱动方式") << tr("PWM电压(V)") << tr("PWM频率(Hz)") << tr("PWM占空比(%)")
         << tr("采样频率(kHz)") << tr("采样长度");
    mMode = new BoxQModel(this);
    mMode->setRowCount(parms.size());
    mMode->setColumnCount(mname.size());
    mMode->setVerticalHeaderLabels(pwms);
    mMode->setHorizontalHeaderLabels(mname);
    connect(mMode, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(change()));

    mView = new QTableView(this);
    mView->setModel(mMode);
    mView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mView->horizontalHeader()->setFixedHeight(30);
    mView->verticalHeader()->setFixedWidth(128);
    connect(mView, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));

    QHBoxLayout *vlay = new QHBoxLayout;
    vlay->addWidget(vView);
    vlay->addWidget(pView);
    vlay->addWidget(mView);
    QGroupBox *boxParm = new QGroupBox(this);
    boxParm->setTitle(tr("设置参数"));
    boxParm->setFixedHeight(240);
    boxParm->setLayout(vlay);

    QHBoxLayout *flay = new QHBoxLayout;
    flay->addWidget(boxTest);
    flay->addWidget(boxParm);
    layout->addStretch();
    layout->addLayout(flay);
}

void TypSetLod::initTimeBar()
{
    QStringList times;
    times << tr("T0") << tr("T1") << tr("T2") << tr("T3") << tr("T4")
          << tr("T5") << tr("T6") << tr("T7") << tr("T8") << tr("T9");
    tMode = new BoxQModel();
    tMode->setRowCount(1);
    tMode->setColumnCount(times.size());
    tMode->setHorizontalHeaderLabels(times);
    connect(tMode, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(drawWave()));
    connect(tMode, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(change()));
    tView = new QTableView(this);
    tView->setModel(tMode);
    tView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tView->verticalHeader()->setVisible(false);

    wave = new BoxQImage(this);
    wave->setFixedHeight(200);

    QVBoxLayout *tlay = new QVBoxLayout;
    tlay->addWidget(tView);
    tlay->addWidget(wave);
    QGroupBox *boxTime = new QGroupBox(this);
    boxTime->setTitle(tr("设置时序"));
    boxTime->setFixedHeight(320);
    boxTime->setLayout(tlay);
    layout->addWidget(boxTime);
}

void TypSetLod::initButtonBar()
{
    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setFixedSize(97, 44);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);
    layout->addLayout(btnLayout);
    layout->addStretch();
}

void TypSetLod::initItemDelegate()
{
    isInit = false;
    turns  << "不测试" << "逆时针" << "顺时针";
    speeds << "FG" << "伺服";
    drivers << tr("内置") << tr("外置");
    sources << tr("工控蓝仪") << tr("菊水电源") << tr("天宏电源") << tr("蓝仪电源");
    modes << "PWM" << "Vsp";
    freqs << "1K" << "8K" << "10K" << "20K";

    BoxDouble *curr = new BoxDouble;
    iView->setItemDelegateForRow(0, curr);
    iView->setItemDelegateForRow(1, curr);
    iView->setItemDelegateForRow(2, curr);
    BoxDouble *rate = new BoxDouble;
    iView->setItemDelegateForRow(3, rate);
    iView->setItemDelegateForRow(4, new BoxQItems);

    BoxDouble *volt = new BoxDouble;
    volt->setDecimals(0);
    volt->setMaxinum(500);
    vView->setItemDelegateForRow(0, volt);
    BoxDouble *voff = new BoxDouble;
    voff->setMaxinum(30);
    vView->setItemDelegateForRow(1, voff);
    vView->setItemDelegateForRow(2, voff);
    BoxDouble *toff = new BoxDouble;
    toff->setMaxinum(1.5);
    vView->setItemDelegateForRow(3, toff);
    BoxDouble *time = new BoxDouble;
    time->setDecimals(1);
    time->setMininum(3);
    vView->setItemDelegateForRow(4, time);
    BoxDouble *freq = new BoxDouble;
    freq->setDecimals(0);
    vView->setItemDelegateForRow(5, freq);

    BoxDouble *poff = new BoxDouble;
    poff->setMininum(-30);
    pView->setItemDelegateForRow(0, poff);
    pView->setItemDelegateForRow(1, poff);
    pView->setItemDelegateForRow(2, poff);
    pView->setItemDelegateForRow(4, new BoxQItems);
    pView->setItemDelegateForRow(5, new BoxQItems);

    mView->setItemDelegateForRow(0, new BoxQItems);
    mView->setItemDelegateForRow(1, voff);
    mView->setItemDelegateForRow(2, new BoxQItems);
}

void TypSetLod::initSettings()
{
    int row = 0;
    int item = Qt::Key_C;
    item = (this->objectName() == "setnld") ? Qt::Key_D : item;
    item = (this->objectName() == "setlvs") ? Qt::Key_F : item;
    int addr = tmpSet.value(4000 + item).toInt();  // 负载配置地址

    for (int i=0; i < iMode->rowCount()*2; i++) {
        QString str = tmpSet.value(addr + CACHELOD*row + i).toString();
        if (i == 8)
            str = turns.at(abs(str.toInt()) % turns.size());
        if (i == 9)
            str = speeds.at(abs(str.toInt()) % speeds.size());
        iMode->setData(iMode->index(i/2, i%2), str, Qt::DisplayRole);
    }
    row++;
    for (int i=0; i < vMode->rowCount(); i++) {
        QString str = tmpSet.value(addr + CACHELOD*row + i).toString();
        vMode->setData(vMode->index(i, 0), str, Qt::DisplayRole);
    }
    row++;
    for (int i=0; i < pMode->rowCount(); i++) {
        QString str = tmpSet.value(addr + CACHELOD*row + i).toString();
        if (i == 4)
            str = sources.at(abs(str.toInt()) % sources.size());
        if (i == 5)
            str = drivers.at(abs(str.toInt()) % drivers.size());
        pMode->setData(pMode->index(i, 0), str, Qt::DisplayRole);
    }
    row++;
    for (int i=0; i < tMode->columnCount(); i++) {
        QString str = tmpSet.value(addr + CACHELOD*row + i).toString();
        tMode->setData(tMode->index(0, i), str, Qt::DisplayRole);
    }
    row++;
    for (int i=0; i < mMode->rowCount(); i++) {
        QString str = tmpSet.value(addr + CACHELOD*row + i).toString();
        if (i == 0)
            str = modes.at(abs(str.toInt()) % modes.size());
        if (i == 2)
            str = freqs.at(abs(str.toInt()) % freqs.size());
        mMode->setData(mMode->index(i, 0), str, Qt::DisplayRole);
    }
    isInit = (this->isHidden()) ? false : true;
}

void TypSetLod::saveSettings()
{
    confSettings();
    int item = Qt::Key_C;
    item = (this->objectName() == "setnld") ? Qt::Key_D : item;
    item = (this->objectName() == "setlvs") ? Qt::Key_F : item;
    int addr = tmpSet.value(4000 + item).toInt();  // 负载配置地址
    for (int i=0; i < iMode->rowCount()*2; i++) {
        QString str = iMode->index(i/2, i%2).data().toString();
        if (i < 8)
            str = QString::number(iMode->index(i/2, i%2).data().toDouble());
        if (i == 8)
            str = QString::number(turns.indexOf(str));
        if (i == 9)
            str = QString::number(speeds.indexOf(str));
        tmpMsg.insert(addr + CACHELOD*0x00 + i, str);
    }
    for (int i=0; i < vMode->rowCount(); i++) {
        QString str = QString::number(vMode->index(i, 0).data().toDouble());
        tmpMsg.insert(addr + CACHELOD*0x01 + i, str);
    }
    for (int i=0; i < pMode->rowCount(); i++) {
        QString str = pMode->index(i, 0).data().toString();
        if (i < 3)
            str = QString::number(pMode->index(i, 0).data().toDouble());
        if (i == 4)
            str = QString::number(sources.indexOf(str));
        if (i == 5)
            str = QString::number(drivers.indexOf(str));
        tmpMsg.insert(addr + CACHELOD*0x02 + i, str);
    }
    for (int i=0; i < tMode->columnCount(); i++) {
        QString str = tMode->index(0, i).data().toString();
        tmpMsg.insert(addr + CACHELOD*0x03 + i, str);
    }
    for (int i=0; i < mMode->rowCount(); i++) {
        QString str = mMode->index(i, 0).data().toString();
        if (i == 0)
            str = QString::number(modes.indexOf(str));
        if (i == 2)
            str = QString::number(freqs.indexOf(str));
        tmpMsg.insert(addr + CACHELOD*0x04 + i, str);
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetLod::confSettings()
{
    QStringList tmpStr;
    tmpStr << "curr_max" << "curr_min" << "pwr_max" << "pwr_min" << "icc_max" << "icc_min";
    tmpStr << "speed_max" << "speed_min" << "turn" << "speed_from";
    for (int i=0; i < tmpStr.size(); i++) {
        QString str = iMode->index(i/2, i%2).data().toString();
        if (i == 8)
            str = QString::number(turns.indexOf(str));
        if (i == 9)
            str = QString::number(speeds.indexOf(str));
        tmpMap.insert(tmpStr.at(i), str);
    }
    tmpStr.clear();
    tmpStr << "volt" << "vcc_volt" << "vsp_volt" << "torque" << "time" << "freq";
    for (int i=0; i < tmpStr.size(); i++) {
        QString str = vMode->index(i, 0).data().toString();
        if (i == 0) {
            double v = pMode->index(0, 0).data().toString().toDouble();
            str = QString::number(str.toDouble() + v);
        }
        tmpMap.insert(tmpStr.at(i), str);
    }
    tmpStr.clear();
    tmpStr << "driver" << "vcc_offset" << "vsp_offset" << "torque_offset" << "power" << "vcc_driver";
    for (int i=0; i < tmpStr.size(); i++) {
        QString str = pMode->index(i, 0).data().toString();
        if (i == 0)
            str = QString::number(0);
        if (i == 4)
            str = QString::number(sources.indexOf(str));
        if (i == 5)
            str = QString::number(drivers.indexOf(str));
        tmpMap.insert(tmpStr.at(i), str);
    }
    tmpStr.clear();
    for (int i=0; i < tMode->columnCount(); i++) {
        QString str = tMode->index(0, i).data().toString();
        tmpStr.append(str);
    }
    tmpMap.insert("sequence", tmpStr.join(","));

    tmpStr.clear();
    tmpStr << "vsp_pwm" << "pwm_volt" << "pwm_rate"
           << "pwm_duty" << "sample_freq" << "sample_lenth";
    for (int i=0; i < mMode->rowCount(); i++) {
        QString str = mMode->index(i, 0).data().toString();
        if (i == 0)
            str = QString::number(modes.indexOf(str));
        if (i == 2)
            str = QString::number(freqs.indexOf(str) + 1);
        tmpMap.insert(tmpStr.at(i), str);
    }

    tmpMap.insert("freq_std", 1000);
    tmpMap.insert("baudrate", 6);

    QString str = QString("LOAD");
    str = (this->objectName() == "setnld") ? QString("NOLOAD") : str;
    str = (this->objectName() == "setlvs") ? QString("LPH") : str;

    config.insert(str, tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetLod::autoChange()
{
    change();
    if (isInit) {
        if (iView->hasFocus()) {
            int r = iView->currentIndex().row();
            int c = iView->currentIndex().column();
            QString tmpStr;
            if (r == 0x04) {
                if (c == 0) {
                    tmpStr = iMode->index(r, 0).data().toString();
                    tmpStr = turns.at((turns.indexOf(tmpStr) + 1) % turns.size());
                    iMode->setData(iMode->index(r, 0), tmpStr, Qt::DisplayRole);
                }
                if (c == 1) {
                    tmpStr = iMode->index(r, 1).data().toString();
                    tmpStr = speeds.at((speeds.indexOf(tmpStr) + 1) % speeds.size());
                    iMode->setData(iMode->index(r, 1), tmpStr, Qt::DisplayRole);
                }
            }
        }
        if (pView->hasFocus()) {
            QString tmpStr;
            int r = pView->currentIndex().row();
            int setuser = tmpSet.value(DataUser).toInt();
            if ((r == 4) && (tmpSet.value(setuser).toString() == "supper")) {
                tmpStr = pMode->index(r, 0).data().toString();
                tmpStr = sources.at((sources.indexOf(tmpStr) + 1) % sources.size());
                pMode->setData(pMode->index(r, 0), tmpStr, Qt::DisplayRole);
            }
            if (r == 5) {
                tmpStr = pMode->index(r, 0).data().toString();
                tmpStr = drivers.at((drivers.indexOf(tmpStr) + 1) % drivers.size());
                pMode->setData(pMode->index(r, 0), tmpStr, Qt::DisplayRole);
            }
        }
        if (mView->hasFocus()) {
            QString tmpStr;
            int r = mView->currentIndex().row();
            if (r == 0) {
                tmpStr = mMode->index(r, 0).data().toString();
                tmpStr = modes.at((modes.indexOf(tmpStr) + 1) % modes.size());
                mMode->setData(mMode->index(r, 0), tmpStr, Qt::DisplayRole);
            }
            if (r == 2) {
                tmpStr = mMode->index(r, 0).data().toString();
                tmpStr = freqs.at((freqs.indexOf(tmpStr) + 1) % freqs.size());
                mMode->setData(mMode->index(r, 0), tmpStr, Qt::DisplayRole);
            }
        }
    }
}

void TypSetLod::autoInput()
{
    change();
    if (isInit) {
        int r = iView->currentIndex().row();
        if (iMode->item(r, 1)->text().toDouble() > iMode->item(r, 0)->text().toDouble()) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            iMode->item(r, 1)->setText("0.00");
        }
    }
}

void TypSetLod::drawWave()
{
    if (isInit) {
        double ta = 0;
        for (int i=0; i < tMode->columnCount(); i++) {
            ta += tMode->index(0, i).data().toString().toDouble() * 100;
        }
        ta *= 2;
        QMap<int, double> tp, tl;
        for (int i=0; i < 12; i++) {
            if (i == 0 || i == 11)
                tp[i] = 5;
            if (i == 6)
                tp[i] = 40;
            if (i > 0 && i < 6) {
                tp[i] = tMode->index(0, i-1).data().toString().toDouble() * 100 * 100 / ta;
            }
            if (i > 6 && i < 11) {
                tp[i] = tMode->index(0, i-2).data().toString().toDouble() * 100 * 100 / ta;
            }
        }
        tl[0] = tp[0];
        for (int i=1; i < 12; i++) {
            volatile double pp = 0;
            for (int t=i; t >= 0; t--)
                pp += tp[t];
            tl[i] = pp;
        }
        for (int i=0; i < tl.size(); i++) {
            QVariantMap tmp;
            tmp.insert("index", i);
            tmp.insert("point", tl[i] + ((i > 5) ? 0.1*i : 0));
            wave->setGrids(tmp);
            tmp.insert("color", int(Qt::white));
            tmp.insert("width", tl[(i > 4 && i < 10) ? i + 1 : i] + ((i > 4) ? 0.1*i : 0));
            tmp.insert("lenth", 80);
            tmp.insert("title", tr("T%1").arg(i));
            wave->setTexts(tmp);
        }
        QList<int> cc;
        cc << int(Qt::yellow) << int(Qt::green) << int(Qt::red);
        QStringList names;
        names << "Vcc" << "Vm" << "Vsp";
        for (int t=0; t < 3; t++) {
            QVariantMap tmp;
            tmp.insert("width", 2);
            tmp.insert("index", t);
            tmp.insert("frame", 1);
            tmp.insert("color", cc.at(t));
            QStringList mPoint;
            for (int i=0; i < 100; i++) {
                if (i < tl[t*2] || i >= tl[11-t*2])
                    mPoint.append(QString::number(5 + (2-t)*25));
                if (i >= tl[t*2] && i < tl[t*2 + 1]) {
                    mPoint.append(QString::number(5 + (i - tl[t*2])*20/tp[t*2+1] + (2-t)*25));
                }
                if (i >= tl[t*2 + 1] && i < tl[10-t*2]) {
                    mPoint.append(QString::number(25 + (2-t)*25));
                }
                if (i >= tl[10-t*2] && i < tl[11-t*2]) {
                    mPoint.append(QString::number(5 + (tl[11-t*2] - i)*20/tp[11-t*2] + (2-t)*25));
                }
            }
            tmp.insert("point", mPoint);
            wave->setLines(tmp);
            tmp.insert("index", t + 10);
            tmp.insert("lenth", (2-t)*25+10);
            tmp.insert("title", names.at(t));
            wave->setTexts(tmp);
        }
        wave->update();
    }
}

void TypSetLod::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetLod::recvShowEvent()
{
    QString str = QString("LOAD");
    str = (this->objectName() == "setnld") ? QString("NOLOAD") : str;
    str = (this->objectName() == "setlvs") ? QString("LPH") : str;
    if (this->objectName() != "setlod") {
        vView->hideRow(3);
        pView->hideRow(3);
        QStringList parms;
        parms << tr("Vm补偿(V)")<< tr("Vcc补偿(V)") << tr("Vsp补偿(V)")
              << tr("扭矩补偿(N·m)") << tr("电源选择") << tr("Vcc内外置");
        pMode->setVerticalHeaderLabels(parms);
    }
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 %1").arg(str));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetLod::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Down:
        if (msg.value(Qt::Key_1).toString() == this->objectName()) {
            recvShowEvent();
            confSettings();
        }
        break;
    case Qt::Key_Save:
        if (!this->isHidden())
            saveSettings();
        break;
    default:
        break;
    }
}

void TypSetLod::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    drawWave();
    e->accept();
}

void TypSetLod::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}
