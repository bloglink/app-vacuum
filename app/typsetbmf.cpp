/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       反电动势配置界面
*******************************************************************************/
#include "typsetbmf.h"

TypSetBmf::TypSetBmf(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetBmf::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetBmf::initLayout()
{
    layout = new QVBoxLayout(this);
}

void TypSetBmf::initViewBar()
{
    QStringList names;
    names << tr("上限") << tr("下限");
    QStringList headers;
    headers << tr("电压有效值") << tr("反电动势常数") << tr("三相相位差")
            << tr("三相不平衡度") << tr("三相相序") << tr("电压峰峰值");
    iMode = new BoxQModel();
    iMode->setRowCount(headers.size());
    iMode->setColumnCount(names.size());
    iMode->setHorizontalHeaderLabels(names);
    iMode->setVerticalHeaderLabels(headers);
    connect(iMode, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    iView = new QTableView(this);
    iView->setModel(iMode);
    iView->verticalHeader()->setFixedWidth(108);
    iView->horizontalHeader()->setFixedHeight(30);
    iView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    iView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    connect(iView, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));

    QVBoxLayout *ilay = new QVBoxLayout;
    ilay->addWidget(iView);

    QGroupBox *boxTest = new QGroupBox(this);
    boxTest->setTitle(tr("测试项目"));
    boxTest->setFixedHeight(300);
    boxTest->setLayout(ilay);

    QStringList pname;
    pname << tr("参数");
    QStringList phead;
    phead << tr("设置转速") << tr("设置转向") << tr("设置电压") << tr("测试时间") << tr("温度补偿")
          << tr("采样频率") << tr("采样长度");
    pMode = new BoxQModel();
    pMode->setRowCount(phead.size());
    pMode->setColumnCount(pname.size());
    pMode->setHorizontalHeaderLabels(pname);
    pMode->setVerticalHeaderLabels(phead);

    pView = new QTableView(this);
    pView->setModel(pMode);
    pView->verticalHeader()->setFixedWidth(75);
    pView->horizontalHeader()->setFixedHeight(30);
    pView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(pView, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));

    QHBoxLayout *play = new QHBoxLayout;
    play->addWidget(pView);
    QGroupBox *boxParm = new QGroupBox(this);
    boxParm->setTitle(tr("设置参数"));
    boxParm->setFixedHeight(300);
    boxParm->setLayout(play);

    QHBoxLayout *flay = new QHBoxLayout;
    flay->addWidget(boxTest);
    flay->addWidget(boxParm);
    layout->addStretch();
    layout->addLayout(flay);
}

void TypSetBmf::initButtonBar()
{
    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setMinimumSize(97, 35);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);

    layout->addLayout(btnLayout);
    layout->addStretch();
}

void TypSetBmf::initItemDelegate()
{
    isInit = false;
    sques << tr("不测试") << tr("ABC") << tr("ACB");
    turns << "逆时针" << "顺时针";
    comps << tr("不补偿") << tr("补偿");

    BoxDouble *bemf = new BoxDouble;
    bemf->setMaxinum(3000);
    iView->setItemDelegateForRow(0, bemf);
    iView->setItemDelegateForRow(1, bemf);
    BoxDouble *diff = new BoxDouble;
    diff->setDecimals(0);
    diff->setMaxinum(360);
    iView->setItemDelegateForRow(2, diff);
    //    BoxDouble *noun = new BoxDouble;
    //    iView->setItemDelegateForRow(3, noun);
//    iView->setItemDelegateForRow(2, new BoxQItems);
    iView->setItemDelegateForRow(3, new BoxQItems);
    iView->setItemDelegateForRow(4, new BoxQItems);
    iView->setItemDelegateForRow(5, bemf);

    BoxDouble *speed = new BoxDouble;
    speed->setDecimals(0);
    speed->setMaxinum(3000);
    pView->setItemDelegateForRow(0, speed);
    pView->setItemDelegateForRow(1, new BoxQItems);
    BoxDouble *volt = new BoxDouble;
    volt->setDecimals(1);
    volt->setMaxinum(30);
    pView->setItemDelegateForRow(2, volt);
    BoxDouble *time = new BoxDouble;
    time->setMaxinum(999);
    time->setDecimals(1);
    pView->setItemDelegateForRow(3, time);
    pView->setItemDelegateForRow(4, new BoxQItems);
}

void TypSetBmf::initSettings()
{
    int row = 0;
    int addr = tmpSet.value(4000 + Qt::Key_E).toInt();  // 负载配置地址
    for (int i=0; i < 12; i++) {
        iMode->setData(iMode->index(i/2, i%2), tmpSet.value(addr + i), Qt::DisplayRole);
        if (i == 8) {
            int t = tmpSet.value(addr + i).toInt() % sques.size();
            iMode->setData(iMode->index(i/2, i%2), sques.at(t), Qt::DisplayRole);
        }
    }
    row++;
    for (int i=0; i < pMode->rowCount(); i++) {
        QString str = tmpSet.value(addr + CACHEBMF*row + i).toString();
        if (i == 1)
            str = turns.at(str.toInt() % turns.size());
        if (i == 4)
            str = comps.at(str.toInt() % comps.size());
        pMode->setData(pMode->index(i, 0), str, Qt::DisplayRole);
    }
    isInit = (this->isHidden()) ? false : true;
}

void TypSetBmf::saveSettings()
{
    confSettings();
    int addr = tmpSet.value(4000 + Qt::Key_E).toInt();  // BEMF配置地址
    for (int i=0; i < iMode->rowCount()*2; i++) {
        QString str = iMode->index(i/2, i%2).data().toString();
        if (i == 8)
            str = QString::number(sques.indexOf(str));
        if (i == 7 || i == 9)
            continue;
        tmpMsg.insert(addr + CACHEBMF*0x00 + i, str);
    }
    for (int i=0; i < pMode->rowCount(); i++) {
        QString str = pMode->index(i, 0).data().toString();
        if (i == 1)
            str = QString::number(turns.indexOf(str));
        if (i == 4)
            str = QString::number(comps.indexOf(str));
        //        if (i == 5)
        //            str = "0";
        tmpMsg.insert(addr + CACHEBMF*0x01 + i, str);
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetBmf::confSettings()
{
    int row = 0;
    QStringList tmpStr;
    tmpStr << "volt_max" << "volt_min" << "bemf_max" << "bemf_min" << "angle_max" << "angle_min"
           << "noun" << "noun_min" << "phase" << "phase_min";
    for (int i=0; i < tmpStr.size(); i++) {
        QString str = iMode->index(i/2, i%2).data().toString();
        if (i == tmpStr.indexOf("phase"))
            str = QString::number(sques.indexOf(str));
        if (i == tmpStr.indexOf("phase_min") || i == tmpStr.indexOf("noun_min"))
            continue;
        tmpMap.insert(tmpStr.at(i), str);
    }
    row++;
    tmpStr.clear();
    tmpStr << "speed" << "turn" << "volt_vcc" << "time" << "tmpcomp"
           << "sample_freq" << "sample_lenth";
    for (int i=0; i < tmpStr.size(); i++) {
        QString str = pMode->index(i, 0).data().toString();
        if (i == tmpStr.indexOf("turn"))
            str = QString::number(turns.indexOf(str));
        if (i == tmpStr.indexOf("tmpcomp"))
            continue;
        tmpMap.insert(tmpStr.at(i), str);
    }
    tmpMap.insert("gear", "0");  // 档位 0:大档;1:小档
    tmpMap.insert("type", "0");  // 连接方式 0:三角型 1:Y型
    tmpMap.insert("driver", "0");  // 内置Vcc
    config.insert("BEMF", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetBmf::autoChange()
{
    change();
    if (isInit) {
        if (pView->hasFocus()) {
            int r = pView->currentIndex().row();
            QString tmpStr;
            if (r == 1) {
                tmpStr = pMode->index(r, 0).data().toString();
                tmpStr = turns.at((turns.indexOf(tmpStr) + 1) % turns.size());
                pMode->setData(pMode->index(r, 0), tmpStr, Qt::DisplayRole);
            }
            if (r == 4) {
                tmpStr = pMode->index(r, 0).data().toString();
                tmpStr = comps.at((comps.indexOf(tmpStr) + 1) % comps.size());
                pMode->setData(pMode->index(r, 0), tmpStr, Qt::DisplayRole);
            }
        }
    }
}

void TypSetBmf::autoInput()
{
    change();
    if (isInit) {
        int r = iView->currentIndex().row();
        double max = iMode->item(r, 0)->text().toDouble();
        double min = iMode->item(r, 1)->text().toDouble();
        if (min > max) {
            QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
            iMode->item(r, 1)->setText("0.00");
        }
    }
}

void TypSetBmf::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetBmf::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 BEMF"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetBmf::recvAppMsg(QTmpMap msg)
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

void TypSetBmf::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetBmf::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}


