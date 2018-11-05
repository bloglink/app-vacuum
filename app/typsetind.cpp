/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电感配置界面
*******************************************************************************/
#include "typsetind.h"

TypSetInd::TypSetInd(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetInd::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetInd::initLayout()
{
    layout = new QVBoxLayout(this);
    layout->addStretch();
}

void TypSetInd::initViewBar()
{
    QStringList headers;
    headers << tr("电感") << tr("端一") << tr("端二") << tr("电感单位")
            << tr("电感上限") << tr("电感下限") << tr("Q值上限") << tr("Q值下限")
            << tr("标准电感") << tr("线路补偿1") << tr("线路补偿2");
    mView = new BoxQModel;
    mView->setRowCount(IND_SIZE);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, CHECKIND)->setCheckable(true);
    }
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(CHECKIND, QHeaderView::Fixed);
    view->setColumnWidth(CHECKIND, 58);
    view->setFixedHeight(360);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));
    layout->addWidget(view);
}

void TypSetInd::initButtonBar()
{
    QGridLayout *btnLayout = new QGridLayout;

    int row = 0;
    nounBox = new QDoubleSpinBox(this);
    nounBox->setDecimals(1);
    nounBox->setFixedSize(125, 40);
    nounBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    nounBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(nounBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("不平衡度")), row, 0);
    btnLayout->addWidget(nounBox, row, 1);

    row++;
    timeBox = new QSpinBox(this);
    timeBox->setFixedSize(125, 40);
    timeBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    timeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(timeBox, SIGNAL(valueChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("平均次数")), row, 0);
    btnLayout->addWidget(timeBox, row, 1);

    row++;
    freqBox = new QComboBox(this);
    freqBox->setFixedSize(125, 40);
    freqBox->setView(new QListView);
    freqBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(freqBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("测试频率")), row, 0);
    btnLayout->addWidget(freqBox, row, 1);

    modeBox = new QComboBox(this);
    modeBox->setFixedSize(125, 40);
    modeBox->setView(new QListView);
    modeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(modeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("连接方式")), row, 2);
    btnLayout->addWidget(modeBox, row, 3);

    tempBox = new QComboBox(this);
    tempBox->setFixedSize(125, 40);
    tempBox->setView(new QListView);
    tempBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(tempBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(tempBox, row, 4);

    row++;
    smaxBox = new QDoubleSpinBox(this);
    smaxBox->setDecimals(1);
    smaxBox->setFixedSize(125, 40);
    smaxBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    smaxBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(smaxBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("标准上限")), row, 0);
    btnLayout->addWidget(smaxBox, row, 1);

    sminBox = new QDoubleSpinBox(this);
    sminBox->setDecimals(1);
    sminBox->setFixedSize(125, 40);
    sminBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    sminBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(sminBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("标准下限")), row, 2);
    btnLayout->addWidget(sminBox, row, 3);

    QPushButton *btnAuto = new QPushButton(this);
    btnAuto->setText(tr("自动计算"));
    btnAuto->setFixedHeight(40);
    connect(btnAuto, SIGNAL(clicked(bool)), this, SLOT(autoCal()));
    btnLayout->addWidget(btnAuto, row, 4, 1, 2);

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setFixedSize(125, 40);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    btnLayout->addWidget(btnSave, row, 7);

    btnLayout->setColumnStretch(6, 1);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(btnLayout);
    layout->addWidget(box);
    layout->addStretch();
}

void TypSetInd::initItemDelegate()
{
    isInit = false;
    freqs << "100" << "120" << "1k" << "10k";
    modes << tr("串联") << tr("并联");
    speed << tr("快测") << tr("慢测");
    units << tr("uH") << tr("mH");

    freqBox->addItems(freqs);
    modeBox->addItems(modes);
    tempBox->addItems(speed);

    view->setItemDelegateForColumn(CHECKIND, new BoxQItems);
    BoxDouble *port = new BoxDouble;
    port->setDecimals(0);
    port->setMininum(1);
    port->setMaxinum(8);
    view->setItemDelegateForColumn(PORTIND1, port);
    view->setItemDelegateForColumn(PORTIND2, port);
    view->setItemDelegateForColumn(UNITIND1, new BoxQItems);
    BoxDouble *real = new BoxDouble;
    real->setDecimals(3);
    real->setMininum(0);
    real->setMaxinum(2000);
    view->setItemDelegateForColumn(UPPERIND, real);
    view->setItemDelegateForColumn(LOWERIND, real);
    view->setItemDelegateForColumn(QMAXIND1, real);
    view->setItemDelegateForColumn(QMININD1, real);
    view->setItemDelegateForColumn(STDDIND1, real);
    view->setItemDelegateForColumn(COMPIND1, real);
    view->setItemDelegateForColumn(COMPIND2, real);
}

void TypSetInd::initSettings()
{
    int addr = tmpSet.value((4000 + Qt::Key_8)).toInt();
    nounBox->setValue(tmpSet.value(addr + 0).toDouble());
    timeBox->setValue(tmpSet.value(addr + 1).toDouble());
    freqBox->setCurrentIndex(tmpSet.value(addr + 2).toInt());
    modeBox->setCurrentIndex(tmpSet.value(addr + 3).toInt());
    tempBox->setCurrentIndex(tmpSet.value(addr + 4).toInt());
    smaxBox->setValue(tmpSet.value(addr + 5).toDouble());
    sminBox->setValue(tmpSet.value(addr + 6).toDouble());

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_8)).toInt() + CACHEIND;
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHEIND*t + i).toDouble();
            QString str = QString::number(real);
            if (t == CHECKIND) {
                real = (real == 0) ? Qt::Unchecked : Qt::Checked;
                mView->setData(mView->index(i, t), real, Qt::CheckStateRole);
                continue;
            }
            str = (t == UNITIND1) ? units.at(str.toInt() % units.size()) : str;
            mView->setData(mView->index(i, t), str, Qt::DisplayRole);
        }
    }
    isInit = (this->isHidden()) ? false : true;
}

void TypSetInd::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_8)).toInt();
    tmpMsg.insert(addr + 0, QString::number(nounBox->value()));
    tmpMsg.insert(addr + 1, QString::number(timeBox->value()));
    tmpMsg.insert(addr + 2, QString::number(freqBox->currentIndex()));
    tmpMsg.insert(addr + 3, QString::number(modeBox->currentIndex()));
    tmpMsg.insert(addr + 4, QString::number(tempBox->currentIndex()));
    tmpMsg.insert(addr + 5, QString::number(smaxBox->value()));
    tmpMsg.insert(addr + 6, QString::number(sminBox->value()));

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_8)).toInt() + CACHEIND;
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKIND) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == UNITIND1) {
                str = QString::number(units.indexOf(mView->index(i, t).data().toString()));
            }
            tmpMsg.insert(addr + CACHEIND*t + i, str);
        }
    }

    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetInd::confSettings()
{
    tmpMap.insert("noun", QString::number(nounBox->value()));
    QStringList m;
    m << QString::number(timeBox->value());
    m << QString::number(freqBox->currentIndex());
    m << QString::number(modeBox->currentIndex());
    m << QString::number(tempBox->currentIndex());
    tmpMap.insert("mode", m.join(","));
    tmpMap.insert("std_max", QString::number(smaxBox->value()));
    tmpMap.insert("std_min", QString::number(sminBox->value()));

    QStringList names;
    names << "test" << "port1" << "port2" << "unit" << "max" << "min"
          << "qmax" << "qmin" << "std" << "wire_comp1" << "wire_comp2";
    QStringList tmp;
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKIND) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == UNITIND1) {
                str = QString::number(units.indexOf(mView->index(i, t).data().toString()));
            }
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    config.insert("IND", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetInd::autoChange()
{
    change();
    if (isInit) {
        int r = view->currentIndex().row();
        int c = view->currentIndex().column();
        int i = 0;
        switch (c) {
        case UNITIND1:
            i = units.indexOf(view->currentIndex().data().toString());
            i = (i + 1) % units.size();
            mView->setData(mView->index(r, c), units.at(i), Qt::DisplayRole);
            break;
        default:
            break;
        }
    }
}

void TypSetInd::autoInput()
{
    change();
    if (isInit) {
        int row = view->currentIndex().row();
        int col = view->currentIndex().column();
        if (col == PORTIND1 || col == PORTIND2) {
            int max = mView->item(row, PORTIND1)->text().toInt();
            int min = mView->item(row, PORTIND2)->text().toInt();
            if (abs(max - min) % 3 == 0) {
                QString str = tr("端子号不能相同或差值为3的倍数");
                QMessageBox::warning(this, "警告", str, QMessageBox::Ok);
                str = QString::number(col == PORTIND1 ? (min-1)%3 : (max + 1)%3);
                mView->item(row, col)->setText(str);
            }
        }
        if ((col == UNITIND1) && row == 0) {
            QString str = mView->item(row, col)->text();
            for (int i=1; i < mView->rowCount(); i++) {
                mView->item(i, col)->setText(str);
            }
        }
        if (col == UPPERIND || col == LOWERIND) {
            double max = mView->item(row, UPPERIND)->text().toDouble();
            double min = mView->item(row, LOWERIND)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, QMAXIND1)->setText("0.00");
            }
        }
        if (col == QMAXIND1 || col == QMININD1) {
            double max = mView->item(row, QMAXIND1)->text().toDouble();
            double min = mView->item(row, QMININD1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, QMAXIND1)->setText("0.00");
            }
        }
    }
}

void TypSetInd::autoCal()
{
    for (int i=0; i < mView->rowCount(); i++) {
        double rstd = mView->item(i, STDDIND1)->text().toDouble();
        double rmin = rstd*(100-sminBox->value())/100;
        double rmax = rstd*(100+smaxBox->value())/100;
        mView->item(i, UPPERIND)->setText(QString::number(rmax, 'g', rmax < 1 ? 3 : 4));
        mView->item(i, LOWERIND)->setText(QString::number(rmin, 'g', rmax < 1 ? 3 : 4));
    }
}

void TypSetInd::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetInd::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 IND"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetInd::recvAppMsg(QTmpMap msg)
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

void TypSetInd::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetInd::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}


