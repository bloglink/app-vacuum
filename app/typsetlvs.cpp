/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       低启配置界面
*******************************************************************************/
#include "typsetlvs.h"

TypSetLVS::TypSetLVS(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetLVS::initUI()
{
    initLayout();
    initViewBar();
    initButtonBar();
    initItemDelegate();
}

void TypSetLVS::initLayout()
{
    layout = new QVBoxLayout(this);
    layout->addStretch();
}

void TypSetLVS::initViewBar()
{
    QStringList headers;
    headers << tr("低启") << tr("电流上限") << tr("电流下限") << tr("功率上限")
            << tr("功率下限") << tr("容压上限") << tr("容压下限") << tr("转向") << tr("时间");
    mView = new BoxQModel;
    mView->setRowCount(LVS_SIZE);
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < LVS_SIZE; i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, CHECKLVS)->setCheckable(true);
        mView->item(i, CHECKLVS)->setText(QString("12%1").arg(i+3));
    }
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));

    view = new QTableView(this);
    view->setModel(mView);
    view->horizontalHeader()->setFixedHeight(30);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(CHECKLVS, QHeaderView::Fixed);
    view->setColumnWidth(CHECKLVS, 96);
    view->setFixedHeight(240);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange(QModelIndex)));
    layout->addWidget(view);
    view->hideColumn(VMAXLVS1);
    view->hideColumn(VMINLVS1);
}

void TypSetLVS::initButtonBar()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;

    int row = 0;
    passBox = new QSpinBox(this);
    passBox->setFixedSize(125, 40);
    passBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    passBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(passBox, SIGNAL(valueChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("输出通道")));
    btnLayout->addWidget(passBox);

    voltBox = new QSpinBox(this);
    voltBox->setMaximum(500);
    voltBox->setFixedSize(125, 40);
    voltBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    voltBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(voltBox, SIGNAL(valueChanged(int)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("输出电压")));
    btnLayout->addWidget(voltBox);

    compBox = new QDoubleSpinBox(this);
    compBox->setDecimals(1);
    compBox->setMinimum(-99.9);
    compBox->setFixedSize(125, 40);
    compBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    compBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(compBox, SIGNAL(valueChanged(double)), this, SLOT(change()));
    btnLayout->addWidget(new QLabel(tr("输出频率")));
    btnLayout->addWidget(compBox);
    btnLayout->addStretch();

    QPushButton *btnSave = new QPushButton(tr("保存"), this);
    btnSave->setFixedSize(125, 40);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    btnLayout->addWidget(btnSave);

    QGroupBox *box = new QGroupBox(this);
    box->setFixedHeight(90);
    box->setLayout(btnLayout);
    layout->addWidget(box);
    layout->addStretch();
}

void TypSetLVS::initItemDelegate()
{
    isInit = false;
    turns << tr("NULL") << tr("CCW") << tr("CW");

    view->setItemDelegateForColumn(CHECKLVS, new BoxQItems);

    BoxDouble *curr = new BoxDouble;
    view->setItemDelegateForColumn(CMAXLVS1, curr);
    view->setItemDelegateForColumn(CMINLVS1, curr);

    view->setItemDelegateForColumn(PMAXLVS1, curr);
    view->setItemDelegateForColumn(PMINLVS1, curr);

    view->setItemDelegateForColumn(TURNLVS1, new BoxQItems);

    view->setItemDelegateForColumn(TIMELVS1, curr);
}

void TypSetLVS::initSettings()
{
    int addr = tmpSet.value((4000 + Qt::Key_A)).toInt();
    passBox->setValue(tmpSet.value(addr + 0).toDouble());
    voltBox->setValue(tmpSet.value(addr + 4).toDouble());
    compBox->setValue(tmpSet.value(addr + 5).toDouble());

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_A)).toInt() + CACHELVS;
        for (int i=0; i < mView->rowCount(); i++) {
            double real = tmpSet.value(addr + CACHELVS*t + i).toDouble();
            QString str = QString::number(real);
            if (t == CHECKLVS) {
                real = (real == 0) ? Qt::Unchecked : Qt::Checked;
                mView->setData(mView->index(i, t), real, Qt::CheckStateRole);
                continue;
            }
            if (t == TURNLVS1) {
                str = turns.at(str.toInt() % turns.size());
            }
            mView->setData(mView->index(i, t), str, Qt::DisplayRole);
        }
    }
    view->setFixedHeight(120);
    isInit = (this->isHidden()) ? false : true;
}

void TypSetLVS::saveSettings()
{
    confSettings();
    int addr = tmpSet.value((4000 + Qt::Key_A)).toInt();

    tmpMsg.insert(addr + 0, QString::number(passBox->value()));
    tmpMsg.insert(addr + 4, QString::number(voltBox->value()));
    tmpMsg.insert(addr + 5, QString::number(compBox->value()));

    for (int t=0; t < mView->columnCount(); t++) {
        int addr = tmpSet.value((4000 + Qt::Key_A)).toInt() + CACHELVS;
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKLVS) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1);
            }
            if (t == TURNLVS1) {
                str = QString::number(turns.indexOf(mView->index(i, t).data().toString()));
            }
            tmpMsg.insert(addr + CACHELVS*t + i, str);
        }
    }

    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetLVS::confSettings()
{
    tmpMap.insert("pass", QString::number(passBox->value()));
    tmpMap.insert("volt", QString::number(voltBox->value()));
    tmpMap.insert("freq", QString::number(compBox->value()));

    QStringList names;
    names << "test" << "curr_max" << "curr_min" << "pwr_max" << "pwr_min"
          << "cap_max" << "cap_min" << "turn" << "time";
    QStringList tmp;
    for (int t=0; t < names.size(); t++) {
        for (int i=0; i < mView->rowCount(); i++) {
            QString str = QString::number(mView->index(i, t).data().toDouble());
            if (t == CHECKLVS) {
                int k = mView->index(i, t).data(Qt::CheckStateRole).toInt();
                str = QString::number((k == 0) ? 0 : 1+i);
            }
            if (t == TURNLVS1) {
                str = QString::number(turns.indexOf(mView->index(i, t).data().toString()));
            }
            tmp.append(str);
        }
        tmpMap.insert(names.at(t), tmp.join(","));
        tmp.clear();
    }
    config.insert("LVS", tmpMap);
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
    config.clear();
    tmpMap.clear();
}

void TypSetLVS::autoChange(QModelIndex index)
{
    change();
    if (isInit) {
        int r = index.row();
        int c = index.column();
        int i = 0;
        switch (c) {
        case TURNLVS1:
            i = turns.indexOf(view->currentIndex().data().toString());
            i = qMax(0, (i + 1) % turns.size());
            mView->setData(mView->index(r, c), turns.at(i), Qt::DisplayRole);
            break;
        default:
            break;
        }
    }
}

void TypSetLVS::autoInput()
{
    change();
    if (isInit) {
        int row = view->currentIndex().row();
        int col = view->currentIndex().column();
        if (col == CMAXLVS1 || col == CMINLVS1) {
            double max = mView->item(row, CMAXLVS1)->text().toDouble();
            double min = mView->item(row, CMINLVS1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, CMINLVS1)->setText("0.00");
            }
        }
        if (col == PMAXLVS1 || col == PMINLVS1) {
            double max = mView->item(row, PMAXLVS1)->text().toDouble();
            double min = mView->item(row, PMINLVS1)->text().toDouble();
            if (min > max) {
                QMessageBox::warning(this, "警告", tr("下限大于上限"), QMessageBox::Ok);
                mView->item(row, PMINLVS1)->setText("0.00");
            }
        }
    }
}

void TypSetLVS::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetLVS::recvShowEvent()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", QString("6004 LVS"));
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    initSettings();
}

void TypSetLVS::recvAppMsg(QTmpMap msg)
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

void TypSetLVS::showEvent(QShowEvent *e)
{
    this->setFocus();
    recvShowEvent();
    e->accept();
}

void TypSetLVS::hideEvent(QHideEvent *e)
{
    isInit = false;
    e->accept();
}
