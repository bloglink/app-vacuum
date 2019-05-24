/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电参配置界面
*******************************************************************************/
#include "typsetpwr.h"

TypSetPwr::TypSetPwr(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetPwr::initUI()
{
    initLayout();
    initViewBar();
    initHallBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetPwr::initLayout()
{
    layout = new QVBoxLayout(this);
    layout->addStretch();
}

void TypSetPwr::initViewBar()
{
    QStringList headers;
    headers << tr("电参") << tr("电流上限") << tr("电流下限") << tr("功率上限")
            << tr("功率下限") << tr("容压上限") << tr("容压下限") << tr("转向")
            << tr("转速上限") << tr("转速下限")  << tr("时间");
    mView = new BoxQModel;
    mView->setRowCount(PWR_SIZE);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < PWR_SIZE; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
    }
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(CHECKPWR, QHeaderView::Fixed);
    view->setColumnWidth(CHECKPWR, 96);
    view->setFixedHeight(240);
    layout->addWidget(view);
}

void TypSetPwr::initHallBar()
{
    QStringList headers;
    headers << tr("PG") << tr("高电平上限") << tr("高电平下限") << tr("低电平上限")
            << tr("低电平下限") << tr("频率上限") << tr("频率下限") << tr("占空比上限")
            << tr("占空比下限") << tr("PG电压");
    mHall = new BoxQModel;
    mHall->setRowCount(3);
    mHall->setColumnCount(headers.size());
    mHall->setHorizontalHeaderLabels(headers);

    for (int i=0; i < 3; i++) {
        for (int j=0; j < headers.size(); j++) {
            mHall->setData(mHall->index(i, j), "", Qt::DisplayRole);
        }
    }
    connect(mHall, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoHall()));

    hall = new QTableView(this);
    hall->setModel(mHall);
    hall->horizontalHeader()->setFixedHeight(30);
    hall->setEditTriggers(QAbstractItemView::AllEditTriggers);
    hall->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    hall->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    hall->horizontalHeader()->setSectionResizeMode(CHECKPWR, QHeaderView::Fixed);
    hall->setColumnWidth(CHECKPWR, 96);
    hall->setFixedHeight(150);
    layout->addWidget(hall);
}

void TypSetPwr::initButtonBar()
{
    QGridLayout *btnLayout = new QGridLayout;

    int row = 0;
    passBox = new QCheckBox(tr("测试PG"), this);
    connect(passBox, SIGNAL(clicked(bool)), this, SLOT(change()));
    btnLayout->addWidget(passBox, row, 0);

    row++;
    vmaxBox = new QSpinBox(this);
    vmaxBox->setMaximum(500);
    vmaxBox->setFixedSize(125, 40);
    vmaxBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    vmaxBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btnLayout->addWidget(new QLabel(tr("输入电压")), row, 0);
    btnLayout->addWidget(vmaxBox, row, 1);

    row++;
    voltBox = new QSpinBox(this);
    voltBox->setMaximum(500);
    voltBox->setFixedSize(125, 40);
    voltBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    voltBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(voltBox, SIGNAL(valueChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("输出电压")), row, 0);
    btnLayout->addWidget(voltBox, row, 1);

    readBox = new QComboBox(this);
    readBox->setView(new QListView);
    readBox->addItem(tr("PG"));
    readBox->addItem(tr("光电开关"));
    readBox->setFixedSize(125, 40);
    connect(readBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("转速读取")), row, 2);
    btnLayout->addWidget(readBox, row, 3);

    row++;
    pullBox = new QComboBox(this);
    pullBox->setView(new QListView);
    pullBox->addItem(tr("是"));
    pullBox->addItem(tr("否"));
    pullBox->setFixedSize(125, 40);
    pullBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pullBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("上拉电阻")), row, 0);
    btnLayout->addWidget(pullBox, row, 1);

    countBox = new QSpinBox(this);
    countBox->setFixedSize(125, 40);
    countBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    countBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(countBox, SIGNAL(valueChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("磁极数")), row, 2);
    btnLayout->addWidget(countBox, row, 3);

    QPushButton *btnSave = new QPushButton(tr("保存"), this);
    btnSave->setFixedSize(125, 40);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    btnLayout->addWidget(btnSave, row, 7);

    btnLayout->setColumnStretch(6, 1);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(btnLayout);
    layout->addWidget(box);
    layout->addStretch();
}

void TypSetPwr::initItemDelegate()
{
    isInit = false;
    grades << "---" << "123" << "124" << "125" << "126" << "127" << "128";
    turns << tr("NULL") << tr("CCW") << tr("CW");

    LibDelege *test = new LibDelege;
    test->setInit("combobox", 0, 0, grades);

    LibDelege *turn = new LibDelege;
    turn->setInit("combobox", 0, 0, turns);

    view->setItemDelegateForColumn(CHECKPWR, test);

    BoxDouble *curr = new BoxDouble;
    curr->setDecimals(3);
    view->setItemDelegateForColumn(CMAXPWR1, curr);
    view->setItemDelegateForColumn(CMINPWR1, curr);

    view->setItemDelegateForColumn(PMAXPWR1, curr);
    view->setItemDelegateForColumn(PMINPWR1, curr);

    view->setItemDelegateForColumn(VMAXPWR1, curr);
    view->setItemDelegateForColumn(VMINPWR1, curr);

    view->setItemDelegateForColumn(TURNPWR1, turn);

    view->setItemDelegateForColumn(0x08, curr);
    view->setItemDelegateForColumn(0x09, curr);
    view->setItemDelegateForColumn(0x0A, curr);

    BoxDouble *volt = new BoxDouble;
    volt->setMaxinum(30);
    BoxDouble *freq = new BoxDouble;
    BoxDouble *duty = new BoxDouble;
    duty->setMaxinum(100);
    hall->setItemDelegateForColumn(0x00, new BoxQItems);
    hall->setItemDelegateForColumn(0x01, volt);
    hall->setItemDelegateForColumn(0x02, volt);
    hall->setItemDelegateForColumn(0x02, volt);
    hall->setItemDelegateForColumn(0x03, volt);
    hall->setItemDelegateForColumn(0x04, freq);
    hall->setItemDelegateForColumn(0x05, freq);
    hall->setItemDelegateForColumn(0x06, duty);
    hall->setItemDelegateForColumn(0x07, duty);
    hall->setItemDelegateForColumn(0x08, volt);
}

void TypSetPwr::initSettings()
{
    int addr = tmpSet.value((4000 + Qt::Key_7)).toInt();
    passBox->setChecked(tmpSet.value(addr + 0).toInt() == 1);
    vmaxBox->setValue(tmpSet.value(addr + 1).toDouble());
    voltBox->setValue(tmpSet.value(addr + 4).toDouble());
    readBox->setCurrentIndex(tmpSet.value(addr + 5).toInt());
    pullBox->setCurrentIndex(tmpSet.value(addr + 6).toInt());
    countBox->setValue(tmpSet.value(addr + 7).toDouble());

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_7)).toInt() + CACHEPWR;
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHEPWR*t + i).toDouble();
            QString str = QString::number(real);
            if (t == CHECKPWR) {
                str = grades.at(str.toInt() % grades.size());
                mView->setData(mView->index(i, 0), str, Qt::DisplayRole);
                if (i < 3)
                    mHall->setData(mHall->index(i, 0), str, Qt::DisplayRole);
                continue;
            }
            if (t == TURNPWR1) {
                str = turns.at(str.toInt() % turns.size());
            }
            mView->setData(mView->index(i, t), str, Qt::DisplayRole);
        }
    }
    for (int t=0; t < mHall->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_7)).toInt() + CACHEPWR;
        for (int i=0; i < mHall->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHEPWR*t + i + 0x08).toDouble();
            QString str = QString::number(real);
            if (t == CHECKPWR) {
                continue;
            }
            mHall->setData(mHall->index(i, t), str, Qt::DisplayRole);
        }
    }
    int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台设置地址
    int mode = tmpSet.value(back + backMode).toInt();  // 测试模式
    for (int i=1; i < PWR_SIZE; i++) {
        view->setRowHidden(i, (mode == 1) ? true : false);
    }
    view->setFixedHeight((mode == 1) ? 120 : 360);
    isInit = (this->isHidden()) ? false : true;
}

void TypSetPwr::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_7)).toInt();

    tmpMsg.insert(addr + 0, QString::number(passBox->isChecked() ? 1 : 0));
    tmpMsg.insert(addr + 1, QString::number(vmaxBox->value()));
    tmpMsg.insert(addr + 4, QString::number(voltBox->value()));
    tmpMsg.insert(addr + 5, QString::number(readBox->currentIndex()));
    tmpMsg.insert(addr + 6, QString::number(pullBox->currentIndex()));
    tmpMsg.insert(addr + 7, QString::number(countBox->value()));

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_7)).toInt() + CACHEPWR;
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKPWR) {
                str = QString::number(grades.indexOf(mView->index(i, 0).data().toString()));
            }
            if (t == TURNPWR1) {
                str = QString::number(turns.indexOf(mView->index(i, t).data().toString()));
            }
            tmpMsg.insert(addr + CACHEPWR*t + i, str);
        }
    }
    for (int t=0; t < mHall->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_7)).toInt() + CACHEPWR;
        for (int i=0; i < mHall->rowCount(); i++) {
            QString str = QString::number(mHall->index(i, t).data().toDouble());
            if (t == CHECKPWR) {
                str = QString::number(grades.indexOf(mView->index(i, 0).data().toString()));
            }
            tmpMsg.insert(addr + CACHEPWR*t + i + 0x08, str);
        }
    }

    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetPwr::confSettings()
{
    tmpMap.insert("pass", QString::number(1));
    tmpMap.insert("pg_test", QString::number(passBox->isChecked() ? 1 : 0));
    tmpMap.insert("volt_input", QString::number(vmaxBox->value()));
    tmpMap.insert("volt", QString::number(voltBox->value()));
    tmpMap.insert("pull_up", QString::number(pullBox->currentIndex()));
    tmpMap.insert("count", QString::number(countBox->value()));

    QStringList names;
    names << "test" << "curr_max" << "curr_min" << "pwr_max" << "pwr_min"
          << "cap_max" << "cap_min" << "turn" << "speed_max" << "speed_min" << "time" ;
    QStringList tmp;
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKPWR) {
                str = QString::number(grades.indexOf(mView->index(i, 0).data().toString()));
            }
            if (t == TURNPWR1) {
                str = QString::number(turns.indexOf(mView->index(i, t).data().toString()));
            }
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    QStringList title;
    title << "pg_test" << "high_max" << "high_min" << "low_max" << "low_min"
          << "freq_max" << "freq_min" << "duty_max" << "duty_min" << "pg_volt";
    for (int t=0; t < title.size(); t++) {
        for (int i=0; i < mHall->rowCount(); i++) {
            QString str = QString::number(mHall->index(i, t).data().toDouble());
            tmp.append(str);
        }
        if (t != CHECKPWR) {
            tmpMap.insert(title.at(t), tmp.join(","));
        }
        tmp.clear();
    }
    config.insert("PWR", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetPwr::autoInput()
{
    change();
    if (isInit) {
        int row = view->currentIndex().row();
        int col = view->currentIndex().column();
        if (col == CHECKPWR) {
            QString str = mView->item(row, CHECKPWR)->text();
            if (row < 3)
                mHall->item(row, CHECKPWR)->setText(str);
        }
        if (col == CMAXPWR1 || col == CMINPWR1) {
            double max = mView->item(row, CMAXPWR1)->text().toDouble();
            double min = mView->item(row, CMINPWR1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, CMINPWR1)->setText("0.00");
            }
        }
        if (col == PMAXPWR1 || col == PMINPWR1) {
            double max = mView->item(row, PMAXPWR1)->text().toDouble();
            double min = mView->item(row, PMINPWR1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, PMINPWR1)->setText("0.00");
            }
        }
        if (col == VMAXPWR1 || col == VMINPWR1) {
            double max = mView->item(row, VMAXPWR1)->text().toDouble();
            double min = mView->item(row, VMINPWR1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, VMINPWR1)->setText("0.00");
            }
        }
    }
}

void TypSetPwr::autoHall()
{
    change();
    if (isInit) {
        int row = hall->currentIndex().row();
        int col = hall->currentIndex().column();
        if (col > 0 && col < 9) {
            int rmax = col - ((col % 2 == 0) ? 1 : 0);
            int rmin = rmax + 1;
            double max = mHall->item(row, rmax)->text().toDouble();
            double min = mHall->item(row, rmin)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mHall->item(row, rmin)->setText("0.00");
            }
        }
    }
}

void TypSetPwr::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetPwr::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 PWR"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetPwr::recvAppMsg(QTmpMap msg)
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

void TypSetPwr::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetPwr::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}


