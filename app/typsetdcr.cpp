/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置界面
*******************************************************************************/
#include "typsetdcr.h"

TypSetDcr::TypSetDcr(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetDcr::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetDcr::initLayout()
{
    layout = new QVBoxLayout(this);
    layout->addStretch();
}

void TypSetDcr::initViewBar()
{
    QStringList headers;
    headers << tr("电阻") << tr("端一") << tr("端二") << tr("线圈材料")
            << tr("电阻单位") << tr("电阻上限") << tr("电阻下限") << tr("标准电阻")
            << tr("线路补偿1") << tr("线路补偿2");
    mView = new BoxQModel;
    mView->setRowCount(DCR_SIZE);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < 8; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, CHECKDCR)->setCheckable(true);
    }
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(CHECKDCR, QHeaderView::Fixed);
    view->setColumnWidth(CHECKDCR, 58);
    view->setFixedHeight(360);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));
    layout->addWidget(view);
}

void TypSetDcr::initButtonBar()
{
    QGridLayout *btnLayout = new QGridLayout;

    int row = 0;
    timeBox = new QDoubleSpinBox(this);
    timeBox->setFixedSize(125, 40);
    timeBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    timeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(timeBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("测试时间")), row, 0);
    btnLayout->addWidget(timeBox, row, 1);

    row++;
    modeBox = new QSpinBox(this);
    modeBox->setMaximum(3);
    modeBox->setFixedSize(125, 40);
    modeBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    modeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btnLayout->addWidget(new QLabel(tr("平衡模式")), row, 0);
    btnLayout->addWidget(modeBox, row, 1);
    QString str;
    str += "0: 与平均值求差/平均值;abs(R1-Ravr)/Ravr,abs(R2-Ravr)/Ravr,abs(R3-Ravr)/Ravr\n";
    str += "1: 两两之间求差/标准值;abs(R1-R2)/Rstd,abs(R1-R3)/Rstd,abs(R2-R3)/Rstd\n";
    str += "2: 两两之间求差;abs(R1-R2),abs(R1-R3),abs(R2-R3)";
    modeBox->setToolTip(str);

    nounBox = new QDoubleSpinBox(this);
    nounBox->setDecimals(1);
    nounBox->setFixedSize(125, 40);
    nounBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    nounBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(nounBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("不平衡度")), row, 2);
    btnLayout->addWidget(nounBox, row, 3);

    row++;
    cnvtBox = new QComboBox(this);
    cnvtBox->addItem(tr("是"));
    cnvtBox->addItem(tr("否"));
    cnvtBox->setFixedSize(125, 40);
    cnvtBox->setView(new QListView);
    cnvtBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(cnvtBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("温度折算")), row, 0);
    btnLayout->addWidget(cnvtBox, row, 1);

    tempBox = new QDoubleSpinBox(this);
    tempBox->setDecimals(1);
    tempBox->setFixedSize(125, 40);
    tempBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    timeBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(tempBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("标准温度")), row, 2);
    btnLayout->addWidget(tempBox, row, 3);

    compBox = new QDoubleSpinBox(this);
    compBox->setDecimals(1);
    compBox->setMinimum(-99.9);
    compBox->setFixedSize(125, 40);
    compBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    compBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(compBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("补偿温度")), row, 4);
    btnLayout->addWidget(compBox, row, 5);

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

void TypSetDcr::initItemDelegate()
{
    isInit = false;
    wires << tr("铜") << tr("铝") << tr("铜铝");  // << tr("热敏电阻");
    units << tr("mΩ") << tr("Ω") << tr("kΩ");

    view->setItemDelegateForColumn(CHECKDCR, new BoxQItems);

    BoxDouble *port = new BoxDouble;
    port->setDecimals(0);
    port->setMininum(1);
    port->setMaxinum(8);
    view->setItemDelegateForColumn(PORTDCR1, port);
    view->setItemDelegateForColumn(PORTDCR2, port);

    view->setItemDelegateForColumn(WIREDCR1, new BoxQItems);
    view->setItemDelegateForColumn(UNITDCR1, new BoxQItems);

    BoxDouble *real = new BoxDouble;
    real->setDecimals(3);
    real->setMininum(0);
    real->setMaxinum(2000);
    view->setItemDelegateForColumn(UPPERDCR, real);
    view->setItemDelegateForColumn(LOWERDCR, real);
    view->setItemDelegateForColumn(SSTDDCR1, real);
    view->setItemDelegateForColumn(COMPDCR1, real);
    view->setItemDelegateForColumn(COMPDCR2, real);
}

void TypSetDcr::initSettings()
{
    int addr = tmpSet.value((4000 + Qt::Key_1)).toInt();
    timeBox->setValue(tmpSet.value(addr + 0).toDouble());
    modeBox->setValue(tmpSet.value(addr + 1).toDouble());
    nounBox->setValue(tmpSet.value(addr + 2).toDouble());
    cnvtBox->setCurrentIndex(tmpSet.value(addr + 3).toInt());
    tempBox->setValue(tmpSet.value(addr + 4).toDouble());
    compBox->setValue(tmpSet.value(addr + 5).toDouble());
    smaxBox->setValue(tmpSet.value(addr + 6).toDouble());
    sminBox->setValue(tmpSet.value(addr + 7).toDouble());

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_1)).toInt() + CACHEDCR;
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHEDCR*t + i).toDouble();
            QString str = QString::number(real);
            if (t == CHECKDCR) {
                real = (real == 0) ? Qt::Unchecked : Qt::Checked;
                mView->setData(mView->index(i, t), real, Qt::CheckStateRole);
                continue;
            }
            str = (t == WIREDCR1) ? wires.at(str.toInt() % wires.size()) : str;
            str = (t == UNITDCR1) ? units.at(str.toInt() % units.size()) : str;
            mView->setData(mView->index(i, t), str, Qt::DisplayRole);
        }
    }
    isInit = (this->isHidden()) ? false : true;
}

void TypSetDcr::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_1)).toInt();

    tmpMsg.insert(addr + 0, QString::number(timeBox->value()));
    tmpMsg.insert(addr + 1, QString::number(modeBox->value()));
    tmpMsg.insert(addr + 2, QString::number(nounBox->value()));
    tmpMsg.insert(addr + 3, QString::number(cnvtBox->currentIndex()));
    tmpMsg.insert(addr + 4, QString::number(tempBox->value()));
    tmpMsg.insert(addr + 5, QString::number(compBox->value()));
    tmpMsg.insert(addr + 6, QString::number(smaxBox->value()));
    tmpMsg.insert(addr + 7, QString::number(sminBox->value()));

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_1)).toInt() + CACHEDCR;
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKDCR) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == WIREDCR1) {
                str = QString::number(wires.indexOf(mView->index(i, t).data().toString()));
            }
            if (t == UNITDCR1) {
                str = QString::number(units.indexOf(mView->index(i, t).data().toString()));
            }
            tmpMsg.insert(addr + CACHEDCR*t + i, str);
        }
    }

    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetDcr::confSettings()
{
    tmpMap.insert("time", QString::number(timeBox->value()));
    tmpMap.insert("mode", QString::number(modeBox->value()));
    tmpMap.insert("noun", QString::number(nounBox->value()));
    tmpMap.insert("temp_comp", QString::number(cnvtBox->currentIndex()));
    tmpMap.insert("std_temp", QString::number(tempBox->value()));
    tmpMap.insert("comp_temp", QString::number(compBox->value()));
    tmpMap.insert("std_max", QString::number(smaxBox->value()));
    tmpMap.insert("std_min", QString::number(sminBox->value()));

    QStringList names;
    names << "test" << "port1" << "port2" << "wire" << "unit"
          << "max" << "min" << "std" << "wire_comp1" << "wire_comp2";
    QStringList tmp;
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKDCR) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == WIREDCR1) {
                str = QString::number(wires.indexOf(mView->index(i, t).data().toString()));
            }
            if (t == UNITDCR1) {
                str = QString::number(units.indexOf(mView->index(i, t).data().toString()));
            }
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    config.insert("DCR", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetDcr::autoChange()
{
    change();
    if (isInit) {
        int r = view->currentIndex().row();
        int c = view->currentIndex().column();
        int i = 0;
        switch (c) {
        case WIREDCR1:
            i = wires.indexOf(view->currentIndex().data().toString());
            i = (i + 1) % wires.size();
            mView->setData(mView->index(r, c), wires.at(i), Qt::DisplayRole);
            break;
        case UNITDCR1:
            i = units.indexOf(view->currentIndex().data().toString());
            i = (i + 1) % units.size();
            mView->setData(mView->index(r, c), units.at(i), Qt::DisplayRole);
            break;
        default:
            break;
        }
    }
}

void TypSetDcr::autoInput()
{
    change();
    if (isInit) {
        int row = view->currentIndex().row();
        int col = view->currentIndex().column();
        if (col == PORTDCR1 || col == PORTDCR2) {
            int max = mView->item(row, PORTDCR1)->text().toInt();
            int min = mView->item(row, PORTDCR2)->text().toInt();
            if (abs(max - min) % 3 == 0) {
                QString str = tr("端子号不能相同或差值为3的倍数");
                QMessageBox::warning(this, "警告", str, QMessageBox::Ok);
                str = QString::number(col == PORTDCR1 ? (min-1)%3 : (max + 1)%3);
                mView->item(row, col)->setText(str);
            }
        }
        if ((col == WIREDCR1 || col == UNITDCR1) && row == 0) {
            QString str = mView->item(row, col)->text();
            for (int i=1; i < mView->rowCount(); i++) {
                mView->item(i, col)->setText(str);
            }
        }
        if (col == UPPERDCR || col == LOWERDCR) {
            double max = mView->item(row, UPPERDCR)->text().toDouble();
            double min = mView->item(row, LOWERDCR)->text().toDouble();
            QString str = mView->item(row, UNITDCR1)->text();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, LOWERDCR)->setText("0.00");
            }
            if (str == "kΩ" && max > 20) {
                QMessageBox::warning(this, "警告", tr("上限20kΩ"), QMessageBox::Ok);
                mView->item(row, UPPERDCR)->setText("20.00");
            }
            if (str == "kΩ" && min > 20) {
                QMessageBox::warning(this, "警告", tr("上限20kΩ"), QMessageBox::Ok);
                mView->item(row, LOWERDCR)->setText("20.00");
            }
        }
    }
}

void TypSetDcr::autoCal()
{
    for (int i=0; i < mView->rowCount(); i++) {
        double rstd = mView->item(i, SSTDDCR1)->text().toDouble();
        double rmin = rstd*(100-sminBox->value())/100;
        double rmax = rstd*(100+smaxBox->value())/100;
        if (mView->item(i, UNITDCR1)->text() == "kΩ") {
            rmin = qMin(20.0, rmin);
            rmax = qMin(20.0, rmax);
        } else {
            rmin = qMin(2000.0, rmin);
            rmax = qMin(2000.0, rmax);
        }
        mView->item(i, UPPERDCR)->setText(QString::number(rmax, 'g', rmax < 1 ? 3 : 4));
        mView->item(i, LOWERDCR)->setText(QString::number(rmin, 'g', rmax < 1 ? 3 : 4));
    }
}

void TypSetDcr::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetDcr::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 DCR"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetDcr::recvAppMsg(QTmpMap msg)
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

void TypSetDcr::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetDcr::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}

